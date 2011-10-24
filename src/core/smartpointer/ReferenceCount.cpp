//
//  ReferenceCount.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 10/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ReferenceCount.h"
#include "RbException.h"


ReferenceCount::ReferenceCount(void) {
    
}

ReferenceCount::ReferenceCount(const ReferenceCount& rc) {
    
}

ReferenceCount& ReferenceCount::operator=(const ReferenceCount &) {
    
    return *this;
}

ReferenceCount::~ReferenceCount(void) {
    
}

size_t ReferenceCount::getNumberReferences(void *ptr) {
    size_t refs = refCountMap[ptr];
    return refs;
}

void ReferenceCount::decreaseReferenceCount(void *ptr) {
    if (refCountMap.find(ptr) == refCountMap.end()) {
		throw RbException("ERROR: Missing entry in reference count map!");
	}
    refCountMap[ptr]--;
}


void ReferenceCount::erase(void *ptr) {
    if (refCountMap.find(ptr) == refCountMap.end()) {
		throw RbException("ERROR: Missing entry in reference count map!");
	}
    refCountMap.erase(ptr);
}

void ReferenceCount::increaseReferenceCount(void *ptr) {
    
	if (refCountMap.find(ptr) == refCountMap.end()) {  
		refCountMap[ptr] = 1;
	} else {
		refCountMap[ptr]++;
	}
}