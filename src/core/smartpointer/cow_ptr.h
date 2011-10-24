/*
 * cow_ptr - simple copy-on-write pointer.
 *
 * Implemented with reference counting (before write).
 */

#ifndef COW_PTR_H
#define COW_PTR_H

/* For ANSI-challenged compilers, you may want to #define
 * NO_MEMBER_TEMPLATES or explicit */

#include "counted_ptr.h"

template <class X> class cow_ptr
{
public:
    typedef X element_type;
    
    explicit cow_ptr(X* p = 0) throw()            : itsPtr(p) {}
    // no need for ~cow_ptr - the counted_ptr takes care of everything.
    cow_ptr(const cow_ptr& r) throw()             : itsPtr(r.itsPtr) {}
    cow_ptr& operator=(const cow_ptr& r)
    {
        if (this != &r)
            itsPtr = r.itsPtr;
        return *this;
    }
    
#ifndef NO_MEMBER_TEMPLATES
    template <class Y> friend class cow_ptr<Y>;
    template <class Y> cow_ptr(const cow_ptr<Y>& r) throw()
    : itsPtr(r.itsPtr) {}
    template <class Y> cow_ptr& operator=(const cow_ptr<Y>& r)
    {
        if (this != &r)
            itsPtr = r.itsPtr;
        return *this;
    }
#endif
    
    const X& operator*()    const throw()   {return *itsPtr;}
    const X* operator->()   const throw()   {return itsPtr.get();}
    const X* get()          const throw()   {return itsPtr.get();}
    
    X& operator*()                          {copy(); return *itsPtr;}
    X* operator->()                         {copy(); return itsPtr.get();}
    X* get()                                {copy(); return itsPtr.get();}
private:
    counted_ptr<X> itsPtr;
    void copy()                            // create a new copy of itsPtr
    {
        if (!itsPtr.unique()) {
            X* old_p = itsPtr.get();
            itsPtr = counted_ptr<X>(new X(*old_p));
        }
    }
};

#endif // COW_PTR_H