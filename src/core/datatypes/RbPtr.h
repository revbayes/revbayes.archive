/**
 * @file RbPtr.h
 *
 * SuperSmartPointer:
 *
 * From Chapter 25 of Professional C++ 
 * by Nicholas A. Solter and Scott J. Kleper 
 * (Wrox, 2005) 
 *
 * @brief Declaration of ReferenceCount
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef RbPtr_H
#define RbPtr_H




#include <iostream>
#include <map>



template <typename T>
class RbPtr {

    public:
        explicit                    RbPtr(T* inPtr = NULL);
                                   ~RbPtr(void);
                                    RbPtr(const RbPtr<T>& src);
        RbPtr<T>&                   operator=(const RbPtr<T>& rhs);
        const T&                    operator*(void) const;
        const T*                    operator->(void) const;
        T&                          operator*(void);
        T*                          operator->(void);
        operator                    void*(void) const { return mPtr; }
        T*                          get(void) const { return mPtr; }

        static RbPtr<T>             getNullPtr(void)                                           //!< Get the null pointer
            {
            static RbPtr theNullPtr = RbPtr(NULL);
            return theNullPtr;
            }
        
    protected:
        void                        finalizePointer(void);
        void                        initializePointer(T* inPtr);
        T*                          mPtr;
};

#endif