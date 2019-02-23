#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "compatibility.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_globalsymbol.h"


#include <vector>
#include <array>
#include <list>

extern "C" {
extern void init_interpreter();
}

struct c_atom : public t_atom
{
    void operator=(const double& v)
    {
        a_type = A_FLOAT;
        a_w.w_float = v;
    }

    void operator=(const long& v)
    {
        a_type = A_LONG;
        a_w.w_long = v;
    }

    void operator=(std::string v)
    {
        a_type = A_SYM;
        a_w.w_sym = gensym(v.c_str());
    }

    void operator=(py::handle obj)
    {
        if (PyFloat_Check(obj.ptr()))
            return operator=(PyFloat_AsDouble(obj.ptr()));

        if (PyLong_Check(obj.ptr()))
            return operator=(PyLong_AsLong(obj.ptr()));

        if (py::isinstance<py::str>(obj))
            return operator=(obj.cast<std::string>());

        return operator=(py::str(obj).cast<std::string>());
    }
};

//extern std::vector<c_atom> to_atoms(const py::object& obj);

extern void to_atoms(std::vector<c_atom>& dest, const pybind11::handle &obj);

extern py::object to_object(long argc, t_atom *argv);
extern py::object to_object(t_symbol* s, long argc, t_atom *argv);

extern std::string to_string(long argc, t_atom *argv);
extern std::string to_string(t_atom *argv);
extern std::string to_func_argument_string(long argc, t_atom *argv);
extern std::string to_func_argument_string(t_symbol* s, long argc, t_atom *argv);
