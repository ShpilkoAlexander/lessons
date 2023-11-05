#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence
{
public:
    InternationalDrivingLicence()
        : parent()
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : parent(other.parent)
    {
        InternationalDrivingLicence::SetVTable(const_cast<InternationalDrivingLicence*>(&other));
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence()
    {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
        DrivingLicence::SetVTable((DrivingLicence*)this);
    }





    static void PrintID(const InternationalDrivingLicence* obj)
    {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }

    void PrintID() const
    {
        PrintID(this);
    }

    static void Delete(InternationalDrivingLicence* obj)
    {
        delete obj;
    }

    void Delete()
    {
        Delete(this);
    }





    int GetID() const
    {
        return parent.GetID();
    }

    static void PrintUniqueIDCount()
    {
        IdentityDocument::PrintUniqueIDCount();
    }


    operator DrivingLicence() { return { parent }; }




    static void SetVTable(InternationalDrivingLicence* obj)
    {
        *(InternationalDrivingLicence::VTable**)obj = &InternationalDrivingLicence::vtable;
    }

    struct VTable
    {
        using DeleteType = void (*)(InternationalDrivingLicence*);
        using PrintIDType = void (*)(const InternationalDrivingLicence*);

        DeleteType delete_func;
        PrintIDType print_id_func;
    };

private:
    DrivingLicence parent;
    static VTable vtable;
};

InternationalDrivingLicence::VTable InternationalDrivingLicence::vtable =
{ InternationalDrivingLicence::Delete, InternationalDrivingLicence::PrintID };
