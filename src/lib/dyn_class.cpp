#include "compatibility.h"

#include <stdarg.h>

#include "ext.h"
#include "ext_obex.h"

#include <unordered_map>

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "dyn_class.hpp"

#include <functional>

typedef struct _dyn_class_object {
    t_object ob;
    std::unordered_map<std::string, py::object> attr = {};
    std::unordered_map<std::string, py::object> method = {};
} t_dyn_class_object;

std::unordered_map<std::string, t_class*> _class_map;

// ---

t_dyn_class_object* _dyn_class_new(t_symbol* s, long argc, t_atom* argv)
{
    t_dyn_class_object* x = NULL;

    std::string name = std::string(s->s_name);
    if (_class_map.find(name) == _class_map.end())
        return NULL;

    x = (t_dyn_class_object*)object_alloc(_class_map[name]);
    return (x);
}

void _dyn_class_free(t_dyn_class_object* x)
{
}

// ---

void dyn_class_register(std::string name, t_class* cls)
{
    // TODO: check if already exists

    class_register(CLASS_BOX, cls);
    _class_map[name] = cls;
};

// ---

typedef std::function<void(t_object*, t_symbol*, long, t_atom*)> dyn_method_func;

// ---

void dyn_class_create(std::string name)
{
    t_class* ret = class_new(name.c_str(), (method)_dyn_class_new, (method)_dyn_class_free, (long)sizeof(_dyn_class_object),
        0L, A_GIMME, 0);
    dyn_class_register(name, ret);
};

// ---

//template <typename CT, typename ... A> struct function
//: public function<decltype(&CT::operator())(A...)> {};

//template <typename C> struct function<C> {
//private:
//    C mObject;

//public:
//    function(const C & obj)
//        : mObject(obj) {}

//    template<typename... Args> typename
//    std::result_of<C(Args...)>::type operator()(Args... a) {
//        return this->mObject.operator()(a...);
//    }

//    template<typename... Args> typename
//    std::result_of<const C(Args...)>::type operator()(Args... a) const {
//        return this->mObject.operator()(a...);
//    }
//};

//namespace make {
//    template<typename C> auto function(const C & obj) {
//        return ::function<C>(obj);
//    }

//template<typename C> auto function_ptr(const C & obj) {
//    return new ::function<C>(obj);
//}
//}
// ---

void _dyn_class_method(t_object*, t_symbol* s, long a_c, t_atom* a_v)
{
    post("test method: %s", s->s_name);
}

void dyn_class_add_method(std::string cls_name, std::string m_name)
{
    auto cls = _class_map[cls_name];
    if (!cls)
        return;

    //    auto lambda =
    //    auto ptr = &decltype(lambda)::operator();

    //    auto method1 = make::function_ptr([](t_object*, t_symbol*, long, t_atom*) {
    //        post("test method");
    //    });

    //    const auto p = &method1.operator();

    class_addmethod(cls, (method)&_dyn_class_method, m_name.c_str(), A_GIMME, 0);
}
