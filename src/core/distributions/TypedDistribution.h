/**
 * @file
 * This file contains the declaration of the interface for all distribution.
 *
 * Distributions are either values inside DAG nodes, i.e. a constant node used as input for the DPP,
 * or be associated with a stochastic node.
 *
 * First, some distributions are requiring a distribution as a parameter, e.g. a generating distribution. Thus,
 * we need to implement distributions as objects storable in DAG nodes.
 *
 * Second, all stochastic nodes hold a distribution pointer. The value of the stochastic node is returned via
 * a call to get value in the distribution.
 *
 * Every distribution owns its value and hence this class is templated. Owning the value
 * has the advantage that calls to update can modify the value instead of creating a new object.
 * This is benefitial in functions generating large objects.
 *
 * @brief Declaration of distributions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-04, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */


#ifndef TypedDistribution_H
#define TypedDistribution_H

#include "Distribution.h"
#include "Function.h"
#include "UnivariateFunction.h"

#include <iostream>

namespace RevBayesCore {
    
    template <class valueType>
    class StochasticNode;
    
    template<class valueType>
    class TypedDistribution : public Distribution {
        
    public:
        // constructors and destructor
        virtual                        ~TypedDistribution(void);
        
        // public methods
        valueType&                      getValue(void);                                                             //!< Get the current value (non-const)
        const valueType&                getValue(void) const;                                                       //!< Get the current value
        StochasticNode<valueType>*      getStochasticNode(void);                                                    //!< Get the stochastic node holding this distribution
        void                            setStochasticNode(StochasticNode<valueType> *n);                            //!< Set the stochastic node holding this distribution
        
        // virtual methods
        virtual void                    setValue(valueType *v);                                                     //!< Set the current value, e.g. attach an observation (clamp)
        virtual void                    setValue(const valueType &v);                                               //!< Set the current value, e.g. attach an observation (clamp)
        
        // pure virtual public methods
        virtual TypedDistribution*      clone(void) const = 0;                                                      //!< Clone the distribution
        virtual double                  computeLnProbability(void) = 0;                                             //!< Clone the ln probability density
        virtual void                    redrawValue(void) = 0;                                                      //!< Draw a new random value from the distribution
        
    protected:
        TypedDistribution(valueType *v);
        TypedDistribution(const TypedDistribution &d);
        
        // overloaded operators
        TypedDistribution&              operator=(const TypedDistribution &d); 

        virtual void                    swapParameterInternal(const DagNode *oldP, const DagNode *newP) = 0;        //!< Exchange the parameter

        
        // inheritable attributes
        StochasticNode<valueType>*      dagNode;                                                                    //!< The stochastic node holding this distribution. This is needed for delegated calls to the DAG, such as getAffected(), ...
        valueType*                      value;
        
    };
    
    // Global functions using the class
    template <class valueType>
    std::ostream&               operator<<(std::ostream& o, const TypedDistribution<valueType>& x);                                //!< Overloaded output operator
    
}


#include "Cloneable.h"
#include "Cloneable.h"
#include "IsDerivedFrom.h"


template <class valueType>
RevBayesCore::TypedDistribution<valueType>::TypedDistribution(valueType *v) : Distribution(), 
    dagNode( NULL ), 
    value( v ) 
{
    
}

template <class valueType>
RevBayesCore::TypedDistribution<valueType>::TypedDistribution(const TypedDistribution &d) : Distribution(d), 
    dagNode( NULL ), 
    value( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *d.value ) ) 
{
    
}

template <class valueType>
RevBayesCore::TypedDistribution<valueType>::~TypedDistribution( void ) {
    
    delete value;
    
}



template <class valueType>
RevBayesCore::TypedDistribution<valueType>& RevBayesCore::TypedDistribution<valueType>::operator=(const TypedDistribution &d) {
    
    if ( this != &d ) 
    {
        // call base class
        Distribution::operator=( d );
        
        // make my own copy of the value (we rely on proper implementation of assignment operators)
        delete value;
        value = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( *d.value );
    }
    
    return *this;
}


template <class valueType>
valueType& RevBayesCore::TypedDistribution<valueType>::getValue(void) {
    
    return *value;
}

template <class valueType>
const valueType& RevBayesCore::TypedDistribution<valueType>::getValue(void) const {
    
    return *value;
}


template <class valueType>
RevBayesCore::StochasticNode<valueType>* RevBayesCore::TypedDistribution<valueType>::getStochasticNode( void ) {
    
    return dagNode;
}

template <class valueType>
void RevBayesCore::TypedDistribution<valueType>::setStochasticNode( StochasticNode<valueType> *n ) {
    
    dagNode = n;
}

template <class valueType>
void RevBayesCore::TypedDistribution<valueType>::setValue( valueType *v ) {
    
    // free memory
    if (value != v)
        delete value;
    
    value = v;
    
}

template <class valueType>
void RevBayesCore::TypedDistribution<valueType>::setValue( const valueType &v ) {
    
    // free memory
    delete value;
    
    value = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( v );
    
}


template <class valueType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const TypedDistribution<valueType>& f) {
    
    o << "Distribution()";
    
    return o;
}

#endif
