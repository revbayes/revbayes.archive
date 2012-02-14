/**
 * @file RbDagNodePtr.h
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

#ifndef RbDagNodePtr_H
#define RbDagNodePtr_H

#include "RbMemoryManager.h"

class DAGNode;

//#include <map>
//#include <string>

class RbDagNodePtr {
    
public:
    RbDagNodePtr(DAGNode* inPtr = NULL);
    ~RbDagNodePtr(void);
    RbDagNodePtr(const RbDagNodePtr& src);
    RbDagNodePtr&                               operator=(const RbDagNodePtr& rhs);
    DAGNode&                                    operator*(void) const;
    DAGNode*                                    operator->(void) const;
    operator                                    DAGNode*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(const DAGNode* qPtr);
    static std::map<const DAGNode*,size_t>&     getRefCountMap(void);                                           //!< Get the memory manager
    static void                                 incrementCountForAddress(const DAGNode* qPtr);
    
private:
    size_t                                      countForAddress(const DAGNode* qPtr);
    void                                        finalizePointer(void);


    
    void                        initializePointer(DAGNode* inPtr);
    DAGNode*                    mPtr;
};


#endif

