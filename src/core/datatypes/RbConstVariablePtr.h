/**
 * @file RbConstVariablePtr.h
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
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-19, version 1.0
 *
 * $Id: RbConstVariablePtr.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef RbConstVariablePtr_H
#define RbConstVariablePtr_H

#include <string>

class Variable;

class RbConstVariablePtr {
    
public:
    RbConstVariablePtr(const Variable* inPtr = NULL);
    ~RbConstVariablePtr(void);
    RbConstVariablePtr(const RbConstVariablePtr& src);
    RbConstVariablePtr&                         operator=(const RbConstVariablePtr& rhs);
    const Variable&                             operator*(void) const;
    const Variable*                             operator->(void) const;
    operator                                    const Variable*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(const Variable* qPtr);
    static void                                 incrementCountForAddress(const Variable* qPtr);
    
private:
    size_t                                      countForAddress(const Variable* qPtr);
    void                                        finalizePointer(void);
    void                                        initializePointer(const Variable* inPtr);
    
    const Variable*                                   mPtr;
};


#endif

