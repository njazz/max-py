#include "c_atom.hpp"

void c_atom::operator=(const double& v)
{
    a_type = A_FLOAT;
    a_w.w_float = v;
}

void c_atom::operator=(const long& v)
{
    a_type = A_LONG;
    a_w.w_long = v;
}

void c_atom::operator=(std::string v)
{
    a_type = A_SYM;
    a_w.w_sym = gensym(v.c_str());
//    printf("set %s\n",v.c_str());
//    printf("<< %s\n",a_w.w_sym->s_name);

}

void c_atom::operator=(t_symbol*s)
{
    a_type = A_SYM;
    a_w.w_sym = s;
}

void c_atom::operator=(py::handle obj)
{
    if (PyFloat_Check(obj.ptr()))
        return operator=(PyFloat_AsDouble(obj.ptr()));

    if (PyLong_Check(obj.ptr()))
        return operator=(PyLong_AsLong(obj.ptr()));

    if (py::isinstance<py::str>(obj))
        return operator=(obj.cast<std::string>());

    return operator=(py::str(obj).cast<std::string>());
}
