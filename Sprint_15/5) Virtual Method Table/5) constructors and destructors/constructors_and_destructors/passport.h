#pragma once

#include "identity_document.h"

#include <iostream>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>

using namespace std::string_view_literals;

class Passport
{
public:
    Passport()
        : parent()
        , expiration_date_(GetExpirationDate())
    {
        Passport::SetVTable(this);
        std::cout << "Passport::Ctor()"sv << std::endl;
    }


    Passport(const Passport& other)
        : parent(other.parent)
        , expiration_date_(other.expiration_date_)
    {
        Passport::SetVTable(const_cast<Passport*>(&other));
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    static void SetVTable(Passport* obj)
    {
        *(Passport::VTable**)obj = &Passport::vtable;
    }




    ~Passport()
    {
        std::cout << "Passport::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }





    static void PrintVisa(const Passport* obj, const std::string& country)
    {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << obj->GetID() << std::endl;
    }


    static void PrintID(const Passport* obj)
    {
        std::cout << "Passport::PrintID() : "sv << obj->GetID();
        std::cout << " expiration date : "sv << obj->expiration_date_.tm_mday << "/"sv << obj->expiration_date_.tm_mon << "/"sv
            << obj->expiration_date_.tm_year + 1900 << std::endl;
    }

    void PrintVisa(const std::string& country) const
    {
        PrintVisa(this, country);
    }


    void PrintID() const
    {
        PrintID(this);
    }


    static void Delete(Passport* obj)
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


    operator IdentityDocument()
    {
        return { parent };
    }




/*
    static void SetVTable(Passport* obj)
    {
        *(Passport::VTable**)obj = &Passport::vtable;
    }
    */
    struct VTable
    {
        using DeleteType = void (*)(Passport*);
        using PrintIDType = void (*)(const Passport*);
        using PrintVisaType = void (*)(const Passport*, const std::string&);

        DeleteType delete_func;
        PrintIDType print_id_func;
        PrintVisaType print_visa_func;
    };

private:
    IdentityDocument parent;
    static VTable vtable;

    const struct tm expiration_date_;

    tm GetExpirationDate()
    {
        time_t t = time(nullptr);
        tm exp_date = *localtime(&t);
        exp_date.tm_year += 10;
        mktime(&exp_date);
        return exp_date;
    }
};

Passport::VTable Passport::vtable = { Passport::Delete, Passport::PrintID, Passport::PrintVisa };
