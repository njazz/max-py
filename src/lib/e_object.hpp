#pragma once

#include "e_object.hpp"

#include <string>

#include "compatibility.h"
#include "ext.h"

#include <pybind11/pybind11.h>

#include <memory>
namespace py = pybind11;

class EObject;
typedef std::shared_ptr<EObject> EObjectPtr;

class EObject {
private:
        t_object* __ptr = 0;
public:
    EObject(std::string name);
    ~EObject();

    //
    static EObjectPtr None();

    //

    int InletCount();
    int OutletCount();

    bool IsNone();

    std::string ClassName();
    std::string Namespace();
    EObjectPtr ParentPatch();

    std::vector<std::string> MethodNames();

    void Message(std::string sel, py::object args);

};
