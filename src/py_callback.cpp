// py.callback
// [py.callback func_name] adds func_name to embedded 'max' module

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
typedef struct _py_callback : public PyBox {
    std::string name = "";

} t_py_callback;

//1
void* py_callback_new(t_symbol* s, long argc, t_atom* argv);
void py_callback_free(t_py_callback* x);

void* py_callback_class;

void* py_callback_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_callback* x = NULL;

    if ((x = (t_py_callback*)object_alloc((t_class*)py_callback_class))) {
        x->outlet_1 = outlet_new(x, NULL);
    }

    if (argc != 1) {
        error("py.callback: name required");
        return 0;
    }

    if (!x)
        return 0;

    x->name = to_string(argv);

    // check if already exists:
    try {
        auto obj = py::eval("'" + x->name + "' in dir(max)");
        if (obj.cast<bool>()) {
            error("py.callback: name already used");
            return 0;
        }
    } catch (std::exception& e) {
        error(e.what());
        return 0;
    }

    x->_list = gensym("list");

    try {
        auto mod = py::eval("max");
        auto func = py::cpp_function([=](py::object lst) {

            x->Output(lst);

        });

        if (mod.is_none()) {
            post("module error");
            return 0;
        }

        PyObject_SetAttr(mod.ptr(), py::str(x->name).ptr(), func.ptr());
        post("py.callback: added max.%s()", x->name.c_str());

    } catch (std::exception& e) {
        error(e.what());
        return 0;
    }

    return (x);
}

//
void py_callback_free(t_py_callback* x)
{
    auto mod = py::eval("max");
    if (mod.is_none()) {
        return;
    };
    PyObject_DelAttr(mod.ptr(), py::str(x->name).ptr());
    post("py.callback: removed max.%s()", x->name.c_str());
}

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.callback", (method)py_callback_new, (method)py_callback_free, (long)sizeof(t_py_callback),
        0L, A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_callback_class = c;

    maxpy_init();

    return 0;
}
}
