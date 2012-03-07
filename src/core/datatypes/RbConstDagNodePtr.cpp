
#include "DAGNode.h"
#include "RbConstDagNodePtr.h"

RbConstDagNodePtr::RbConstDagNodePtr(const DAGNode* inPtr) {
    
    initializePointer(inPtr);
}

RbConstDagNodePtr::RbConstDagNodePtr(const RbConstDagNodePtr& src) {
    
    initializePointer(src.mPtr);
}

RbConstDagNodePtr::~RbConstDagNodePtr(void) {
    
    finalizePointer();
}

RbConstDagNodePtr& RbConstDagNodePtr::operator=(const RbConstDagNodePtr& rhs) {
    
    if (this == &rhs) 
    {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


void RbConstDagNodePtr::initializePointer(const DAGNode* inPtr) {
    
    mPtr = inPtr;
    
    incrementCountForAddress(mPtr);
}

void RbConstDagNodePtr::finalizePointer(void) {
    
    if ( decrementCountForAddress(mPtr) ) { 
        delete mPtr;
    }
}

const DAGNode* RbConstDagNodePtr::operator->(void) const {
    
    return (mPtr);
}

const DAGNode& RbConstDagNodePtr::operator*(void) const {
    
    return (*mPtr);
}


size_t RbConstDagNodePtr::countForAddress(const DAGNode* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return -1;
    
    return qPtr->getReferenceCount();
}


void RbConstDagNodePtr::incrementCountForAddress(const DAGNode* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return;
    
    // We know that this is a safe const cast (Sebastian)
    const_cast<DAGNode*>(qPtr)->incrementReferenceCount();
}

bool RbConstDagNodePtr::decrementCountForAddress(const DAGNode* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return false;
    
    // We know that this is a safe const cast (Sebastian)
    size_t refCount = const_cast<DAGNode*>(qPtr)->decrementReferenceCount();
    
    return refCount == 0;
}


