
#include "DAGNode.h"
#include "RbDagNodePtr.h"

#include <iostream>
#include <typeinfo>

RbDagNodePtr::RbDagNodePtr(DAGNode* inPtr) {
    
    initializePointer(inPtr);
    //    if (RbMemoryManager::rbMemoryManager().countForAddress(inPtr) == 1) {
    //        std::cerr << "Initialize pointer " << mPtr << " of type " << typeid(mPtr).name() << "|" << typeid(*mPtr).name() << std::endl;
    //    }
}

RbDagNodePtr::RbDagNodePtr(const RbDagNodePtr& src) {
    
    initializePointer(src.mPtr);
}

RbDagNodePtr::~RbDagNodePtr(void) {
    
    finalizePointer();
}

RbDagNodePtr& RbDagNodePtr::operator=(const RbDagNodePtr& rhs) {
    
    if (this == &rhs) 
    {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


void RbDagNodePtr::initializePointer(DAGNode* inPtr) {
    
    mPtr = inPtr;
    RbMemoryManager& myMemoryManager = getMemoryManager();
    myMemoryManager.incrementCountForAddress(mPtr);
}

void RbDagNodePtr::finalizePointer(void) {
    
    RbMemoryManager& myMemoryManager = getMemoryManager();
    
    if ( myMemoryManager.decrementCountForAddress(mPtr) ) { 
        delete mPtr;
//                std::cerr << "Destroying pointer " << mPtr << " of type " << typeid(mPtr).name() << std::endl;
    }
}

DAGNode* RbDagNodePtr::operator->(void) const {
    
    return (mPtr);
}

DAGNode& RbDagNodePtr::operator*(void) const {
    
    return (*mPtr);
}


