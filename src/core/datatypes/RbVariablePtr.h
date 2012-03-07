/**
 * @file RbVariablePtr.h
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
 * @since 2011-08-19, version 1.0
 *
 * $Id$
 */

#ifndef RbVariablePtr_H
#define RbVariablePtr_H

class Variable;

class RbVariablePtr {
    
public:
    RbVariablePtr(Variable* inPtr = NULL);
    ~RbVariablePtr(void);
    RbVariablePtr(const RbVariablePtr& src);
    RbVariablePtr&                              operator=(const RbVariablePtr& rhs);
    Variable&                                   operator*(void) const;
    Variable*                                   operator->(void) const;
    operator                                    Variable*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(Variable* qPtr);
    static void                                 incrementCountForAddress(Variable* qPtr);
    
private:
    size_t                                      countForAddress(const Variable* qPtr);
    void                                        finalizePointer(void);
    void                                        initializePointer(Variable* inPtr);
    
    Variable*                                   mPtr;
};


#endif

