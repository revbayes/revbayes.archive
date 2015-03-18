#ifndef MixtureDistribution_H
#define MixtureDistribution_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    
    /**
     * This class implements a generic mixture distribution between several possible values.
     *
     * This mixture can be considered as a multinomial distribution. We specify a vector of probabilities
     * and a vector of values. Then, a value drawn from this distribution takes each value corresponding to
     * its probability.
     * The values are already of the correct mixture type. You may want to apply a mixture allocation move
     * to change between the current value. The values themselves change automatically when the input parameters change.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-11-18, version 1.0
     */
    template <class mixtureType>
    class MixtureDistribution : public TypedDistribution<mixtureType>, public MemberObject<int> {
        
    public:
        // constructor(s)
        MixtureDistribution(const TypedDagNode< RbVector<mixtureType> > *v, const TypedDagNode< RbVector<double> > *p);
        
        // public member functions
        MixtureDistribution*                                clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, int &rv) const;     //!< Map the member methods to internal function calls
        const std::vector<mixtureType>&                     getParameterValues(void) const;
        size_t                                              getCurrentIndex(void) const;
        size_t                                              getNumberOfCategories(void) const;
        void                                                redrawValue(void);
        void                                                setCurrentIndex(size_t i);
        void                                                setValue(mixtureType *v, bool f=false);
        
        // special handling of state changes
        void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                keepSpecialization(DagNode* affecter);
        void                                                restoreSpecialization(DagNode *restorer);
        void                                                touchSpecialization(DagNode *toucher);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter
        
        
    private:
        // helper methods
        const mixtureType&                                  simulate();
        
        // private members
        const TypedDagNode< RbVector<mixtureType> >*        parameterValues;
        const TypedDagNode< RbVector<double> >*             probabilities;
        
        size_t                                              index;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include <cmath>

template <class mixtureType>
RevBayesCore::MixtureDistribution<mixtureType>::MixtureDistribution(const TypedDagNode< RbVector<mixtureType> > *v, const TypedDagNode< RbVector<double> > *p) : TypedDistribution<mixtureType>( Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( v->getValue()[0] ) ),
    parameterValues( v ),
    probabilities( p ),
    index( 0 )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( parameterValues );
    this->addParameter( probabilities );
    
    *this->value = simulate();
}



template <class mixtureType>
RevBayesCore::MixtureDistribution<mixtureType>* RevBayesCore::MixtureDistribution<mixtureType>::clone( void ) const
{

    return new MixtureDistribution<mixtureType>( *this );
}



template <class mixtureType>
double RevBayesCore::MixtureDistribution<mixtureType>::computeLnProbability( void )
{
    
    return probabilities->getValue()[index];
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, int &rv) const
{
    
    if ( n == "getAllocationIndex" )
    {
        rv = int(index);
    }
    else
    {
        throw RbException("A mixture distribution does not have a member method called '" + n + "'.");
    }
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter)
{
    // only delegate when the toucher was our parameters
    if ( affecter == parameterValues )
    {
        this->dagNode->getAffectedNodes( affected );
    }
}


template <class mixtureType>
size_t RevBayesCore::MixtureDistribution<mixtureType>::getCurrentIndex( void ) const
{

    return index;
}


template <class mixtureType>
size_t RevBayesCore::MixtureDistribution<mixtureType>::getNumberOfCategories( void ) const
{

    return probabilities->getValue().size();
}


template <class mixtureType>
const std::vector<mixtureType>& RevBayesCore::MixtureDistribution<mixtureType>::getParameterValues( void ) const
{
    return parameterValues->getValue();
}

template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::keepSpecialization( DagNode* affecter )
{
    // only do this when the toucher was our parameters
    if ( affecter == parameterValues )
    {
        this->dagNode->keepAffected();
    }
}


template <class mixtureType>
const mixtureType& RevBayesCore::MixtureDistribution<mixtureType>::simulate()
{
    
    const std::vector<double> &probs = probabilities->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    index = 0;
    while ( u > probs[index] )
    {
        u -= probs[index];
        index++;
    }
    
    return parameterValues->getValue()[index];
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::redrawValue( void )
{

    *(this->value) = simulate();

}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::setCurrentIndex(size_t i)
{

    delete this->value;

    index = i;
    const mixtureType &tmp = parameterValues->getValue()[i];
    
    this->value = Cloner<mixtureType, IsDerivedFrom<mixtureType, Cloneable>::Is >::createClone( tmp );
}


/** Swap a parameter of the distribution */
template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == parameterValues)
    {
        parameterValues = static_cast<const TypedDagNode< RbVector<mixtureType> >* >( newP );
    }
    else if (oldP == probabilities)
    {
        probabilities = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::restoreSpecialization( DagNode *restorer )
{
    
    // only do this when the toucher was our parameters
    if ( restorer == parameterValues )
    {
        *this->value = parameterValues->getValue()[index];
        this->dagNode->restoreAffected();
    }
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::setValue(mixtureType *v, bool force)
{
    
    const RbVector<mixtureType> &vals = parameterValues->getValue();
    // we need to catch the value and increment the index
    for (index = 0; index < vals.size(); ++index)
    {
        if ( vals[index] == *v )
        {
            break;
        }
    }
    
    // delegate class
    TypedDistribution<mixtureType>::setValue( v, force );
}


template <class mixtureType>
void RevBayesCore::MixtureDistribution<mixtureType>::touchSpecialization( DagNode *toucher )
{
    // only do this when the toucher was our parameters
    if ( toucher == parameterValues )
    {
        *this->value = parameterValues->getValue()[index];
        this->dagNode->touchAffected();
    }
}

#endif
