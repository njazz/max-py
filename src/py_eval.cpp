// py.eval
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
#include <chrono>

typedef struct _py_eval {
    t_object ob;
    void* out1;

    std::string code = "";
    py::object out = py::object();

    py::object compiled = py::none();

    t_symbol* _list;

    std::string id = "";

        std::vector<c_atom> _output;

    //    std::vector<t_atom> _out_list;

} t_py_eval;

//
void* py_eval_new(t_symbol* s, long argc, t_atom* argv);
void py_eval_free(t_py_eval* x);

void py_eval_message(t_py_eval* x, t_symbol* s, long argc, t_atom* argv);

//
void* py_eval_class;

//

void py_eval_set(t_py_eval* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = to_string(argc, argv);
}

void py_eval_bang(t_py_eval* x);

void py_eval_message(t_py_eval* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = (s) ? (std::string(s->s_name) + " ") : "";
    x->code += to_string(argc, argv);

    try {
        //        x->compiled = py::eval("eval(compile(\"\"\"" + x->code + "\"\"\",'<string>','eval'))");
        x->id = std::to_string((long)x);

        py::exec(std::string("e_" + x->id + " = compile(\"\"\"" + x->code + "\"\"\",'<string>','eval')").c_str());

        x->compiled = py::eval("lambda: eval(e_" + x->id + ")");


    } catch (std::exception& e) {
        error("py.eval: compile error %s", e.what());
        x->compiled = py::eval("lambda: None");
    };

    py_eval_bang(x);
}

void py_eval_bang(t_py_eval* x)
{
    auto st = std::chrono::high_resolution_clock::now();
    try {
        //                auto ev = py::eval("eval");
        //        auto obj = x->compiled;

        auto obj = x->compiled(); //py::eval("eval(e_"+x->id +")");

//       auto obj = py::eval("eval(e_" + x->id+")");

        //         py::eval(x->code.c_str());
        //                        post("eval time %f", (std::chrono::high_resolution_clock::now() - st) / 1000.);

        //        post(py::str(obj).cast<std::string>().c_str());

//        auto a = to_atoms(obj);
        to_atoms(x->_output, obj);

        //        to_atoms(x->_out_list, obj);

        outlet_anything(x->out1, x->_list, x->_output.size(), x->_output.data());

        //                        post("total time %f", (std::chrono::high_resolution_clock::now() - st) / 1000.);

    } catch (std::exception& e) {
        error("py.eval: %s", e.what());
    };
}

void* py_eval_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_eval* x = NULL;

    if ((x = (t_py_eval*)object_alloc((t_class*)py_eval_class))) {

        x->out1 = outlet_new(x, NULL);
    }

    x->_list = gensym("list");

    if (argc)
        py_eval_message(x, NULL, argc, argv);

    return (x);
}
//
void py_eval_free(t_py_eval* x)
{
    //    py::finalize_interpreter();
}

// ---

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.eval", (method)py_eval_new, (method)py_eval_free, (long)sizeof(t_py_eval),
        0L, A_GIMME, 0);

    class_addmethod(c, (method)py_eval_set, "set", A_GIMME, 0);
    class_addmethod(c, (method)py_eval_message, "anything", A_GIMME, 0);
    class_addmethod(c, (method)py_eval_bang, "bang", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_eval_class = c;

    init_interpreter();

    try {
        py::eval("import maxpy as max");
    } catch (std::exception& e) {
        error("py.eval: failed");
    }

    object_post((t_object*)py_eval_class, "py_eval: loaded");

    return 0;
}
}
