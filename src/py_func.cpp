// py.func

// anything:    run list (>0 elements) as python code; outputs return data through outlet
// set:         set code without output
// bang:        run previously set code
// import:      [callable_object] [module callable_object]

// arguments = 'import'

#include <stdarg.h>

#include "compatibility.h"

#include "ext.h"
#include "ext_obex.h"

#include <stdio.h>

#include "stdlib.h"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

#include <string>

#include "convert.hpp"
//#include "dyn_class.hpp"

//
//bench
#include <chrono>

#include "py_box.hpp"

typedef struct _py_func : public PyBox {
//    t_object ob;
//    void* outlet_1;

    std::string module = "";
    std::string func = "";

    py::handle obj;

    py::object py_func = py::eval("lambda: Nonde");
    py::object py_arg = py::none();

//    t_symbol* _list;

//    std::vector<c_atom> _output;

} t_py_func;

//

void* py_func_new(t_symbol* s, long argc, t_atom* argv);
void py_func_free(t_py_func* x);
void py_func_bang(t_py_func* x);

void* py_func_class;

//

bool _py_func_import_func(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    if (!check_arguments(argc, argv, { A_SYM })) {
        return false;
    }

    x->module = "";
    x->func = argv[0].a_w.w_sym->s_name;
    return true;
}

bool _py_func_import_module_func(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    if (!check_arguments(argc, argv, { A_SYM, A_SYM })) {
        return false;
    }

    x->module = argv[0].a_w.w_sym->s_name;
    x->func = argv[1].a_w.w_sym->s_name;
    return true;
}



void _py_func_precompile(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    try {

        maxpy_import_module(x->module);

        x->py_arg = py::none(); //to_object(argc,argv);
        x->py_func = py::eval(x->module).attr(x->func.c_str());

        auto mod = py::eval(x->module).ptr();
        x->py_func = py::reinterpret_borrow<py::object>(PyObject_GetAttrString(mod, x->func.c_str()));

    } catch (std::exception& e) {
        error("py.func: failed to import module %s:", x->module.c_str());
        error(e.what());

        x->py_arg = py::none();
        x->py_func = py::eval("lambda:: None");
    };
}
// ---

void py_func_import(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{

    if (!(_py_func_import_func(x, s, argc, argv) || _py_func_import_module_func(x, s, argc, argv)))
        return;

    _py_func_precompile(x, s, argc, argv);
}

void py_func_set(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    x->py_arg = to_object(argc, argv);
}

void py_func_anything(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{

    x->py_arg = to_object(s, argc, argv);

    py_func_bang(x);
}

void py_func_int(t_py_func* x, int i)
{

    x->py_arg = py::int_(i);

    py_func_bang(x);
}

void py_func_float(t_py_func* x, double f)
{

    x->py_arg = py::float_(f);

    py_func_bang(x);
}

void py_func_symbol(t_py_func* x, t_symbol* s)
{

    x->py_arg = py::str(s->s_name);

    py_func_bang(x);
}

void py_func_bang(t_py_func* x)
{

    try {

        py::object obj;
        if (x->py_arg.is_none())
            obj = x->py_func();
        else
            obj = x->py_func(x->py_arg);

        x->Output(obj);
//        to_atoms(x->_output, obj);
//        outlet_anything(x->outlet_1, x->_list, x->_output.size(), x->_output.data());

    } catch (std::exception& e) {
        error("py.func: %s", e.what());
        return;
    };
}

void* py_func_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_func* x = NULL;

    if ((x = (t_py_func*)object_alloc((t_class*)py_func_class))) {
        x->outlet_1 = outlet_new(x, NULL);
    }

    x->_list = gensym("list");

    py_func_import(x, NULL, argc, argv);

    return (x);
}

//
void py_func_free(t_py_func* x)
{
    //    py::finalize_interpreter();
}

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.func", (method)py_func_new, (method)py_func_free, (long)sizeof(t_py_func),
        0L, A_GIMME, 0);

    class_addmethod(c, (method)py_func_anything, "anything", A_GIMME, 0);
    class_addmethod(c, (method)py_func_int, "int", A_GIMME, 0);
    class_addmethod(c, (method)py_func_float, "float", A_GIMME, 0);
    class_addmethod(c, (method)py_func_symbol, "symbol", A_GIMME, 0);
    class_addmethod(c, (method)py_func_bang, "bang", A_GIMME, 0);

    class_addmethod(c, (method)py_func_import, "import", A_GIMME, 0);

    class_addmethod(c, (method)py_func_set, "set", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_func_class = c;

    maxpy_init();

    object_post((t_object*)py_func_class, "py.func: loaded");

    return 0;
}
}
