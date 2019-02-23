#include "convert.hpp"

#include <pybind11/embed.h>
namespace py = pybind11;

static bool _initialized = false;

#include <chrono>

extern "C" {
extern void init_interpreter()
{
    if (_initialized)
        return;

    _initialized = true;
    py::initialize_interpreter();

    py::exec("import platform");
    auto obj = py::eval("platform.python_version()");
    post("Python version: %s", obj.cast<std::string>().c_str());

}
}

// ---

// ---

t_atom make_atom(const float& v)
{
    t_atom a; // = t_atom();

    a.a_type = A_FLOAT;
    a.a_w.w_float = v;
    return a;
}

t_atom make_atom(const double& v)
{
    t_atom a; // = t_atom();

    a.a_type = A_FLOAT;
    a.a_w.w_float = v;
    return a;
}

//template <>
//inline
t_atom make_atom(const long& v)
{
    t_atom a; // = t_atom();

    a.a_type = A_LONG;
    a.a_w.w_long = v;
    return a;
}

//template <>
//inline
t_atom make_atom(const std::string& v)
{
    t_atom a; // = t_atom();

    a.a_type = A_SYM;
    a.a_w.w_sym = gensym(v.c_str());
    return a;
}

//inline
t_atom make_atom_obj(py::handle obj)
{
    if (py::isinstance<py::float_>(obj))
        //    if (PyFloat_Check(obj.ptr()))
        //        return make_atom(obj.cast<float>());
        return make_atom(PyFloat_AsDouble(obj.ptr()));

    if (py::isinstance<py::int_>(obj))
        //    if (PyLong_Check(obj.ptr()))
        //        return make_atom(obj.cast<long>());
        return make_atom(PyLong_AsLong(obj.ptr()));

    if (py::isinstance<py::str>(obj))
        return make_atom(obj.cast<std::string>());

    return make_atom(py::str(obj).cast<std::string>());
}

// ---

#include "ext.h"

//extern std::vector<c_atom> to_atoms(const py::object& obj)
//{

//    if (obj.is_none())
//        return {};

//    if (py::isinstance<py::list>(obj)) {

//        auto lst_ = obj.cast<py::list>();

//        std::vector<c_atom> ret; // = std::array<t_atom>(lst_.size());

//        ret.resize(lst_.size());

//        int i = 0;

//        //auto& e : obj

//        //        post("len %i", lst_.size());

//        //        for (int i = 0; i < lst_.size(); i++) {
//        for (auto& e : obj) {

//            //                        c_atom a;
//            //                        a=e;
//            //                        ret.push_back(a);
//            ret[i] = e;
//            i++;

//            //            ret[i] = lst_[i]; //e;
//            //            ret[i] = make_atom_obj(e);//lst_[i]);
//            //            i++;
//        }
//        //            ret.push_back(make_atom_obj(lst_[i]));
//        //    }

//        return ret;
//    }

//    c_atom ret;
//    ret = obj;
//    return { ret }; //{make_atom(py::str(obj).cast<std::string>()) };
//}

extern void to_atoms(std::vector<c_atom>& dest, const py::handle& obj)
{

    if (obj.is_none())
        //        return {};
    {
        dest.resize(0);
        return;
}



    if (py::isinstance<py::list>(obj)) {

        auto lst_ = obj.cast<py::list>();

        //        std::vector<c_atom> ret; // = std::array<t_atom>(lst_.size());

        auto len_ = PyList_Size(lst_.ptr());

        if (dest.size() != len_)
            dest.resize(lst_.size());

        int i = 0;

        //auto& e : obj

        //        post("len %i", lst_.size());



                for (int i = 0; i < len_; i++) {
//        for (auto& e : obj) {

            //                        c_atom a;
            //                        a=e;
            //                        ret.push_back(a);

//                    auto st = std::chrono::high_resolution_clock::now();
            dest[i] = PyList_GetItem(obj.ptr(),i);//e;
//            i++;

//            post("atom convert time %f", std::chrono::high_resolution_clock::now()-st);

            //            ret[i] = lst_[i]; //e;
            //            ret[i] = make_atom_obj(e);//lst_[i]);
            //            i++;
        }
        //            ret.push_back(make_atom_obj(lst_[i]));
        //    }

        //        return ret;
        return;
    }

    dest.resize(1);
    dest[0] = obj;

    //    c_atom ret;
    //    ret = obj;
    //    return { ret }; //{make_atom(py::str(obj).cast<std::string>()) };
}

// UNUSED:
//extern void to_atoms(std::vector<t_atom>& dest, const py::object& obj)
//{

//    if (obj.is_none())
//        //        return {};
//        dest = {};

//    if (py::isinstance<py::list>(obj)) {

//        //        std::vector<t_atom> ret;
//        dest.clear();

//        for (auto& e : obj) {
//            dest.push_back(make_atom_obj(e));
//        }

//        //        return ret;
//    }

//    //return
//    {
//        dest = { make_atom_obj(obj) };
//    };
//}

// --

extern py::object to_object(long argc, t_atom* argv)
{

    // list
    if (argc > 1) {
        auto ret = py::list();

        for (int i = 0; i < argc; i++) {
            ret.append(to_object(1, &argv[i]));
        }

        return ret;
    } else if (argc == 1) {
        //        py::object ret = py::none();
        if (argv[0].a_type == A_FLOAT)
            return py::float_(argv[0].a_w.w_float);
        if (argv[0].a_type == A_LONG)
            return py::int_(argv[0].a_w.w_long);
        if (argv[0].a_type == A_SYM)
            return py::str(argv[0].a_w.w_sym->s_name);
    }

    return py::none();
}

py::object to_object(t_symbol* s, long argc, t_atom *argv)
{
    if (argc==0)
        if (s)
            return py::str(s->s_name);

    return to_object(argc,argv);

}

extern std::string to_string(long argc, t_atom* argv)
{
    std::string ret;

    for (int i = 0; i < argc; i++) {
        auto a = argv[i];

        std::string a_str = "";

        if (a.a_type == A_FLOAT)
            a_str = std::to_string(a.a_w.w_float);
        if (a.a_type == A_LONG)
            a_str = std::to_string(a.a_w.w_long);
        if (a.a_type == A_SYM)
            a_str = a.a_w.w_sym->s_name;

        ret += a_str + " ";
    }
    return ret;
}
extern std::string to_string(t_atom* argv)
{
    return to_string(1, argv);
}

extern std::string to_func_argument_string(long argc, t_atom* argv)
{
    std::string ret;

    for (int i = 0; i < argc; i++) {
        auto a = argv[i];

        std::string a_str = "";

        if (a.a_type == A_FLOAT)
            a_str = std::to_string(a.a_w.w_float);
        if (a.a_type == A_LONG)
            a_str = std::to_string(a.a_w.w_long);
        if (a.a_type == A_SYM)
            a_str = a.a_w.w_sym->s_name;

        ret += a_str;
        if (i < (argc - 1))
            ret += ",";
    }
    return ret;
}

extern std::string to_func_argument_string(t_symbol* s, long argc, t_atom* argv)
{
    auto p2 = to_func_argument_string(argc, argv);
    if (p2 == "")
        return std::string(s->s_name);
    else
        return std::string(s->s_name) + ", " + p2;
}
