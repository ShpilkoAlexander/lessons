#pragma once

#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack
{
public:
    TravelPack()
        : parent()
        , identity_doc1_((Passport*) new Passport())
        , identity_doc2_((DrivingLicence*) new DrivingLicence())
    {
        SetVTable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : parent(other.parent)
        , identity_doc1_((Passport*) new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_)))
        , identity_doc2_((DrivingLicence*) new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_)))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)
    {

        SetVTable(const_cast<TravelPack*>(&other));
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }



    ~TravelPack()
    {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
    }




    static void PrintID(const TravelPack* obj)
    {
        obj->identity_doc1_->PrintID();
        obj->identity_doc2_->PrintID();
        obj->additional_pass_.PrintID();
        obj->additional_dr_licence_.PrintID();
    }

    void PrintID() const
    {
        PrintID(this);
    }

    static void Delete(TravelPack* obj)
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



    static void SetVTable(TravelPack* obj)
    {
        *(TravelPack::VTable**)obj = &TravelPack::vtable;
    }


private:
    struct VTable
    {
        using DeleteType = void (*)(TravelPack*);
        using PrintIDType = void (*)(const TravelPack*);

        DeleteType delete_func;
        PrintIDType print_id_func;
    };

private:
    IdentityDocument parent;
    static VTable vtable;

    Passport* identity_doc1_;
    DrivingLicence* identity_doc2_;

    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
};

TravelPack::VTable TravelPack::vtable = { TravelPack::Delete, TravelPack::PrintID };
