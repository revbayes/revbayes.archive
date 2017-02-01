#ifndef BiogeographicTreeHistoryCtmc_H
#define BiogeographicTreeHistoryCtmc_H

#include "GeneralTreeHistoryCtmc.h"
#include "RbConstants.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"
#include "ConstantNode.h"
#include "HomologousDiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "RateGeneratorSequence.h"
#include "RateGeneratorSequenceUsingMatrix.h"

#include <cmath>
#include <cstring>

namespace RevBayesCore {
    
    
    template<class charType>
    class BiogeographicTreeHistoryCtmc : public GeneralTreeHistoryCtmc<charType> {
        
    public:
        BiogeographicTreeHistoryCtmc(const TypedDagNode< Tree > *t, size_t nChars, size_t nSites, bool useAmbigChar=false, bool forbidExtinction=true);
        BiogeographicTreeHistoryCtmc(const BiogeographicTreeHistoryCtmc &n);                                                                         //!< Copy constructor
        virtual                                            ~BiogeographicTreeHistoryCtmc(void);                                                //!< Virtual destructor
        
        // public member functions
        
        BiogeographicTreeHistoryCtmc*                       clone(void) const;                                                           //!< Create an independent clone
//        void                                                initializeTipValues(void);
//        void                                                drawInitValue(void);
//        void                                                redrawValue(void);
//        virtual void                                        simulate(void);
        
//        bool                                                samplePathStart(const TopologyNode& node);
//        bool                                                samplePathEnd(const TopologyNode& node);
//        bool                                                samplePathHistory(const TopologyNode& node);
        
        void                                                setCladogeneticStateProbabilities(const TypedDagNode< RbVector< double > > *r);
        
    protected:
        
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
//        virtual double                                      computeRootLikelihood(const TopologyNode &n);
//        virtual double                                      computeInternalNodeLikelihood(const TopologyNode &n);
//        virtual double                                      computeTipLikelihood(const TopologyNode &node);
        // (not needed)        void                         keepSpecialization(DagNode* affecter);
        // (not needed)        void                         restoreSpecialization(DagNode *restorer);
//        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // model settings
        bool                                                forbidExtinction;
        
    private:
        
        // members
        const TypedDagNode< RbVector< double > >*           cladogeneticStateProbabilities;
        
        // flags specifying which model variants we use
        
//        virtual void                                        simulate(const TopologyNode& node, BranchHistory* bh, std::vector< DiscreteTaxonData< charType > >& taxa);
//        virtual void                                        simulateHistory(const TopologyNode& node, BranchHistory* bh);
        
    };
    
}



#include "PathRejectionSampleProposal.h"
#include "RateMatrix_JC.h"
#include "RbConstants.h"

template<class charType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::BiogeographicTreeHistoryCtmc(const TypedDagNode<Tree> *tau, size_t nChars, size_t nSites, bool useAmbigChar, bool fe) : GeneralTreeHistoryCtmc<charType>( tau, nChars, nSites, useAmbigChar ),
    forbidExtinction(fe)
{
    
    // initialize with default parameters
    cladogeneticStateProbabilities = NULL;
    
    // flags specifying which model variants we use
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( cladogeneticStateProbabilities );
    
}


template<class charType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::BiogeographicTreeHistoryCtmc(const BiogeographicTreeHistoryCtmc &d) : GeneralTreeHistoryCtmc<charType>( d )
{
    // initialize with default parameters
    cladogeneticStateProbabilities        = d.cladogeneticStateProbabilities;
    
    // flags specifying which model variants we use
    
}


template<class charType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::~BiogeographicTreeHistoryCtmc( void )
{
    ; // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template<class charType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType>* RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::clone( void ) const
{
    
    return new BiogeographicTreeHistoryCtmc<charType>( *this );
}

template<class charType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::setCladogeneticStateProbabilities(const TypedDagNode< RbVector< double > > *p)
{
    
    // remove the old parameter first
    if ( cladogeneticStateProbabilities != NULL )
    {
        this->removeParameter( cladogeneticStateProbabilities );
        cladogeneticStateProbabilities = NULL;
    }
    
    if ( p != NULL )
    {
        // set the value
        cladogeneticStateProbabilities = p;
    }
    
    // add the new parameter
    this->addParameter( cladogeneticStateProbabilities );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}

/** Swap a parameter of the distribution */
template<class charType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if (oldP == cladogeneticStateProbabilities)
    {
        cladogeneticStateProbabilities = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        GeneralTreeHistoryCtmc<charType>::swapParameterInternal(oldP,newP);
    }
    
}


#endif /* defined(__rb_mlandis__BiogeographicTreeHistoryCtmc__) */
