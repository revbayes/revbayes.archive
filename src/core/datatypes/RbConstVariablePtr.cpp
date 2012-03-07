
#include "Variable.h"
#include "RbConstVariablePtr.h"


RbConstVariablePtr::RbConstVariablePtr(const Variable* inPtr) {
    
    initializePointer(inPtr);
}

RbConstVariablePtr::RbConstVariablePtr(const RbConstVariablePtr& src) {
    
    initializePointer(src.mPtr);
}

RbConstVariablePtr::~RbConstVariablePtr(void) {
    
    finalizePointer();
}

RbConstVariablePtr& RbConstVariablePtr::operator=(const RbConstVariablePtr& rhs) {
    
    if (this == &rhs) 
    {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


void RbConstVariablePtr::initializePointer(const Variable* inPtr) {
    
    mPtr = inPtr;
    
    incrementCountForAddress(mPtr);
}

void RbConstVariablePtr::finalizePointer(void) {
    
    if ( decrementCountForAddress(mPtr) ) { 
        delete mPtr;
    }
}

const Variable* RbConstVariablePtr::operator->(void) const {
    
    return (mPtr);
}

const Variable& RbConstVariablePtr::operator*(void) const {
    
    return (*mPtr);
}


size_t RbConstVariablePtr::countForAddress(const Variable* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return -1;
    
    return qPtr->getReferenceCount();
}


void RbConstVariablePtr::incrementCountForAddress(const Variable* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return;
    
    // We know that this is a safe const cast (Sebastian)
    const_cast<Variable*>(qPtr)->incrementReferenceCount();
}

bool RbConstVariablePtr::decrementCountForAddress(const Variable* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return false;
    
    // We know that this is a safe const cast (Sebastian)
    size_t refCount = const_cast<Variable*>(qPtr)->decrementReferenceCount();
    
    return refCount == 0;
}


