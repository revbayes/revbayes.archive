/**
 * @file
 * This file contains the declaration of the random variable class for a character state evolving along a tree.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the character state evolution along a tree class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */



#ifndef GeneralBranchHeterogeneousCharEvoModel_H
#define GeneralBranchHeterogeneousCharEvoModel_H

#include "AbstractSiteHomogeneousMixtureCharEvoModel.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class GeneralBranchHeterogeneousCharEvoModel : public AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType> {
        
    public:
        GeneralBranchHeterogeneousCharEvoModel(const TypedDagNode< treeType > *t, bool c, size_t nSites);
        GeneralBranchHeterogeneousCharEvoModel(const GeneralBranchHeterogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~GeneralBranchHeterogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        GeneralBranchHeterogeneousCharEvoModel*             clone(void) const;                                                                          //!< Create an independent clone
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< std::vector< double > > *r);
        void                                                setRateMatrix(const TypedDagNode< RateMatrix > *rm);
        void                                                setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                setSiteRates(const TypedDagNode< std::vector< double > > *r);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        const std::vector<double>&                          getRootFrequencies(void);
        
    private:        
        // members
        const TypedDagNode< double >*                       homogeneousClockRate;
        const TypedDagNode< std::vector< double > >*        heterogeneousClockRates;
        const TypedDagNode< RateMatrix >*                   homogeneousRateMatrix;
        const TypedDagNode< RbVector< RateMatrix > >*       heterogeneousRateMatrices;
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
        const TypedDagNode< std::vector< double > >*        siteRates;
        
        
        // flags specifying which model variants we use
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                rateVariationAcrossSites;
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::GeneralBranchHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, bool c, size_t nSites) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>(  t, 1, c, nSites ) {
    
    // initialize with default parameters
    homogeneousClockRate        = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates     = NULL;
    homogeneousRateMatrix       = new ConstantNode<RateMatrix>("rateMatrix", new RateMatrix_JC(charType().getNumberOfStates()) );
    heterogeneousRateMatrices   = NULL;
    rootFrequencies             = NULL;
    siteRates                   = NULL;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;
    
    // add the parameters to the parents list
    this->addParameter( homogeneousClockRate );
    this->addParameter( homogeneousRateMatrix );
    
    this->redrawValue();

}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::GeneralBranchHeterogeneousCharEvoModel(const GeneralBranchHeterogeneousCharEvoModel &d) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>( d ) {
    // parameters are automatically copied
    // initialize with default parameters
    homogeneousClockRate        = d.homogeneousClockRate;
    heterogeneousClockRates     = d.heterogeneousClockRates;
    homogeneousRateMatrix       = d.homogeneousRateMatrix;
    heterogeneousRateMatrices   = d.heterogeneousRateMatrices;
    rootFrequencies             = d.rootFrequencies;
    siteRates                   = d.siteRates;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = d.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = d.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = d.rateVariationAcrossSites;

}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::~GeneralBranchHeterogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>* RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    
    return new GeneralBranchHeterogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL ) 
    {
        return rootFrequencies->getValue();
    } 
    else 
    {
        return homogeneousRateMatrix->getValue().getStationaryFrequencies();
    }

}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    // first, get the rate matrix for this branch
    const RateMatrix *rm;
    if ( this->branchHeterogeneousSubstitutionMatrices == true ) 
    {
        rm = &this->heterogeneousRateMatrices->getValue()[nodeIdx];
    } 
    else 
    {
        rm = &this->homogeneousRateMatrix->getValue();
    }
    
    // second, get the clock rate for the branch
    double branchTime;
    if ( this->branchHeterogeneousClockRates == true ) 
    {
        branchTime = this->heterogeneousClockRates->getValue()[nodeIdx] * brlen;
    } 
    else 
    {
        branchTime = this->homogeneousClockRate->getValue() * brlen;
    }
    
    // and finally compute the per site rate transition probability matrix
    if ( this->rateVariationAcrossSites == true ) 
    {
        const std::vector<double> &r = this->siteRates->getValue();
        for (size_t i = 0; i < this->numSiteRates; ++i)
        {
            rm->calculateTransitionProbabilities( branchTime * r[i], this->transitionProbMatrices[i] );
        }
    } 
    else 
    {
        rm->calculateTransitionProbabilities( branchTime, this->transitionProbMatrices[0] );
    }
    
}



template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setClockRate(const TypedDagNode< double > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL ) 
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
    
    // add the parameter
    this->addParameter( homogeneousClockRate );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}



template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setClockRate(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL ) 
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
    
    // add the parameter
    this->addParameter( heterogeneousClockRates );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRateMatrix(const TypedDagNode< RateMatrix > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL ) 
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneousRateMatrices );
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateMatrix = rm;
    
    // add the parameter
    this->addParameter( homogeneousRateMatrix );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL ) 
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneousRateMatrices );
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateMatrices = rm;
    
    // add the parameter
    this->addParameter( heterogeneousRateMatrices );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRootFrequencies(const TypedDagNode< std::vector< double > > *f) {
    
    // remove the old parameter first
    if ( rootFrequencies != NULL ) 
    {
        this->removeParameter( rootFrequencies );
        rootFrequencies = NULL;
    }
    
    if ( f != NULL )
    {
        // set the value
//        branchHeterogeneousSubstitutionMatrices = true;
        rootFrequencies = f;
    
        // add the parameter
        this->addParameter( rootFrequencies );
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setSiteRates(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( siteRates != NULL ) 
    {
        this->removeParameter( siteRates );
        siteRates = NULL;
    }
    
    if ( r != NULL ) 
    {
        // set the value
        rateVariationAcrossSites = true;
        siteRates = r;
        this->numSiteRates = r->getValue().size();
        std::cerr << this->numSiteRates << std::endl;
        this->resizeLikelihoodVectors();
    }
    else
    {
        // set the value
        rateVariationAcrossSites = false;
        siteRates = NULL;
        this->numSiteRates = 1;
        this->resizeLikelihoodVectors();
        
    }
    
    // add the parameter
    this->addParameter( siteRates );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
}




template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == homogeneousClockRate) 
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates) 
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == homogeneousRateMatrix) 
    {
        homogeneousRateMatrix = static_cast<const TypedDagNode< RateMatrix >* >( newP );
    }
    else if (oldP == heterogeneousRateMatrices) 
    {
        heterogeneousRateMatrices = static_cast<const TypedDagNode< RbVector< RateMatrix > >* >( newP );
    }
    else if (oldP == rootFrequencies) 
    {
        rootFrequencies = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == siteRates) 
    {
        siteRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else 
    {
        AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
    
}


#endif
