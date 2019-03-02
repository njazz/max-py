// py.eval

// anything:    evaluate list (>1 elements) as python code
// set:         set code without output
// bang:        evaluate previously set code

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
#include "dyn_class.hpp"

//
#include <chrono>

//
#include "py_box.hpp"

typedef struct _py_eval : public PyBox {

    std::string code = "";
    //    py::object out = py::object();

    py::object compiled = py::none();

    //    t_symbol* _list;

    std::string id = "";

} t_py_eval;

//
void* py_eval_new(t_symbol* s, long argc, t_atom* argv);
void py_eval_free(t_py_eval* x);

void py_eval_message(t_py_eval* x, t_symbol* s, long argc, t_atom* argv);

//
void* py_eval_class;

//

void py_eval_bang(t_py_eval* x);

//
void _py_eval_precompile(t_py_eval* x)
{
    try {
        x->id = std::to_string((long)x);

        py::exec(std::string("e_" + x->id + " = compile(\"\"\"" + x->code + "\"\"\",'<string>','eval')").c_str());

        x->compiled = py::eval("lambda: eval(e_" + x->id + ")");

    } catch (std::exception& e) {
        error("py.eval: compile error %s", e.what());
        x->compiled = py::eval("lambda: None");
    };
}

void py_eval_set(t_py_eval* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = to_string(argc, argv);
    _py_eval_precompile(x);
}

void py_eval_message(t_py_eval* x, t_symbol* s, long argc, t_atom* argv)
{

    x->code = to_string(s, argc, argv);
    _py_eval_precompile(x);
    py_eval_bang(x);
}

void py_eval_bang(t_py_eval* x)
{
    auto st = std::chrono::high_resolution_clock::now();
    try {

        auto obj = x->compiled(); //py::eval("eval(e_"+x->id +")");
        //        to_atoms(x->_output, obj);
        //        outlet_anything(x->outlet_1, x->_list, x->_output.size(), x->_output.data());
        x->Output(obj);

    } catch (std::exception& e) {
        error("py.eval: %s", e.what());
    };
}

//
void* py_eval_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_eval* x = NULL;

    if ((x = (t_py_eval*)object_alloc((t_class*)py_eval_class))) {

        x->outlet_1 = outlet_new(x, NULL);
    }

    x->_list = gensym("list");

    if (argc)
        py_eval_message(x, NULL, argc, argv);

    return (x);
}

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

    maxpy_init();

    object_post((t_object*)py_eval_class, "py_eval: loaded");

    return 0;
}
}
