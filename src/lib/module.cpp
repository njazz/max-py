#include <stdarg.h>

#include "compatibility.h"
#include "ext.h"
#include "ext_globalsymbol.h"
#include "ext_obex.h"

#include <pybind11/embed.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <memory>

#include "convert.hpp"

#include "e_object.hpp"

extern "C" {
PYBIND11_EMBEDDED_MODULE(maxpy, m)
{
    py::class_<EObject, EObjectPtr>(m, "object")
        .def("inlet_count", &EObject::InletCount)
        .def("outlet_count", &EObject::OutletCount)

        .def("class_name", &EObject::ClassName)
        .def("namespace", &EObject::Namespace)

        .def("is_none", &EObject::IsNone)

        .def("message", &EObject::Message)

        .def("method_names", &EObject::MethodNames)

        .def("parent_patch", &EObject::ParentPatch)
        ;

    m.def("post", [](std::string s) {
        post(s.c_str());
    });

    m.def("error", [](std::string s) {
        error(s.c_str());
    });

    m.def("new_object", [](std::string n) -> std::shared_ptr<EObject> {
        auto ret = std::make_shared<EObject>(n);
        return ret;
    });

    m.def("__print", [](py::object obj){
        std::vector<c_atom> dest;
        to_atoms(dest,obj);
        auto s = to_string(dest.size(),dest.data());
        post("max-py: %s", s.c_str());
    });
}
}
