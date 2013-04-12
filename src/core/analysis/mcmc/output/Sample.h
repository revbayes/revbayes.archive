/**
 * @file
 * This file contains the declaration of the Sample class. 
 * A sample represents an mcmc sample with a frequency (how often the value was sampled)
 * and the value.
 *
 *
 * @brief Declaration of the Sample class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-17
 *
 * $Id:$
 */


#ifndef Sample_H
#define Sample_H

#include "Sample.h"

#include <map>

namespace RevBayesCore {

    template<class valueType>
    class Sample {

    public:
        Sample(const valueType& v, unsigned int f);
    
        // overloaded operators
        bool                operator<(const Sample &s) const;
    
        // getters and setters
        unsigned int        getFrequency(void) const { return frequency; }
        const valueType&    getValue(void) const { return value; }
        void                setFrequency(unsigned int f) { frequency = f; }
        void                setValue(const valueType &v) { value = v; }
    
    private:
        valueType           value;
        unsigned int        frequency;

    };
    
}


template <class valueType>
RevBayesCore::Sample<valueType>::Sample(const valueType &v, unsigned int f) : value( v ), frequency( f ) {
    
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator<(const Sample &s) const {
    return this->frequency < s.getFrequency();
}

#endif
