
#include "Variable.h"
#include "RbVariablePtr.h"
#include "RbException.h"

#include <iostream>
#include <typeinfo>

RbVariablePtr::RbVariablePtr(Variable* inPtr) {
    
    initializePointer(inPtr);
}

RbVariablePtr::RbVariablePtr(const RbVariablePtr& src) {
    
    initializePointer(src.mPtr);
}

RbVariablePtr::~RbVariablePtr(void) {
    
    finalizePointer();
}

RbVariablePtr& RbVariablePtr::operator=(const RbVariablePtr& rhs) {
    
    if (this == &rhs) 
    {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


void RbVariablePtr::initializePointer(Variable* inPtr) {
    
    mPtr = inPtr;
    
    incrementCountForAddress(mPtr);
}

void RbVariablePtr::finalizePointer(void) {
    
    if ( decrementCountForAddress(mPtr) ) { 
        delete mPtr;
    }
}

Variable* RbVariablePtr::operator->(void) const {
    
    return (mPtr);
}

Variable& RbVariablePtr::operator*(void) const {
    
    return (*mPtr);
}


size_t RbVariablePtr::countForAddress(const Variable* qPtr) {
    
    
    // check if we got the NULL pointer
    if (qPtr == 0) return -1;
    
    return qPtr->getReferenceCount();
}


//std::map<const Variable*, size_t>& RbVariablePtr::getRefCountMap()
//{
//    static std::map<const Variable*,size_t> refCountMap;
//    return refCountMap;
//}


void RbVariablePtr::incrementCountForAddress(Variable* qPtr) {
    
    //std::map<const Variable*,size_t>& refCountMap = getRefCountMap();
    
    // check if we got the NULL pointer
    if (qPtr == 0) return;
    
    qPtr->incrementReferenceCount();
    //    if ( refCountMap.find(qPtr) == refCountMap.end() ) {
    //        refCountMap[qPtr] = 1;
    //    } 
    //    else {
    //        refCountMap[qPtr]++;
    //    }
}

bool RbVariablePtr::decrementCountForAddress(Variable* qPtr) {
    
    //    std::map<const Variable*,size_t>& refCountMap = getRefCountMap();
    
    // check if we got the NULL pointer
    if (qPtr == 0) return false;
    
    size_t refCount = qPtr->decrementReferenceCount();
    
    return refCount == 0;
    
    //    if ( refCountMap.find(qPtr) == refCountMap.end() )
    //    {
    //        RbException("Missing entry in memory map");
    //        return false;
    //    }
    //    
    //    refCountMap[qPtr]--;
    //    size_t currCount = refCountMap[qPtr];
    //    
    //    if ( currCount == 0 ) 
    //    {
    //        refCountMap.erase(qPtr);
    //        return true;
    //    }
    //    
    //    return false;
}


