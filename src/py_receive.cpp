// py.receive
// [py.receive func_name] adds func_name to embedded 'max' module
// this works the same way as [py.callback] but allo

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

#include <functional>
#include <map>
#include <string>

//#include "dyn_class.hpp"

#include "convert.hpp"

#include "py_box.hpp"

#include "receive_dispatch.hpp"

std::map<std::string, std::vector<PyBox*> > ReceiveDispatch::_outletMap;
//
typedef struct _py_receive : public PyBox {
    std::string name = "";

} t_py_receive;

//1
void* py_receive_new(t_symbol* s, long argc, t_atom* argv);
void py_receive_free(t_py_receive* x);

void* py_receive_class;

void* py_receive_new(t_symbol* s, long argc, t_atom* argv)
{
    t_py_receive* x = NULL;

    if ((x = (t_py_receive*)object_alloc((t_class*)py_receive_class))) {
        x->outlet_1 = outlet_new(x, NULL);
    }

    if (argc != 1) {
        error("py.receive: name required");
        return 0;
    }

    if (!x)
        return 0;

    x->name = to_string(argv);

    ReceiveDispatch::RegisterBox(x->name, x);

    x->_list = gensym("list");

    return (x);
}

//
void py_receive_free(t_py_receive* x)
{
    ReceiveDispatch::UnregisterBox(x->name, x);
}

extern "C" {

int C74_EXPORT main(void)
{
    t_class* c;

    c = class_new("py.receive", (method)py_receive_new, (method)py_receive_free, (long)sizeof(t_py_receive),
        0L, A_GIMME, 0);

    class_register(CLASS_BOX, c);
    py_receive_class = c;

    maxpy_init();

    return 0;
}
}
