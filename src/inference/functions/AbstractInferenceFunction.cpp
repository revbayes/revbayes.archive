//
//  AbstractInferenceFunction.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/19/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AbstractInferenceFunction.h"


/** Execute function */
void AbstractInferenceFunction::execute( void ) {
    
    // set all the offset and the level to 0 and delegate the function call
    std::vector<size_t> offsets;
    for (size_t i = 0; i < members.size(); ++i) {
        offsets.push_back( 0 );
    }
    
    // delegate the function execution
    execute(offsets,0);
}


void AbstractInferenceFunction::execute(std::vector<size_t> &offset, size_t level) {
    
    // first, we test if we have another dimension
    if ( members[members.size()-1].lengths.size() == level ) {
        // no, we execute the actual function here
        executeSimple(offset);
    }
    else {
        // next, we compute the number of elements we need to compute
        std::vector<size_t> elements;
        for (size_t i = 0; i < members.size(); ++i) {
            size_t element = 1;
            for (size_t j = level+1; j < members[i].lengths.size(); ++j) {
                element *= members[i].lengths[j];
            }
            elements.push_back( element );
        }
        
        // now, we iterate over the elements in this dimension
        for (size_t i = 0; i < members[members.size()-1].lengths[level]; ++i) {
            // we recompute the offset
            std::vector<size_t> nested_offsets;
            for (size_t j = 0; j < members.size(); ++j) {
                size_t nested_offset = offset[j];
                // test whether we have ele
                if ( level < members[j].lengths.size() ) {
                    nested_offset += elements[j] * i;
                }
                nested_offsets.push_back( nested_offset );
            }

            // and call the function for one level deeper
            execute(nested_offsets, level+1);
        }
    }
}

/** We catch here the setting of the argument variables to store our parameters. */
void AbstractInferenceFunction::setArguments(const std::vector<RbValue<void*> > &args) {
    
    members = args;
    
    // delegate to derived class' implementation
    setInternalArguments( args );
}


