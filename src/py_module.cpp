// py.module
// import:  load module
// receives [func_name args...] message

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
typedef struct _py_module : public PyBox {
//    t_object ob;
//    void* outlet_1;

    std::string module = "";

} t_py_module;

//1
void* py_module_new(t_symbol* s, long argc, t_atom* argv);
void py_module_free(t_py_module* x);

void* py_module_class;

#pragma mark -

void py_module_import(t_py_module* x, t_symbol* s, long argc, t_atom* argv)
{
    if (argc < 1)
        return;

    auto n = argv[0];
    if (n.a_type != A_SYM)
        return;

    x->module = n.a_w.w_sym->s_name;



        t_symbol* ret = gensym("none");

        path_absolutepath(&ret, gensym((x->module + ".py").c_str()), 0, 0);

//        if (!ret) {
//            // trying to import module in Python search path


//        }

        try{
        py::exec("import "+x->module);
        return;
        }catch(std::exception)
        {};

            try {
        short id;
        char fn[MAX_FILENAME_CHARS];
        path_frompathname(ret->s_name, &id, fn);
        char ret2[MAX_PATH_CHARS];
        path_toabsolutesystempath(id, fn, ret2);

        py::exec("import importlib.util\nspec = importlib.util.spec_from_file_location('" + x->module + "', '" + ret2 + "')\n" + x->module + " = importlib.util.module_from_spec(spec)\nspec.loader.exec_module(" + x->module + ")\n");

        post("py.module: loaded %s", x->module.c_str());

    } catch (std::exception& e) {
        error("py.module: failed to import module %s", x->module.c_str());
        error(e.what());
    };
}

void py_module_anything(t_py_module* x, t_symbol* s, long argc, t_atom* argv)
{
    std::string code = x->module+".";
    code += std::string(s->s_name) + "("; //(s) ? (std::string(s->s_name) + " ") : "";
    code += to_func_argument_string(argc, argv) + ")";

    try {
        auto obj = py::eval(code.c_str());
        //        post(py::str(obj).cast<std::string>().c_str());

        std::vector<c_atom> a;
        to_atoms(a, obj);
        outlet_anything(x->outlet_1, gensym("list"), a.size(), a.data());
    } catch (std::exception& e) {
        error("py.module: %s", e.what());
    };
}

void* py_module_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_module* x = NULL;

    if ((x = (t_py_module*)object_alloc((t_class*)py_module_class))) {
        x->outlet_1 = outlet_new(x, NULL);
    }

    if (argc == 1)
        py_module_import(x, NULL, 1, argv);

    return (x);
}

//
void py_module_free(t_py_module* x)
{
    //    py::finalize_interpreter();
}

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.module", (method)py_module_new, (method)py_module_free, (long)sizeof(t_py_module),
        0L, A_GIMME, 0);

    class_addmethod(c, (method)py_module_import, "import", A_GIMME, 0);

    class_addmethod(c, (method)py_module_anything, "anything", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_module_class = c;

    maxpy_init();

    object_post((t_object*)py_module_class, "py.module: loaded");

    return 0;
}
}
