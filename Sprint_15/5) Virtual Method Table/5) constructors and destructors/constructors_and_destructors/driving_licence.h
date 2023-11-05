#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence
{
public:
    DrivingLicence()
        : parent()
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }


    DrivingLicence(const DrivingLicence& other)
        : parent(other.parent)
    {
        DrivingLicence::SetVTable(&other);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }


    ~DrivingLicence()
    {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }



    static void PrintID(const DrivingLicence* obj)
    {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }


    void PrintID() const
    {
        PrintID(this);
    }


    static void Delete(DrivingLicence* obj)
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

    operator IdentityDocument() { return { parent }; }





    static void SetVTable(const DrivingLicence* obj)
    {
        *(DrivingLicence::VTable**)obj = &DrivingLicence::vtable;
    }

    struct VTable
    {
        using DeleteType = void (*)(DrivingLicence*);
        using PrintIDType = void (*)(const DrivingLicence*);

        DeleteType delete_func;
        PrintIDType print_id_func;
    };

private:
    IdentityDocument parent;
    static VTable vtable;
};

DrivingLicence::VTable DrivingLicence::vtable = { DrivingLicence::Delete, DrivingLicence::PrintID };
