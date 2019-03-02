#include "e_object.hpp"
#include "c_atom.hpp"
#include "convert.hpp"

EObject::EObject(std::string name)
{

    if (name.size())
        __ptr = (t_object*)object_new(CLASS_BOX, gensym(name.c_str()));
}

EObject::~EObject()
{
    if (__ptr)
        object_free(__ptr);
}

EObjectPtr EObject::None()
{
    return std::make_shared<EObject>("");
}

//

int EObject::InletCount()
{
    if (!__ptr)
        return 0;
    return inlet_count(__ptr);
}

int EObject::OutletCount()
{
    if (!__ptr)
        return 0;
    return outlet_count(__ptr);
}

bool EObject::IsNone()
{
    return (!__ptr);
}

std::string EObject::ClassName()
{
    if (!__ptr)
        return "<None>";
    return object_classname(__ptr)->s_name;
}

std::string EObject::Namespace()
{
    if (!__ptr)
        return "<None>";
    return object_namespace(__ptr)->s_name;
}

EObjectPtr EObject::ParentPatch()
{
    t_object* obj;

    auto err = object_obex_lookup(__ptr, gensym("#P"), &obj);
    if (err != MAX_ERR_NONE)
        return EObject::None();

    auto ret = EObject::None();
    ret->__ptr = obj;
    return ret;
}

std::vector<std::string> EObject::MethodNames()
{
    if (!__ptr)
        return {};
    auto cl = object_class(__ptr);
    if (!cl)
        return {};
    auto msc = cl->c_messcount;

    std::vector<std::string> ret;
    for (int i = 1; i < msc; i++) {
        auto sym = cl->c_messlist[i].m_sym;
        if (sym) {
            ret.push_back(std::string(sym->s_name));
            //                post("%s", sym->s_name);
        }
    }

    return ret;
}

void EObject::Message(std::string sel, py::object args)
{
    if (!__ptr)
        return;

    std::vector<c_atom> dest;
    to_atoms(dest, args);

    typedmess(__ptr, gensym(sel.c_str()), dest.size(), dest.data());

}
