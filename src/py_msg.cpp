// TODO
// ----------

//// py.msg

//// anything:    evaluate list (>1 elements) as  arguments to python code
//// set:         set code
//// bang:        evaluate previously set code
//// arguments = set

//#include <stdarg.h>

//#include "compatibility.h"

//#include "ext.h"
//#include "ext_obex.h"

//#include <stdio.h>

//#include "stdlib.h"

//#include <pybind11/embed.h>
//#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
//#include <pybind11/stl_bind.h>

//namespace py = pybind11;

//#include <string>

//#include "convert.hpp"
//#include "dyn_class.hpp"

////
//#include <chrono>

//typedef struct _py_msg {
//    t_object ob;
//    void* outlet_1;

//    std::string code = "";
//    py::object out = py::object();

//    py::object compiled = py::none();

//    t_symbol* _list;

//    std::string id = "";

//    std::vector<c_atom> _output;

//    //    std::vector<t_atom> _out_list;

//    py::object in_message = py::none();

//} t_py_msg;

////
//void* py_msg_new(t_symbol* s, long argc, t_atom* argv);
//void py_msg_free(t_py_msg* x);

//void py_msg_message(t_py_msg* x, t_symbol* s, long argc, t_atom* argv);

////
//void* py_msg_class;

////

//void py_msg_bang(t_py_msg* x);

////
//void _py_msg_precompile(t_py_msg* x)
//{
//    try {
//        x->id = std::to_string((long)x);

//        py::exec(std::string("e_" + x->id + " = compile(\"\"\"" + x->code + "\"\"\",'<string>','eval')").c_str());

//        x->compiled = py::eval("lambda: eval(e_" + x->id + ")");

//    } catch (std::exception& e) {
//        error("py.msg: compile error %s", e.what());
//        x->compiled = py::eval("lambda: None");
//    };
//}

//void py_msg_set(t_py_msg* x, t_symbol* s, long argc, t_atom* argv)
//{
//    x->code = to_string(argc, argv);
//    _py_msg_precompile(x);
//}

//void py_msg_message(t_py_msg* x, t_symbol* s, long argc, t_atom* argv)
//{

//    // TODO:

//    x->in_message = to_object(s, argc, argv);

//    //    x->code = to_string(s, argc, argv);
//    //    _py_msg_precompile(x);

//    py_msg_bang(x);
//}

//void py_msg_bang(t_py_msg* x)
//{
//    auto st = std::chrono::high_resolution_clock::now();
//    try {

//        auto obj = x->compiled(); //py::eval("eval(e_"+x->id +")");
//        to_atoms(x->_output, obj);
//        outlet_anything(x->outlet_1, x->_list, x->_output.size(), x->_output.data());

//    } catch (std::exception& e) {
//        error("py.msg: %s", e.what());
//    };
//}

////
//void* py_msg_new(t_symbol* s, long argc, t_atom* argv)
//{
//    t_py_msg* x = NULL;

//    if ((x = (t_py_msg*)object_alloc((t_class*)py_msg_class))) {

//        x->outlet_1 = outlet_new(x, NULL);
//    }

//    x->_list = gensym("list");

//    if (argc)
//        py_msg_set(x, NULL, argc, argv);

//    return (x);
//}

//void py_msg_free(t_py_msg* x)
//{
//    //    py::finalize_interpreter();
//}

//// ---

//extern "C" {

//int C74_EXPORT main(void)
//{
//    t_class* c;

//    c = class_new("py.msg", (method)py_msg_new, (method)py_msg_free, (long)sizeof(t_py_msg),
//        0L, A_GIMME, 0);

//    class_addmethod(c, (method)py_msg_set, "set", A_GIMME, 0);
//    class_addmethod(c, (method)py_msg_message, "anything", A_GIMME, 0);
//    class_addmethod(c, (method)py_msg_bang, "bang", A_GIMME, 0);

//    class_register(CLASS_BOX, c);
//    py_msg_class = c;

//    maxpy_init();

//    object_post((t_object*)py_msg_class, "py_msg: loaded");

//    return 0;
//}
//}
