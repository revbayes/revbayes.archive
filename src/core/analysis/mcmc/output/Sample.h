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

#include "TraceNumeric.h"

namespace RevBayesCore {

    template<class valueType>
    class Sample : public TraceNumeric {

    public:
        Sample(const valueType& v = valueType(), unsigned int f=0);
    
        // overloaded operators
        bool                            operator<(const Sample &s) const;
        bool                            operator<=(const Sample &s) const;
        bool                            operator==(const Sample &s) const;
        bool                            operator!=(const Sample &s) const;
    
        // getters and setters
        void                            addNonObservations(size_t);
        void                            addObservation(bool tf);
        unsigned int                    getFrequency(void) const;
        const valueType&                getValue(void) const;
        void                            setFrequency(unsigned int f);
        void                            setValue(const valueType &v);
    
    private:
        
        valueType           value;
        unsigned int        frequency;

    };
    
}


template <class valueType>
RevBayesCore::Sample<valueType>::Sample(const valueType &v, unsigned int f) :
    value( v ),
    frequency( f )
{
    
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator<(const Sample &s) const
{

    return this->frequency < s.getFrequency();
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator<=(const Sample &s) const
{
    
    return this->operator<( s ) || this->operator==( s );
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator==(const Sample &s) const
{
    
    return this->frequency == s.getFrequency();
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator!=(const Sample &s) const
{
    
    return this->operator==( s ) == false;
}


template <class valueType>
void RevBayesCore::Sample<valueType>::addNonObservations( size_t n )
{

    values.insert(values.begin(), n, 0.0);
}


template <class valueType>
void RevBayesCore::Sample<valueType>::addObservation( bool tf )
{

    this->frequency += tf;
    addObject( tf );

}


template <class valueType>
unsigned int RevBayesCore::Sample<valueType>::getFrequency( void ) const
{
    
    return frequency;
}


template <class valueType>
const valueType& RevBayesCore::Sample<valueType>::getValue( void ) const
{
    
    return value;
}


template <class valueType>
void RevBayesCore::Sample<valueType>::setFrequency( unsigned int f )
{
    
    frequency = f;
    
}


template <class valueType>
void RevBayesCore::Sample<valueType>::setValue( const valueType &v )
{
    
    value = v;
}

#endif
