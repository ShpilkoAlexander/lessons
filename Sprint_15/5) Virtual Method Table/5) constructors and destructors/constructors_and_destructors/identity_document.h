#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument
{
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_)
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }


    ~IdentityDocument()
    {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }


    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_)
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }


    IdentityDocument& operator=(const IdentityDocument&) = delete;





    static void PrintID(const IdentityDocument* obj)
    {
        std::cout << "IdentityDocument::PrintID() : "sv << obj->unique_id_ << std::endl;
    }


    void PrintID() const
    {
        auto ptr = static_cast<VTable*>(vptr);
        ptr->print_id_func(this);
    }

    static void Delete(IdentityDocument* obj)
    {
        delete obj;
    }


    void Delete()
    {
        auto ptr = static_cast<VTable*>(vptr);
        ptr->delete_func(this);
    }




    int GetID() const
    {
        return unique_id_;
    }

    static void PrintUniqueIDCount()
    {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }





    static void SetVTable(IdentityDocument* obj)
    {
        *(IdentityDocument::VTable**)obj = &IdentityDocument::vtable;
    }

    struct VTable
    {
        using DeleteType = void (*)(IdentityDocument*);
        using PrintIDType = void (*)(const IdentityDocument*);

        DeleteType delete_func;
        PrintIDType print_id_func;
    };

private:
    void* vptr = { &vtable };
    static VTable vtable;

    static int unique_id_count_;
    int unique_id_;
};

int IdentityDocument::unique_id_count_ = 0;

IdentityDocument::VTable IdentityDocument::vtable = { IdentityDocument::Delete, IdentityDocument::PrintID };
