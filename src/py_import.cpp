// py.import

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
typedef struct _py_import {
    t_object ob;
    void* out1;
} t_py_import;

//1
void* py_import_new(t_symbol* s, long argc, t_atom* argv);
void py_import_free(t_py_import* x);

void* py_import_class;

void py_import_load(t_py_import* x, t_symbol* s, long argc, t_atom* argv)
{
    if (argc < 1)
        return;
    auto n = argv[0];
    if (n.a_type != A_SYM)
        return;

    std::string mod_str = n.a_w.w_sym->s_name;

    try {
        py::exec("import " + mod_str);
        post("py.module: loaded %s", mod_str.c_str());
    } catch (std::exception& e) {
        error("py.module: failed to import module %s", mod_str.c_str());
    };

    auto result = py::eval("[f for f in dir(" + mod_str + ") if not f.startswith(\"_\")]");

    std::vector<std::string> res_vec(py::len(result));
    res_vec.clear();
    for (auto& el : result)
        res_vec.push_back(el.cast<std::string>());

    for (auto e : res_vec) {
        post("object: %s", e.c_str());
    }

}

void* py_import_new(t_symbol* s, long argc, t_atom* argv)
{

    t_py_import* x = NULL;

    if (argc != 1) {
        error("Argument missing: module name");
        return NULL;
    }

    if ((x = (t_py_import*)object_alloc((t_class*)py_import_class))) {

        x->out1 = outlet_new(x, NULL);
    }

    py_import_load(x, NULL, 1, argv);

    dyn_class_create("py.import.test");
    dyn_class_add_method("py.import.test", "tester");

    return (x);
}
//
void py_import_free(t_py_import* x)
{
    //    py::finalize_interpreter();
}

// ---

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.import", (method)py_import_new, (method)py_import_free, (long)sizeof(t_py_import),
        0L, A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_import_class = c;

    init_interpreter();

    object_post((t_object*)py_import_class, "py.import: loaded");

    return 0;
}
}
