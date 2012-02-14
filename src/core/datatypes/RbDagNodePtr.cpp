
#include "DAGNode.h"
#include "RbDagNodePtr.h"
#include "RbException.h"

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
    
    incrementCountForAddress(mPtr);
}

void RbDagNodePtr::finalizePointer(void) {
    
    if ( decrementCountForAddress(mPtr) ) { 
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


size_t RbDagNodePtr::countForAddress(const DAGNode* qPtr) {
    
    std::map<const DAGNode*,size_t>& refCountMap = getRefCountMap();
    
    // check if we got the NULL pointer
    if (qPtr == 0) return -1;
    
    if ( refCountMap.find(qPtr) != refCountMap.end() ) {
        return refCountMap[qPtr];
    }
    
    return 0;
}


std::map<const DAGNode*, size_t>& RbDagNodePtr::getRefCountMap()
{
    static std::map<const DAGNode*,size_t> refCountMap;
    return refCountMap;
}


void RbDagNodePtr::incrementCountForAddress(const DAGNode* qPtr) {
    
    std::map<const DAGNode*,size_t>& refCountMap = getRefCountMap();
    
    // check if we got the NULL pointer
    if (qPtr == 0) return;
    
    if ( refCountMap.find(qPtr) == refCountMap.end() ) {
        refCountMap[qPtr] = 1;
    } 
    else {
        refCountMap[qPtr]++;
    }
}

bool RbDagNodePtr::decrementCountForAddress(const DAGNode* qPtr) {
    
    std::map<const DAGNode*,size_t>& refCountMap = getRefCountMap();
    
    // check if we got the NULL pointer
    if (qPtr == 0) return false;
    
    if ( refCountMap.find(qPtr) == refCountMap.end() )
    {
        RbException("Missing entry in memory map");
        return false;
    }
    
    refCountMap[qPtr]--;
    size_t currCount = refCountMap[qPtr];
    
    if ( currCount == 0 ) 
    {
        refCountMap.erase(qPtr);
        return true;
    }
    
    return false;
}


