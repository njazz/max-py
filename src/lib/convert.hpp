#pragma once

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "compatibility.h"
#include "ext.h"
#include "ext_globalsymbol.h"
#include "ext_obex.h"

#include <array>
#include <list>
#include <vector>

#include "c_atom.hpp"

extern "C" {
extern void maxpy_init();
}
extern void maxpy_import_module(std::string& module);

//
extern void to_atoms(std::vector<c_atom>& dest, const pybind11::object& obj);
template <typename T>
extern void to_atoms(std::vector<c_atom> &dest, const std::vector<T> &vec)
{
    dest.resize(vec.size());

    auto it = vec.begin();
    for (auto& e : dest ) {
        e = *it;
        it++;
    }
}

extern py::object to_object(long argc, t_atom* argv);
extern py::object to_object(t_symbol* s, long argc, t_atom* argv);

extern std::string to_string(t_symbol* s, long argc, t_atom* argv);
extern std::string to_string(long argc, t_atom* argv);
extern std::string to_string(t_atom* argv);

extern std::string to_func_argument_string(long argc, t_atom* argv);
extern std::string to_func_argument_string(t_symbol* s, long argc, t_atom* argv);

//
extern bool check_arguments(long argc, t_atom* argv, const std::vector<e_max_atomtypes>& types);
