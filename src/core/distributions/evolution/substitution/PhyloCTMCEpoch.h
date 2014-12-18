//
//  PhyloCTMCEpoch.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/30/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__PhyloCTMCEpoch__
#define __revbayes_proj__PhyloCTMCEpoch__

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TimeTree.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType>
    class PhyloCTMCEpoch : public AbstractPhyloCTMCSiteHomogeneous<charType, RevBayesCore::TimeTree> {
        
    public:
        PhyloCTMCEpoch(const TypedDagNode< RevBayesCore::TimeTree > *t, size_t nChars, bool c, size_t nSites, bool amb);
        virtual                                            ~PhyloCTMCEpoch(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloCTMCEpoch*                                     clone(void) const;                                                                          //!< Create an independent clone
        void                                                setEpochTimes(const TypedDagNode< RbVector< double > > *r);
        void                                                setEpochClockRates(const TypedDagNode< RbVector< double > > *r);
        void                                                setEpochRateMatrices(const TypedDagNode< RbVector< RateMatrix > > *rm);
        
    protected:
        
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
		void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
		void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);

        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        const TypedDagNode< RbVector< double > >*           epochTimes;
        const TypedDagNode< RbVector< RateMatrix > >*       epochRateMatrices;
        const TypedDagNode< RbVector< double > >*           epochClockRates;
        size_t                                              getEpochIndex(double t);
        
        bool                                                useEpochClockRates;
        
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>
#include <vector>

template<class charType>
RevBayesCore::PhyloCTMCEpoch<charType>::PhyloCTMCEpoch(const TypedDagNode<RevBayesCore::TimeTree> *t, size_t nChars, bool c, size_t nSites, bool amb) : AbstractPhyloCTMCSiteHomogeneous<charType, RevBayesCore::TimeTree>(  t, nChars, 1, c, nSites, amb )
{
    epochTimes = new ConstantNode< RbVector< double > >("epochTimes", new RbVector<double>(1, 0.0) );
    epochClockRates = new ConstantNode< RbVector< double > >("epochClockRates", new RbVector<double>(1, 1.0) );
    epochRateMatrices = new ConstantNode< RbVector< RateMatrix> >("epochRateMatrices", new RbVector<RateMatrix>( 1, RateMatrix_JC( nChars )));
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( epochTimes );
    this->addParameter( epochClockRates );
    this->addParameter( epochRateMatrices );
    
}


template<class charType>
RevBayesCore::PhyloCTMCEpoch<charType>::~PhyloCTMCEpoch( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType>
RevBayesCore::PhyloCTMCEpoch<charType>* RevBayesCore::PhyloCTMCEpoch<charType>::clone( void ) const {
    
    return new PhyloCTMCEpoch<charType>( *this );
}



template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    
    // reset the likelihood
    this->lnProb = 0.0;
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods of the left and right subtree
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->numPatterns,0.0);
    
    // get pointers the likelihood for both subtrees
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                tmp += *p_site_left_j * *p_site_right_j * *f_j;
                
                // increment pointers
                ++p_site_left_j; ++p_site_right_j;
            }
            // add the likelihood for this mixture category
            per_mixture_Likelihoods[site] += tmp;
            
            // increment the pointers to the next site
            p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture_left+=this->mixtureOffset; p_mixture_right+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    // sum the log-likelihoods for all sites together
    double p_inv = this->pInv->getValue();
    double oneMinusPInv = 1.0 - p_inv;
    std::vector< size_t >::const_iterator patterns = this->patternCounts.begin();
    if ( p_inv > 0.0 )
    {
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            if ( this->siteInvariant[site] )
            {
                this->lnProb += log( p_inv * f[ this->invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
            }
            else
            {
                this->lnProb += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
            }
        }
    }
    else
    {
        
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            this->lnProb += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
        }
        
    }
    
}


template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    computeRootLikelihood(root, left, right);
}


template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
	computeInternalNodeLikelihood(node, nodeIndex, left, right);
}

template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transitionProbMatrices[mixture].theMatrix;
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        // compute the per site probabilities
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            
            // get the pointers for this mixture category and this site
            const double*       tp_a    = tp_begin;
            // iterate over the possible starting states
            for (size_t c1 = 0; c1 < this->numChars; ++c1)
            {
                // temporary variable
                double sum = 0.0;
                
                // iterate over all possible terminal states
                for (size_t c2 = 0; c2 < this->numChars; ++c2 )
                {
                    sum += p_site_mixture_left[c2] * p_site_mixture_right[c2] * tp_a[c2];
                    
                } // end-for over all distination character
                
                // store the likelihood for this starting state
                p_site_mixture[c1] = sum;
                
                // increment the pointers to the next starting state
                tp_a+=this->numChars;
                
            } // end-for over all initial characters
            
            // increment the pointers to the next site
            p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)
    
}


template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{
    
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    const std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    const std::vector<unsigned long> &char_node = this->charMatrix[nodeIndex];
    

    // compute the transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    double*   p_mixture      = p_node;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*                       tp_begin    = this->transitionProbMatrices[mixture].theMatrix;
        
        // get the pointer to the likelihoods for this site and mixture category
        double*     p_site_mixture      = p_mixture;
        
        // iterate over all sites
        for (size_t site = 0; site != this->numPatterns; ++site)
        {
            
            // is this site a gap?
            if ( gap_node[site] )
            {
                // since this is a gap we need to assume that the actual state could have been any state
                
                // iterate over all initial states for the transitions
                for (size_t c1 = 0; c1 < this->numChars; ++c1)
                {
                    
                    // store the likelihood
                    p_site_mixture[c1] = 1.0;
                    
                }
            }
            else // we have observed a character
            {
                
                // get the original character
                unsigned long org_val = char_node[site];
                
                // iterate over all possible initial states
                for (size_t c1 = 0; c1 < this->numChars; ++c1)
                {
                    
                    if ( this->usingAmbiguousCharacters )
                    {
                        // compute the likelihood that we had a transition from state c1 to the observed state org_val
                        // note, the observed state could be ambiguous!
                        unsigned long val = org_val;
                        
                        // get the pointer to the transition probabilities for the terminal states
                        const double* d  = tp_begin+(this->numChars*c1);
                        
                        double tmp = 0.0;
                        
                        while ( val != 0 ) // there are still observed states left
                        {
                            // check whether we observed this state
                            if ( (val & 1) == 1 )
                            {
                                // add the probability
                                tmp += *d;
                            }
                            
                            // remove this state from the observed states
                            val >>= 1;
                            
                            // increment the pointer to the next transition probability
                            ++d;
                        } // end-while over all observed states for this character
                        
                        // store the likelihood
                        p_site_mixture[c1] = tmp;
                        
                    }
                    else // no ambiguous characters in use
                    {
                        
                        // store the likelihood
                        p_site_mixture[c1] = tp_begin[c1*this->numChars+org_val];
                        
                    }
                    
                } // end-for over all possible initial character for the branch
                
            } // end-if a gap state
            
            // increment the pointers to next site
            p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites/patterns in the sequence
        
        // increment the pointers to next mixture category
        p_mixture+=this->mixtureOffset;
        
    } // end-for over all mixture categories
    
}

template<class charType>
size_t RevBayesCore::PhyloCTMCEpoch<charType>::getEpochIndex(double t)
{
    size_t index = 0;
    while (t < epochTimes->getValue()[index])
    {
        index++;
    }
    return index;
}


template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::setEpochRateMatrices(const TypedDagNode< RbVector< RateMatrix > > *rm) {
    
    if (epochRateMatrices != NULL)
    {
        this->removeParameter( epochRateMatrices );
        epochRateMatrices = NULL;
    }
    
    // set the value
    epochRateMatrices = rm;
    
    // add the new parameter
    this->addParameter( epochRateMatrices );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::setEpochClockRates(const TypedDagNode< RbVector< double > > *r)
{
    
    if (epochClockRates != NULL)
    {
        this->removeParameter( epochClockRates );
        epochClockRates = NULL;
    }
    
    // set the value
    useEpochClockRates = true;
    epochClockRates = r;
    
    // add the new parameter
    this->addParameter( epochClockRates );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}

template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::setEpochTimes(const TypedDagNode< RbVector< double > > *t)
{
    
    if (epochTimes != NULL)
    {
        this->removeParameter( epochTimes );
        epochTimes = NULL;
    }
    
    // set the value
    epochTimes = t;
    
    // add the new parameter
    this->addParameter( epochTimes );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}

/** Swap a parameter of the distribution */
template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == epochTimes)
    {
        epochTimes = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == epochRateMatrices)
    {
        epochRateMatrices = static_cast<const TypedDagNode< RbVector< RateMatrix > >* >( newP );
    }
    else if (oldP == epochClockRates)
    {
        epochClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else
    {
        
        RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, RevBayesCore::TimeTree>::swapParameterInternal(oldP, newP);
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCEpoch<charType>::updateTransitionProbabilities(size_t nodeIdx, double brlen)
{
    
    // get branch start/end ages
    const std::vector<TopologyNode*>& nodes = this->tau->getValue().getNodes();
    const TopologyNode& nd = *nodes[nodeIdx];
    if (nd.isRoot())
        return;
    
    double t_curr = nd.getParent().getAge();
    double t_end = nd.getAge();
    
    // find epoch
    size_t epochIdx = getEpochIndex(t_curr);
    
    // P = I
    TransitionProbabilityMatrix tp(this->numChars);
    for (size_t i = 0; i < this->numChars; i++)
        tp[i][i] = 1.0;
    
    // multiply transition probs across epochs
    while (t_curr > t_end)
    {
        double t_next = 0.0;
        if (epochIdx < epochTimes->getValue().size())
            t_next = epochTimes->getValue()[epochIdx];
        
        // get next time, which is the next epoch or branc end
        if (t_next < t_end)
            t_next = t_end;
        double dt = t_curr - t_next;

        // first, get the rate matrix for this branch
        const RateMatrix *rm;
        rm = &epochRateMatrices->getValue()[epochIdx];
        
        // second, get the clock rate for the branch
        if ( this->branchHeterogeneousClockRates == true )
        {
            dt *= this->heterogeneousClockRates->getValue()[nodeIdx];
        }
        else
        {
            dt *= this->homogeneousClockRate->getValue();
        }
        
        if (useEpochClockRates)
        {
            dt *= epochClockRates->getValue()[epochIdx];
        }
        
        // and finally compute the per site rate transition probability matrix
        if ( this->rateVariationAcrossSites == true && false )
        {
            const std::vector<double> &r = this->siteRates->getValue();
            for (size_t i = 0; i < this->numSiteRates; ++i)
            {
                rm->calculateTransitionProbabilities( dt * r[i], this->transitionProbMatrices[i] );
            }
        }
        else
        {
            rm->calculateTransitionProbabilities( dt, this->transitionProbMatrices[0] );
        }
        
        // epochs construct DTMC
        tp *= this->transitionProbMatrices[0];
        
        // advance increment
        t_curr = t_next;
        epochIdx++;
    }
    
    this->transitionProbMatrices[0] = TransitionProbabilityMatrix(tp);
    
}


#endif /* defined(__revbayes_proj__PhyloCTMCEpoch__) */
