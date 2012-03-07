/**
 * @file RbConstDagNodePtr.h
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
 * $Id: RbConstDagNodePtr.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef RbConstDagNodePtr_H
#define RbConstDagNodePtr_H

class DAGNode;

class RbConstDagNodePtr {
    
public:
    RbConstDagNodePtr(const DAGNode* inPtr = NULL);
    ~RbConstDagNodePtr(void);
    RbConstDagNodePtr(const RbConstDagNodePtr& src);
    RbConstDagNodePtr&                          operator=(const RbConstDagNodePtr& rhs);
    const DAGNode&                              operator*(void) const;
    const DAGNode*                              operator->(void) const;
    operator                                    const DAGNode*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(const DAGNode* qPtr);
    static void                                 incrementCountForAddress(const DAGNode* qPtr);
    
private:
    size_t                                      countForAddress(const DAGNode* qPtr);
    void                                        finalizePointer(void);
    void                                        initializePointer(const DAGNode* inPtr);
    
    const DAGNode*                              mPtr;
};


#endif

