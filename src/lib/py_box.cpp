#include "py_box.hpp"

void PyBox::Output(py::object obj)
{
    to_atoms(_output, obj);
    outlet_anything(outlet_1, _list, _output.size(), _output.data());
}
