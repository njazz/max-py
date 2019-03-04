#include <stdarg.h>
#include "compatibility.h"
#include "ext.h"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

#include <functional>
#include <map>
#include <string>

#include "convert.hpp"
#include "py_box.hpp"

// dispatch
class ReceiveDispatch {
    static std::map<std::string, std::vector<PyBox*> > _outletMap;
    static void _AddBox(std::string sym, PyBox* out);
    static void _RemoveBox(std::string sym, PyBox* out);

public:
    static void RegisterBox(std::string sym, PyBox* box);
    static void UnregisterBox(std::string sym, PyBox* box);

    static void Output(std::string sym, py::object obj);

    //
    static void AttachMethod(std::string sym);
    static void DetachMethod(std::string sym);
};
