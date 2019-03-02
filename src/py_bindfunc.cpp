// py.bindfunc
// TODO
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
typedef struct _py_bindfunc {
    t_object ob;
    void* outlet_1;

    std::string module = "";
    std::string func = "";

    std::string symbol = "";

    t_symbol* x_s = 0;

    t_object* _sender = 0;

} t_py_bindfunc;

//1
void* py_bindfunc_new(t_symbol* s, long argc, t_atom* argv);
void py_bindfunc_free(t_py_bindfunc* x);

void* py_bindfunc_class;

#pragma mark -

void py_bindfunc_import(t_py_bindfunc* x, t_symbol* s, long argc, t_atom* argv)
{
    if (argc < 2)
        return;

    if (argc > 3)
        return;

    post("%i", argc);

    if (argc == 2) {
        if (argv[0].a_type != A_SYM)
            return;
        if (argv[1].a_type != A_SYM)
            return;

        x->module = "";
        x->func = argv[0].a_w.w_sym->s_name;
        x->symbol = argv[1].a_w.w_sym->s_name;

        x->x_s = gensym(x->symbol.c_str());

    } else {
        if (argv[0].a_type != A_SYM)
            return;
        if (argv[1].a_type != A_SYM)
            return;
        if (argv[2].a_type != A_SYM)
            return;

        x->module = argv[0].a_w.w_sym->s_name;
        x->func = argv[1].a_w.w_sym->s_name;
        x->symbol = argv[2].a_w.w_sym->s_name;

        x->x_s = gensym(x->symbol.c_str());

        try {

            t_symbol* ret = NULL;
            gensym("none");

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

            post("py.func: loaded %s", x->module.c_str());

            //            x->x_s->s_thing = (t_object*)x;

            post("bind '%s'", x->x_s->s_name);
            post("thing s %lu", (long)x->x_s->s_thing);

            //            t_atom* a1 = new t_atom();
            //            atom_setsym(a1,x->x_s);
            //            auto new_t = object_new(CLASS_NOBOX,gensym("through"),1,a1);
            //            if (new_t)
            //            {
            //                auto err = globalsymbol_bind((t_object*)x, x->x_s->s_name, 0);
            //                post("new bind err %i", err);
            //            }

            //

            auto err = globalsymbol_bind((t_object*)x, x->x_s->s_name, 0);
            post("bind err %i", err);

            post("thing d %lu", (long)x->x_s->s_thing);
            post("%lu", (long)x);

            if (x->x_s->s_thing) {
                auto cn = object_classname(x->x_s->s_thing);
                post("cls %s", cn->s_name);

                auto cl = object_class(x->x_s->s_thing);

                auto ns = class_namespace(cl);
                post("ns %s", ns->s_name);

                auto msc = cl->c_messcount;

                //                std::vector<std::string> ret;
                for (int i = 1; i < msc; i++) {
                    auto sym = cl->c_messlist[i].m_sym;
                    if (sym) {
                        //                        ret.push_back(std::string(sym->s_name));
                        post("%s", sym->s_name);
                    }
                }

                post("ports %i %i", inlet_count(x->x_s->s_thing), outlet_count(x->x_s->s_thing));

                typedmess(x->x_s->s_thing, gensym("test"), 0, 0);

                //                object_attach_byptr(x, x->x_s->s_thing);
            }

            auto s__ = (t_object*)globalsymbol_reference((t_object*)x, x->x_s->s_name, "through");

            // If we were previously referencing another buffer, we should not longer reference it.
            //            globalsymbol_dereference((t_object *)x, x->x_s->s_name, "send");

            x->_sender = s__;
            post("sender: %lu", (long)x->_sender);

            //            obj

        } catch (std::exception& e) {
            error("py.func: failed to import module %s:", x->module.c_str());
            error(e.what());
        };
    }
}

void py_bindfunc_anything(t_py_bindfunc* x, t_symbol* s, long argc, t_atom* argv)
{
    std::string code = (x->module == "") ? "" : x->module + ".";
    code += x->func + "(";
    code += to_func_argument_string(s, argc, argv) + ")";

    try {
        post(code.c_str());
        auto obj = py::eval(code.c_str());

        // TODO
        std::vector<c_atom> a;
        to_atoms(a, obj);

        outlet_anything(x->outlet_1, gensym("list"), a.size(), a.data());
    } catch (std::exception& e) {
        error("py.func: %s", e.what());
    };
}

void* py_bindfunc_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_bindfunc* x = NULL;

    if ((x = (t_py_bindfunc*)object_alloc((t_class*)py_bindfunc_class))) {
        x->outlet_1 = outlet_new(x, NULL);
    }

    object_register(CLASS_BOX, gensym("py.bindfunc"), (t_object*)x);

    if (argc > 1)
        py_bindfunc_import(x, NULL, argc, argv);

    return (x);
}

//
void py_bindfunc_free(t_py_bindfunc* x)
{
    //    py::finalize_interpreter();
    globalsymbol_unbind((t_object*)x, x->symbol.c_str(), 0);
}

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.bindfunc", (method)py_bindfunc_new, (method)py_bindfunc_free, (long)sizeof(t_py_bindfunc),
        0L, A_GIMME, 0);

    //    class_addmethod(c, (method)py_bindfunc_import, "import", A_GIMME, 0);

    class_addmethod(c, (method)py_bindfunc_anything, "anything", A_GIMME, 0);
    //    class_addmethod(c, (method)py_bindfunc_anything, "notify", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_bindfunc_class = c;

    maxpy_init();

    object_post((t_object*)py_bindfunc_class, "py.bindfunc: loaded");

    return 0;
}
}
