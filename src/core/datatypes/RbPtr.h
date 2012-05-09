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
 * @date Last modified: $Date: 2012-03-07 15:57:47 +0100 (Wed, 07 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-05-09, version 1.0
 *
 * $Id: RbPtr.h 1325 2012-03-07 14:57:47Z hoehna $
 */

#ifndef RbPtr_H
#define RbPtr_H

#include <string>

template <typename ptrType>
class RbPtr {
    
public:
    RbPtr(ptrType* inPtr = NULL);
    ~RbPtr(void);
    RbPtr(const RbPtr& src);
    RbPtr&                                      operator=(const RbPtr& rhs);
    ptrType&                                    operator*(void) const;
    ptrType*                                    operator->(void) const;
    operator                                    ptrType*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(ptrType* qPtr);
    static void                                 incrementCountForAddress(ptrType* qPtr);
    
private:
    size_t                                      countForAddress(const ptrType* qPtr);
    void                                        finalizePointer(void);
    void                                        initializePointer(ptrType* inPtr);
    
    ptrType*                                    mPtr;
};


#endif

