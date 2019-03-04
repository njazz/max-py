#include "receive_dispatch.hpp"

void ReceiveDispatch::_AddBox(std::string sym, PyBox* out)
{
    if (_outletMap.find(sym) == _outletMap.end()) {
        _outletMap[sym] = {};
    }

    if (_outletMap[sym].size() == 0) {
        try {
            ReceiveDispatch::AttachMethod(sym);

        } catch (std::exception& e) {
            error(e.what());
            return;
        }
    }

    _outletMap[sym].push_back(out);
}

void ReceiveDispatch::_RemoveBox(std::string sym, PyBox* out)
{
    if (_outletMap.find(sym) == _outletMap.end())
        return;

    if (std::find(_outletMap[sym].begin(), _outletMap[sym].end(), out) == _outletMap[sym].end())
        return;

    _outletMap[sym].erase(std::find(_outletMap[sym].begin(), _outletMap[sym].end(), out));
}

void ReceiveDispatch::RegisterBox(std::string sym, PyBox* box)
{
    _AddBox(sym, box);
};

void ReceiveDispatch::UnregisterBox(std::string sym, PyBox* box)
{
    _RemoveBox(sym, box);

    if (_outletMap.find(sym) == _outletMap.end())
        return;
    if (_outletMap[sym].size() == 0)
        DetachMethod(sym);
};

void ReceiveDispatch::Output(std::string sym, py::object obj)
{
    for (auto b : _outletMap[sym]) {
        b->Output(obj);
    }
};

//
void ReceiveDispatch::AttachMethod(std::string sym)
{
    auto mod = py::eval("max");
    auto func = py::cpp_function([=](py::object lst) {

        ReceiveDispatch::Output(sym, lst);
    });

    if (mod.is_none()) {
        post("module error");
        return;
    }

    PyObject_SetAttr(mod.ptr(), py::str(sym).ptr(), func.ptr());
    post("py.receive: added max.%s()", sym.c_str());
}

void ReceiveDispatch::DetachMethod(std::string sym)
{
    auto mod = py::eval("max");
    if (mod.is_none()) {
        return;
    };
    PyObject_DelAttr(mod.ptr(), py::str(sym).ptr());
    post("py.receive: removed max.%s()", sym.c_str());
}
