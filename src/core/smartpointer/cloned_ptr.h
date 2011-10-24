/*
 * cloned_ptr - clone-on-create/assign pointer.
 * Useful as a class member to get deep copy semantics.
 */

#ifndef CLONED_PTR_H
#define CLONED_PTR_H

/* For ANSI-challenged compilers, you may want to #define
 * NO_MEMBER_TEMPLATES or explicit */

#include "RbObject.h"

template <class X> class cloned_ptr
{
public:
    typedef X element_type;
    
    explicit cloned_ptr(X* p = 0) throw()       : itsPtr(p) {}
    ~cloned_ptr()                               {delete itsPtr;}
    cloned_ptr(const cloned_ptr& r)             {copy(r.get());}
    cloned_ptr& operator=(const cloned_ptr& r)
    {
        if (this != &r) {
            delete itsPtr;
            copy(r.get());
        }
        return *this;
    }
    
#ifndef NO_MEMBER_TEMPLATES
    template <class Y> cloned_ptr(const cloned_ptr<Y>& r)
    {copy(r.get());}
    template <class Y> cloned_ptr& operator=(const cloned_ptr<Y>& r)
    {
        if (this != &r) {
            delete itsPtr;
            copy(r.get());
        }
        return *this;
    }
#endif
    
    X& operator*()  const throw()               {return *itsPtr;}
    X* operator->() const throw()               {return itsPtr;}
    X* get()        const throw()               {return itsPtr;}
    
private:
    X* itsPtr;
    void copy(X* p)                              {itsPtr = p ? static_cast<X*>(static_cast<RbObject*>(p)->clone()) : 0;}
};

#endif // CLONED_PTR_H