#include <stdarg.h>

#include "compatibility.h"
#include "ext.h"
#include "ext_globalsymbol.h"
#include "ext_obex.h"

#include <pybind11/embed.h>
namespace py = pybind11;

extern "C" {
PYBIND11_EMBEDDED_MODULE(maxpy, m)
{
    m.def("post", [](std::string s) {
        post(s.c_str());
    });

    m.def("error", [](std::string s) {
        error(s.c_str());
    });

    m.def("object", [](std::string n) -> int {
        //        auto p = object_new(NULL, gensym(n.c_str()));
        auto cl = class_findbyname(CLASS_BOX, gensym(n.c_str()));
        if (!cl)
            return 0;

        post("class %s ok", n.c_str());

        auto p = object_new(CLASS_BOX, gensym(n.c_str())); //newobject(cl);
        // object_new_parse

        post("object: %i ", (((long)p) > 0));

        t_symbol** out;
        long c = 0;
        if (p) {
            post("inlets %i", inlet_count((t_object*)p));
            post("outlets %i", outlet_count((t_object*)p));

            object_attr_getnames(p, &c, &out);

            for (int i = 0; i < c; i++) {
                post("attribute: %s", out[i]->s_name);
            }
        }

        return (p != 0);
    });

}
}
