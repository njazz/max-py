// py.exec

// anything:    run list (>1 elements) as python code
// set:         set code without output
// bang:        run previously set code

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

#include "py_box.hpp"
//
typedef struct _py_exec : public PyBox {
//    t_object ob;
//    void* outlet_1;

    std::string code = "";

} t_py_exec;

//
void* py_exec_new(t_symbol* s, long argc, t_atom* argv);
void py_exec_free(t_py_exec* x);
void py_exec_message(t_py_exec* x, t_symbol* s, long argc, t_atom* argv);

//
void* py_exec_class;

#pragma mark -

void py_exec_set(t_py_exec* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = to_string(argc, argv);
}

void py_exec_bang(t_py_exec* x);

void py_exec_message(t_py_exec* x, t_symbol* s, long argc, t_atom* argv)
{
    x->code = to_string(s, argc, argv);
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

        x->outlet_1 = outlet_new(x, NULL);
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

    maxpy_init();

    object_post((t_object*)py_exec_class, "py_exec: loaded");

    //    dyn_class_create("py.test_class");
    //    dyn_class_add_method("py.test_class", "tester");

    return 0;
}
}
