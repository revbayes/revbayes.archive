//
//  RbMemoryManager.cpp
//  RevBayesCore
//
//  Created by John Huelsenbeck on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "RbException.h"
#include "RbMemoryManager.h"


size_t RbMemoryManager::countForAddress(void* qPtr) {

    if ( refCountMap.find(qPtr) != refCountMap.end() ) {
        return refCountMap[qPtr];
    }
    
    return 0;
}

void RbMemoryManager::incrementCountForAddress(void* qPtr) {

    if ( refCountMap.find(qPtr) == refCountMap.end() ) {
        refCountMap[qPtr] = 1;
    } 
    else {
        refCountMap[qPtr]++;
    }
}

bool RbMemoryManager::decrementCountForAddress(void* qPtr) {

    if ( refCountMap.find(qPtr) == refCountMap.end() )
        {
        RbException("Missing entry in memory map");
        return false;
    }
    
    refCountMap[qPtr]--;
    
    if ( refCountMap[qPtr] == 0 ) {
        refCountMap.erase(qPtr);
        return true;
    }
      
    return false;
}


size_t RbMemoryManager::numberOfReferences(void) const {
    size_t refs = 0;
    
    for (std::map<void*, size_t>::const_iterator it = refCountMap.begin(); it != refCountMap.end(); it++) {
        refs += it->second;
    }
    
    return refs;
}


size_t RbMemoryManager::numberOfRegisteredObjects(void) const {
    return refCountMap.size();
}
