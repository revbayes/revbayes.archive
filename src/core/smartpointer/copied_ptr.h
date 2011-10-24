/*
 * copied_ptr - simple copy-on-create/assign pointer.
 *
 * Note: If the actual object pointed to belongs to a derived class,
 * then copied_ptr will not create a copy of the derived class object,
 * but a new base class object.
 * If you want to use a polymorphic copy-on-assign pointer, use
 * cloned_ptr.
 */

#ifndef COPIED_PTR_H
#define COPIED_PTR_H

/* For ANSI-challenged compilers, you may want to #define explicit */

template <class X> class copied_ptr
{
public:
    typedef X element_type;
    
    explicit copied_ptr(X* p = 0) throw()       : itsPtr(p) {}
    ~copied_ptr()                               {delete itsPtr;}
    copied_ptr(const copied_ptr& r)             {copy(r);}
    copied_ptr& operator=(const copied_ptr& r)
    {
        if (this != &r) {
            delete itsPtr;
            copy(r);
        }
        return *this;
    }
    
    X& operator*()  const throw()               {return *itsPtr;}
    X* operator->() const throw()               {return itsPtr;}
    X* get()        const throw()               {return itsPtr;}
    
private:
    X* itsPtr;
    void copy(const copied_ptr& r)  {itsPtr = r.itsPtr ? new X(*r.itsPtr) : 0;}
};

#endif // COPIED_PTR_H