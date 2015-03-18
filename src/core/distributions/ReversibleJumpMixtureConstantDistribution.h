#ifndef ReversibleJumpMixtureConstantDistribution_H
#define ReversibleJumpMixtureConstantDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    
    /**
     * This class implements a generic mixture distribution between a constant value and a distribution.
     *
     * The mixture consists of two components:
     *   a) a constant value
     *   b) a distribution
     * with probability p the current value is from a) and with probability 1-p it is from b).
     * Here we need reversible jump MCMC to mix/jump between the two possible states.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-11-18, version 1.0
     */
    template <class mixtureType>
    class ReversibleJumpMixtureConstantDistribution : public TypedDistribution<mixtureType>, public MemberObject<int> {
        
    public:
        // constructor(s)
        ReversibleJumpMixtureConstantDistribution(const TypedDagNode< mixtureType > *cv, TypedDistribution< mixtureType > *dv, const TypedDagNode<double> *p);
        
        // public member functions
        ReversibleJumpMixtureConstantDistribution*          clone(void) const;                                                                              //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, int &rv) const;    //!< Map the member methods to internal function calls
        const TypedDistribution<mixtureType>&               getBaseDistribution(void) const;
        TypedDistribution<mixtureType>&                     getBaseDistribution(void);
        const mixtureType&                                  getConstantValue(void) const;
        size_t                                              getCurrentIndex(void) const;
        size_t                                              getNumberOfCategories(void) const;
        void                                                redrawValue(void);
        void                                                redrawValueByIndex(int i);
        void                                                setCurrentIndex(size_t i);
        void                                                setValue(mixtureType *v, bool f=false);
        
    
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        
    private:
        // helper methods
        mixtureType*                                        simulate();
        
        // private members
        const TypedDagNode< mixtureType >*                  constValue;
        TypedDistribution<mixtureType>*						baseDistribution;
        const TypedDagNode< double >*                       probability;
        
        size_t                                              index;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include <cmath>

template <class mixtureType>
RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::ReversibleJumpMixtureConstantDistribution(const TypedDagNode< mixtureType > *cv, TypedDistribution< mixtureType > *dv, const TypedDagNode< double > *p) : TypedDistribution<mixtureType>( Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( cv->getValue() ) ),
    constValue( cv ),
    baseDistribution( dv ),
    probability( p ),
    index( 0 )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( constValue );
    this->addParameter( probability );
    
    // add the parameters of the distribution
    const std::set<const DagNode*>& pars = baseDistribution->getParameters();
    for (std::set<const DagNode*>::iterator it = pars.begin(); it != pars.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    delete this->value;
    this->value = simulate();
}



template <class mixtureType>
RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>* RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::clone( void ) const
{
    
    return new ReversibleJumpMixtureConstantDistribution<mixtureType>( *this );
}



template <class mixtureType>
double RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::computeLnProbability( void )
{
    
    double lnProb;
    if ( index == 0 )
    {
        if ( *this->value != constValue->getValue() )
        {
            lnProb = RbConstants::Double::neginf;
        }
        else
        {
            lnProb = log( probability->getValue() );
        }
        
    }
    else
    {
        lnProb = log( 1.0 - probability->getValue() );
        baseDistribution->setValue( Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone(*this->value) );
        lnProb += baseDistribution->computeLnProbability();
        
    }
    
    return lnProb;
}


template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, int &rv) const
{
    
    if ( n == "index" )
    {
        rv = int(index);
    }
    else
    {
        throw RbException("A mixture distribution does not have a member method called '" + n + "'.");
    }
}


template <class mixtureType>
const RevBayesCore::TypedDistribution<mixtureType>& RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::getBaseDistribution( void ) const
{
    
    return *baseDistribution;
}


template <class mixtureType>
RevBayesCore::TypedDistribution<mixtureType>& RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::getBaseDistribution( void )
{
    
    return *baseDistribution;
}


template <class mixtureType>
const mixtureType& RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::getConstantValue( void ) const
{
    
    return constValue->getValue();
}



template <class mixtureType>
size_t RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::getCurrentIndex( void ) const
{
    
    return index;
}


template <class mixtureType>
size_t RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::getNumberOfCategories( void ) const
{
    
    return 2;
}


template <class mixtureType>
mixtureType* RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::simulate()
{
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    
    if ( u < probability->getValue() )
    {
        index = 0;
        return Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( constValue->getValue() );
    }
    else
    {
        index = 1;
        baseDistribution->redrawValue();
        return Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( baseDistribution->getValue() );
    }
    
}


template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::redrawValue( void )
{
    
    delete this->value;
    this->value = simulate();
    
}


template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::redrawValueByIndex( int i )
{
    delete this->value;
    
    if (i == 0)
    {
        index = 0;
        this->value = Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( constValue->getValue() );
    }
    else
    {
        index = 1;
        baseDistribution->redrawValue();
        this->value = Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( baseDistribution->getValue() );
    }
}


template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::setCurrentIndex(size_t i)
{
    // we complete rely on the move to also change the value accordingly!!!
    index = i;
}


/** Swap a parameter of the distribution */
template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == constValue)
    {
        constValue = static_cast<const TypedDagNode< mixtureType >* >( newP );
    }
    else if (oldP == probability)
    {
        probability = static_cast<const TypedDagNode< double >* >( newP );
    }
    else
    {
        baseDistribution->swapParameter(oldP,newP);
    }
}


template <class mixtureType>
void RevBayesCore::ReversibleJumpMixtureConstantDistribution<mixtureType>::setValue(mixtureType *v, bool force)
{
    
    delete this->value;
    
    if ( force == false )
    {
        this->value = v;
    }
    else
    {
        
        if ( *v == constValue->getValue() )
        {
            index = 0;
            this->value = v;
        }
        else
        {
            index = 1;
            baseDistribution->setValue( Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( *v ) );
            this->value = v;
        }
    
    }
    //throw RbException("Cannot set the value of a reversible jump mixture distribution because we don't know which distribution the value should come from.");
}

#endif
