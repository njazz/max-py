// py.func

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

#include "dyn_class.hpp"

#include "convert.hpp"

//
//bench
#include <chrono>

typedef struct _py_func {
    t_object ob;
    void* out1;

    std::string module = "";
    std::string func = "";

    py::handle obj;

    py::object py_func;
    py::object py_arg;

    t_symbol* _list;

    std::vector<c_atom> _output;

} t_py_func;

//1
void* py_func_new(t_symbol* s, long argc, t_atom* argv);
void py_func_free(t_py_func* x);

void* py_func_class;

#pragma mark -

void py_func_import(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argc > 2)
        return;

    post("%i", argc);

    if (argc == 1) {
        if (argv[0].a_type != A_SYM)
            return;
        x->module = "";
        x->func = argv[0].a_w.w_sym->s_name;
    } else {
        if (argv[0].a_type != A_SYM)
            return;
        if (argv[1].a_type != A_SYM)
            return;

        x->module = argv[0].a_w.w_sym->s_name;
        x->func = argv[1].a_w.w_sym->s_name;

        try {

            t_symbol* ret = NULL;
            //            gensym("none");
            path_absolutepath(&ret, gensym((x->module + ".py").c_str()), 0, 0);

            if (!ret) {
                // trying to import module in Python search path
                py::exec("import " + x->module);
                return;
            }

            short id;
            char fn[MAX_FILENAME_CHARS];
            path_frompathname(ret->s_name, &id, fn);
            char ret2[MAX_PATH_CHARS];
            path_toabsolutesystempath(id, fn, ret2);

            py::exec("import importlib.util\nspec = importlib.util.spec_from_file_location('" + x->module + "', '" + ret2 + "')\n" + x->module + " = importlib.util.module_from_spec(spec)\nspec.loader.exec_module(" + x->module + ")\n");

            // store func in var
            //            py::exec("__runme = " + x->module + "." + x->func);
            //            x->module = "";
            //            x->func = "__runme";

            //            x->code = (x->module == "") ? "" : x->module + ".";
            //            x->code += x->func + "()";

            //            x->code += to_func_arguments(s, argc, argv) + ")";

            post("py.func: loaded %s", x->module.c_str());

            // TODO: check if fn exists

            x->py_arg = py::none(); //to_object(argc,argv);
            x->py_func = py::eval(x->module).attr(x->func.c_str());


            auto mod = py::eval(x->module).ptr();
            x->py_func = py::reinterpret_borrow<py::object>(PyObject_GetAttrString(mod, x->func.c_str()));


            //            x->arg = "";

        } catch (std::exception& e) {
            error("py.func: failed to import module %s:", x->module.c_str());
            error(e.what());

            x->py_arg = py::none();
            x->py_func = py::eval("lambda:: None");
        };
    }
}

void py_func_bang(t_py_func* x);

void py_func_anything(t_py_func* x, t_symbol* s, long argc, t_atom* argv)
{
    //    x->code = (x->module == "") ? "" : x->module + ".";
    //    x->code += x->func + "(";
    //    x->code += to_func_arguments(s, argc, argv) + ")";

    //    x->arg = to_func_arguments(s, argc, argv);

    x->py_arg = to_object(s,argc, argv);

    py_func_bang(x);
}

void py_func_int(t_py_func* x, int i)
{
    //    x->code = (x->module == "") ? "" : x->module + ".";
    //    x->code += x->func + "(";
    //    x->code += std::to_string(i) + ")";

    //    x->arg = std::to_string(i);

    x->py_arg = py::int_(i);

    py_func_bang(x);

    //    try {
    //        post(code.c_str());
    //        auto obj = py::eval(code.c_str());
    //        auto a = to_atoms(obj);
    //        outlet_anything(x->out1, gensym("list"), a.size(), a.data());
    //    } catch (std::exception& e) {
    //        error("py.func: %s", e.what());
    //    };
}

void py_func_float(t_py_func* x, double f)
{
    //    x->code = (x->module == "") ? "" : x->module + ".";
    //    x->code += x->func + "(";
    //    x->code += std::to_string(f) + ")";

    //    x->arg = std::to_string(f);
    x->py_arg = py::float_(f);

    py_func_bang(x);
}

void py_func_symbol(t_py_func* x, t_symbol* s)
{
    //    x->code = (x->module == "") ? "" : x->module + ".";
    //    x->code += x->func + "(";
    //    x->code += std::to_string(f) + ")";

    //    x->arg = std::to_string(f);
    x->py_arg = py::str(s->s_name);

    py_func_bang(x);
}

void py_func_bang(t_py_func* x)
{
    //    py::object obj;
//    auto st = std::chrono::high_resolution_clock::now();

    try {
        //        if (x->arg == "")
        //            x->obj = x->py_func();
        //        else

        py::object obj;
        if (x->py_arg.is_none())
            obj = x->py_func();
//            x->obj = PyObject_CallObject(x->py_func.ptr(), 0);
        else
            obj = x->py_func(x->py_arg);
//            x->obj = PyObject_CallObject(x->py_func.ptr(), x->py_arg.ptr());





//        post("func time %f", (std::chrono::high_resolution_clock::now() - st) / 1000.);

        //        auto a = to_atoms(x->obj);

        to_atoms(x->_output, obj);

//        post("atoms time %f", (std::chrono::high_resolution_clock::now() - st) / 1000.);

        outlet_anything(x->out1, x->_list, x->_output.size(), x->_output.data());

//        post("outlet time %f", (std::chrono::high_resolution_clock::now() - st) / 1000.);

    } catch (std::exception& e) {
        error("py.func: %s", e.what());
        return;
    };
}

void* py_func_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_func* x = NULL;

    if ((x = (t_py_func*)object_alloc((t_class*)py_func_class))) {
        x->out1 = outlet_new(x, NULL);
    }

    x->_list = gensym("list");

    if ((argc == 1) || (argc == 2))
        py_func_import(x, NULL, argc, argv);
    else {
        error("required arguments: [function] or [module function]");
        return 0;
    }

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

    //    class_addmethod(c, (method)py_func_import, "import", A_GIMME, 0);

    class_addmethod(c, (method)py_func_anything, "anything", A_GIMME, 0);
    class_addmethod(c, (method)py_func_int, "int", A_GIMME, 0);
    class_addmethod(c, (method)py_func_float, "float", A_GIMME, 0);
    class_addmethod(c, (method)py_func_symbol, "symbol", A_GIMME, 0);
    class_addmethod(c, (method)py_func_bang, "bang", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_func_class = c;

    init_interpreter();

    object_post((t_object*)py_func_class, "py.func: loaded");

    return 0;
}
}
