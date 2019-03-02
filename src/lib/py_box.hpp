#pragma once

#include <stdarg.h>
#include "compatibility.h"
#include "ext.h"

#include <pybind11/pybind11.h>

#include "c_atom.hpp"
#include "convert.hpp"

struct PyBox{
  t_object ob;
  void* outlet_1 = 0;
  t_symbol* _list = 0;
  std::vector<c_atom> _output;

  void Output(py::object obj);
};
