#pragma once

#include <stdarg.h>

#include "compatibility.h"
#include "ext.h"

#include <string>

#include <pybind11/pybind11.h>
namespace py = pybind11;

struct c_atom : public t_atom
{
    c_atom() : t_atom(){};

    c_atom(const c_atom& src)
    {
        a_w = src.a_w;
        a_type = src.a_type;
    }

    void operator=(const double& v);
    void operator=(const long& v);
    void operator=(std::string v);
    void operator=(t_symbol*s);

    void operator=(py::handle obj);

    bool operator==(c_atom& c) const
    {
        if (a_type != c.a_type) return false;

        if (a_type == A_FLOAT)
            return a_w.w_float == c.a_w.w_float;

        if (a_type == A_LONG)
            return a_w.w_long == c.a_w.w_long;

        if (a_type == A_SYM)
            return a_w.w_sym == c.a_w.w_sym;

        return false;

    }
    bool operator==(float v) const
    {
        if (a_type != A_FLOAT) return false;
            return a_w.w_float == v;
    }

    bool operator==(long v) const
    {
        if (a_type != A_LONG) return false;
            return a_w.w_long == v;
    }

    bool operator==(std::string s) const
    {
        if (a_type != A_SYM) return false;
            return a_w.w_sym == gensym(s.c_str());
    }

    bool operator==(t_symbol* s) const
    {
        if (a_type != A_SYM) return false;
            return a_w.w_sym == s;
    }
};
