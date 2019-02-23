// py.exec

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
typedef struct _py_exec {
    t_object ob;
    void* out1;

    std::string code = "";

} t_py_exec;

//1
void* py_exec_new(t_symbol* s, long argc, t_atom* argv);
void py_exec_free(t_py_exec* x);

void py_exec_message(t_py_exec* x, t_symbol* s, long argc, t_atom* argv);

void* py_exec_class;

#pragma mark -

void py_exec_set(t_py_exec* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = to_string(argc,argv);
}

void py_exec_bang(t_py_exec* x);

void py_exec_message(t_py_exec* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = (s) ? (std::string(s->s_name) + " ") : "";
    x->code += to_string(argc,argv);

    py_exec_bang(x);
}

void py_exec_bang(t_py_exec* x)
{
    try {
        py::exec(x->code.c_str());
    } catch (std::exception& e) {
        error("py.exec: %s", e.what());
    };
}

//

void* py_exec_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_exec* x = NULL;

    if ((x = (t_py_exec*)object_alloc((t_class*)py_exec_class))) {

        x->out1 = outlet_new(x, NULL);
    }

    return (x);
}
//
void py_exec_free(t_py_exec* x)
{
    //    py::finalize_interpreter();
}

// ---

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.exec", (method)py_exec_new, (method)py_exec_free, (long)sizeof(t_py_exec),
        0L, A_GIMME, 0);

    class_addmethod(c, (method)py_exec_message, "anything", A_GIMME, 0);
    class_addmethod(c, (method)py_exec_set, "set", A_GIMME, 0);
    class_addmethod(c, (method)py_exec_bang, "bang", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_exec_class = c;

    init_interpreter();

    try {
        py::exec("import maxpy as max");
    } catch (std::exception& e) {
        error("py.exec: failed");
    }

    object_post((t_object*)py_exec_class, "py_exec: loaded");

    dyn_class_create("py.test_class");
    dyn_class_add_method("py.test_class", "tester");

    return 0;
}
}
