#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "convert.hpp"

#include <unordered_map>

std::unordered_map<std::string, t_symbol*> _gensym_mock_map;

t_symbol* gensym(const char* s)
{
    if (_gensym_mock_map.find(std::string(s)) == _gensym_mock_map.end()) {
        auto sym = new t_symbol;
        auto s_name = new char[strlen(s)];
        strcpy(s_name, s);
        sym->s_name = s_name;
        sym->s_thing = 0;

        _gensym_mock_map[std::string(s)] = sym;
    }

    return _gensym_mock_map[std::string(s)];
};

struct __TestVector {
    template <typename T>
    static void Func(T val)
    {
        std::vector<c_atom> atoms;

        to_atoms(atoms, val);

        if (atoms[0].a_type == A_SYM)
            //            printf("sym 0 %s\n", atoms[0].a_w.w_sym->s_name);

            for (int i = 0; i < 5; i++) {
                REQUIRE(atoms[i] == val[i]);
            }

        //        if (atoms[0].a_type == A_SYM)
        //            printf("sym %s\n", atoms[0].a_w.w_sym->s_name);

        //        printf("%s\n", to_string(atoms.size(), atoms.data()).c_str());

        auto obj = to_object(atoms.size(), atoms.data());

        std::vector<c_atom> atoms2;
        to_atoms(atoms2, obj);

        //        printf("%s\n", to_string(atoms2.size(), atoms2.data()).c_str());

        for (int i = 0; i < 5; i++) {
            REQUIRE(atoms2[i] == val[i]);
        }
    }
};

TEST_CASE("Converters", "[1]")
{
    SECTION("test gensym mock")
    {
        c_atom s;
        s = std::string("test1");
        REQUIRE(strcmp(s.a_w.w_sym->s_name, "test1") == 0);
        c_atom s2;
        s2 = std::string("test1");
        gensym("t2");
        gensym("t3");
        REQUIRE(strcmp(s.a_w.w_sym->s_name, "test1") == 0);
        REQUIRE(strcmp(s2.a_w.w_sym->s_name, "test1") == 0);
    }
    SECTION("atoms - basic types")
    {
        c_atom a;

        a = .33;

        REQUIRE(a.a_type == A_FLOAT);
        REQUIRE(a.a_w.w_float == .33);

        a = (long)3;

        REQUIRE(a.a_type == A_LONG);
        REQUIRE(a.a_w.w_long == 3);

        a = "symbol-1";

        REQUIRE(a.a_type == A_SYM);
        REQUIRE(a.a_w.w_sym == gensym("symbol-1"));
        REQUIRE(a.a_w.w_sym != gensym("symbol-2"));

        //        printf("%s", to_string(&a).c_str());
    }

    SECTION("atom - python objects")
    {
        auto __g = py::scoped_interpreter();

        auto p_str = py::eval("str('string')");
        auto p_int = py::eval("int(3)");
        auto p_float = py::eval("float(0.33)");

        c_atom a;

        a = p_str;

        REQUIRE(a.a_type == A_SYM);
        REQUIRE(a.a_w.w_sym == gensym("string"));

        a = p_int;

        REQUIRE(a.a_type == A_LONG);
        REQUIRE(a.a_w.w_long == 3);

        a = p_float;

        REQUIRE(a.a_type == A_FLOAT);
        REQUIRE(a.a_w.w_float == .33);
    }

    SECTION("to object and back")
    {
        auto __g = py::scoped_interpreter();

        __TestVector::Func(std::vector<long>({ 1, 3, 5, 7, 9 }));
        __TestVector::Func(std::vector<float>({ .1, .3, .5, .7, .9 }));
        __TestVector::Func(std::vector<std::string>({ "1", "3", "5", "7", "9" }));
    }
}
