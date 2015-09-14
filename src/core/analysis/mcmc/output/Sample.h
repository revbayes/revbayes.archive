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

#include <vector>

namespace RevBayesCore {

    template<class valueType>
    class Sample {

    public:
        Sample(const valueType& v, unsigned int f=0);
    
        // overloaded operators
        bool                            operator<(const Sample &s) const;
    
        // getters and setters
        void                            addObservation(bool tf);
        void                            computeStatistics(void);
        double                          getEss(void) const;
        unsigned int                    getFrequency(void) const;
        size_t                          getSampleSize(void) const;
        const std::vector<double>&      getTrace(void) const;
        const valueType&                getValue(void) const;
        void                            setFrequency(unsigned int f);
        void                            setTrace(const std::vector<double> &t);
        void                            setValue(const valueType &v);
    
    private:
        
        valueType           value;
        unsigned int        frequency;
        std::vector<double> trace;
        double              ess;

    };
    
}



#include "TraceAnalysisContinuous.h"


template <class valueType>
RevBayesCore::Sample<valueType>::Sample(const valueType &v, unsigned int f) :
    value( v ),
    frequency( f ),
    trace(),
    ess(0)
{
    
}


template <class valueType>
bool RevBayesCore::Sample<valueType>::operator<(const Sample &s) const
{

    return this->frequency < s.getFrequency();
}


template <class valueType>
void RevBayesCore::Sample<valueType>::addObservation( bool tf )
{
    
    if ( tf == true )
    {
        this->frequency++;
        trace.push_back( 1.0 );
    }
    else
    {
        trace.push_back( 0.0 );
    }
    
}


template <class valueType>
void RevBayesCore::Sample<valueType>::computeStatistics( void )
{
    
    TraceAnalysisContinuous a;
    a.analyseCorrelation( trace );
    ess = a.getEss();
}


template <class valueType>
double RevBayesCore::Sample<valueType>::getEss( void ) const
{
    const_cast< Sample<valueType>* >(this)->computeStatistics();
    return ess;
}


template <class valueType>
unsigned int RevBayesCore::Sample<valueType>::getFrequency( void ) const
{
    
    return frequency;
}


template <class valueType>
size_t RevBayesCore::Sample<valueType>::getSampleSize( void ) const
{
    
    return trace.size();
}


template <class valueType>
const std::vector<double>& RevBayesCore::Sample<valueType>::getTrace( void ) const
{
    
    return trace;
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
void RevBayesCore::Sample<valueType>::setTrace( const std::vector<double> &t )
{
    
    trace = t;
    
}


template <class valueType>
void RevBayesCore::Sample<valueType>::setValue( const valueType &v )
{
    
    value = v;
}

#endif
