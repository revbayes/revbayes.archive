#ifndef PhyloCTMCSiteHomogeneousRestriction_H
#define PhyloCTMCSiteHomogeneousRestriction_H

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DistributionBinomial.h"
#include "DistributionNegativeBinomial.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

	enum CodingType { ALL 				= 0x00,
					  NOABSENCESITES	= 0x01,
					  NOPRESENCESITES	= 0x02,
					  VARIABLE			= 0x03,
					  NOSINGLETONGAINS 	= 0x04,
					  NOSINGLETONLOSSES	= 0x08,
					  NOSINGLETONS		= 0x0C,
					  INFORMATIVE		= 0x0F
					};
    
    template<class charType, class treeType>
    class PhyloCTMCSiteHomogeneousRestriction : public AbstractPhyloCTMCSiteHomogeneous<charType, treeType> {
        
    public:
        PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode< treeType > *t, size_t nChars, bool c, size_t nSites, bool amb, int cod = 0);
        PhyloCTMCSiteHomogeneousRestriction(const PhyloCTMCSiteHomogeneousRestriction &n);
        virtual                                            ~PhyloCTMCSiteHomogeneousRestriction(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloCTMCSiteHomogeneousRestriction*                clone(void) const;                                                                        //!< Create an independent clone
        void                                        		redrawValue(void);
        
    protected:
        
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
        void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        
        virtual void                        				computeRootCorrection(size_t root, size_t l, size_t r);
        virtual void                        				computeRootCorrection(size_t root, size_t l, size_t r, size_t m);
		virtual void                        				computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
		virtual void                        				computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
		virtual void                        				computeTipCorrection(const TopologyNode &node, size_t nIdx);

		virtual void                                        resizeLikelihoodVectors(void);

        
    private:        

		int													type;
		size_t												N;

		double												perSiteCorrection;
		size_t												numCorrectionSites;
		size_t												activeCorrectionOffset;
		size_t												nodeCorrectionOffset;
		size_t												mixtureCorrectionOffset;

		std::vector<double>                                 correctionLikelihoods;

		std::vector<double>									perMixtureCorrections;

		virtual double                                      sumRootLikelihood( void );
		size_t 												simulateRestriction( const TopologyNode &node, std::vector<charType> &taxa, size_t rateIndex);

    
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
RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::PhyloCTMCSiteHomogeneousRestriction(const TypedDagNode<treeType> *t, size_t nChars, bool c, size_t nSites, bool amb, int ty) :
	AbstractPhyloCTMCSiteHomogeneous<charType, treeType>(  t, 2, 1, c, nSites, amb ), type(ty), N(nSites), perSiteCorrection(0.0), numCorrectionSites(0)
{

	this->numCorrectionSites = 4*(type > 0);
	activeCorrectionOffset 	= this->numNodes*this->numSiteRates*numCorrectionSites*this->numChars;
	nodeCorrectionOffset 	= this->numSiteRates*numCorrectionSites*this->numChars;
	mixtureCorrectionOffset = numCorrectionSites*this->numChars;

	perMixtureCorrections = std::vector<double>(this->numSiteRates, 0.0);
}


template<class charType, class treeType>
RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::~PhyloCTMCSiteHomogeneousRestriction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>* RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::clone( void ) const {
    
    return new PhyloCTMCSiteHomogeneousRestriction<charType, treeType>( *this );
}

template<class charType, class treeType>
RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::PhyloCTMCSiteHomogeneousRestriction(const PhyloCTMCSiteHomogeneousRestriction &n) :
	AbstractPhyloCTMCSiteHomogeneous< charType, treeType >( n ),
	type(n.type),
	N(n.N),
	perSiteCorrection(n.perSiteCorrection),
	numCorrectionSites(n.numCorrectionSites),
	activeCorrectionOffset(n.activeCorrectionOffset),
	nodeCorrectionOffset(n.activeCorrectionOffset),
	mixtureCorrectionOffset(n.activeCorrectionOffset),
	perMixtureCorrections(n.perMixtureCorrections)
{
	if ( this->inMcmcMode == true)
		correctionLikelihoods = n.correctionLikelihoods;
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]  * this->activeLikelihoodOffset + root  * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]  * this->activeLikelihoodOffset + left  * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right] * this->activeLikelihoodOffset + right * this->nodeOffset;
    
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->numPatterns,0.0);
    
    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture) 
    {
        
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
                  double* p_site_j        = p_site_mixture;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j) 
            {
                // add the probability of starting from this state
                *p_site_j = *p_site_left_j * *p_site_right_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_left_j; ++p_site_right_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_left+=this->mixtureOffset; p_mixture_right+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    computeRootCorrection(root, left, right);
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods of the left and right subtree
          double* p        = this->partialLikelihoods + this->activeLikelihood[root]   * this->activeLikelihoodOffset + root   * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]   * this->activeLikelihoodOffset + left   * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right]  * this->activeLikelihoodOffset + right  * this->nodeOffset;
    const double* p_middle = this->partialLikelihoods + this->activeLikelihood[middle] * this->activeLikelihoodOffset + middle * this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
          double*   p_mixture          = p;
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
              double*   p_site_mixture          = p_mixture;
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        const double*   p_site_mixture_middle   = p_mixture_middle;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
                  double* p_site_j        = p_site_mixture;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            const double* p_site_middle_j = p_site_mixture_middle;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                *p_site_j = *p_site_left_j * *p_site_right_j * *p_site_middle_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_left_j; ++p_site_right_j; ++p_site_middle_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_left+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture_middle+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_left+=this->mixtureOffset; p_mixture_right+=this->mixtureOffset; p_mixture_middle+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    computeRootCorrection(root, left, right, middle);
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
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
    
    computeInternalNodeCorrection(node, nodeIndex, left, right);
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left      = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_middle    = this->partialLikelihoods + this->activeLikelihood[middle]*this->activeLikelihoodOffset + middle*this->nodeOffset;
    const double*   p_right     = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node      = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transitionProbMatrices[mixture].theMatrix;
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_middle   = p_middle + offset;
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
                    sum += p_site_mixture_left[c2] * p_site_mixture_middle[c2] * p_site_mixture_right[c2] * tp_a[c2];
                    
                } // end-for over all distination character
                
                // store the likelihood for this starting state
                p_site_mixture[c1] = sum;
                
                // increment the pointers to the next starting state
                tp_a+=this->numChars;
                
            } // end-for over all initial characters
            
            // increment the pointers to the next site
            p_site_mixture_left+=this->siteOffset; p_site_mixture_middle+=this->siteOffset; p_site_mixture_right+=this->siteOffset; p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)
    
    computeInternalNodeCorrection(node, nodeIndex, left, right, middle);
}




template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
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
    
    computeTipCorrection(node, nodeIndex);

}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeTipCorrection(const TopologyNode &node, size_t nodeIndex)
{
	if(this->numCorrectionSites == 0)
		return;

	std::vector<double>::iterator p_node = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*this->nodeCorrectionOffset;

	// iterate over all mixture categories

	for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
	{

		size_t offset = mixture*this->mixtureCorrectionOffset;

		std::vector<double>::iterator     	u      = p_node + offset;

		//Probability of no observed presence in all leaves descending from this node
		u[0] = 1.0;
		u[1] = 0.0;

		//Probability of observed presence in only one leaf descending from this node
		u[2] = 0.0;
		u[3] = 1.0;

		//Probability of observed presence in all leaves descending from this node
		u[4] = 0.0;
		u[5] = 1.0;

		//Probability of observed presence in all but one leaves descending from this node
		u[6] = 1.0;
		u[7] = 0.0;

	}
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
	if(this->numCorrectionSites == 0)
		return;

	// get the pointers to the partial likelihoods for this node and the two descendant subtrees
	std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*this->activeCorrectionOffset + left*this->nodeCorrectionOffset;
	std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*this->activeCorrectionOffset + right*this->nodeCorrectionOffset;
	std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*this->activeCorrectionOffset + middle*this->nodeCorrectionOffset;
	std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*this->nodeCorrectionOffset;

	const TopologyNode &left_node = this->tau->getValue().getNode(left);
	const TopologyNode &right_node = this->tau->getValue().getNode(right);
	const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

	// iterate over all mixture categories

	for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
	{
		this->updateTransitionProbabilities( left, left_node.getBranchLength() );
		TransitionProbabilityMatrix	pij    		= this->transitionProbMatrices[mixture];

		this->updateTransitionProbabilities( right, right_node.getBranchLength() );
		TransitionProbabilityMatrix	pik    		= this->transitionProbMatrices[mixture];

		this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
		TransitionProbabilityMatrix	pil    		= this->transitionProbMatrices[mixture];

		size_t offset = mixture*this->mixtureCorrectionOffset;

		std::vector<double>::iterator     			u       = p_node + offset;
		std::vector<double>::const_iterator     	u_j  	= p_left + offset;
		std::vector<double>::const_iterator     	u_k 	= p_right + offset;
		std::vector<double>::const_iterator     	u_l 	= p_middle + offset;

		for(size_t site = 0; site < 8; site += 4){
			u[site] = 0;
			u[site + 1] = 0;
			u[site + 2] = 0;
			u[site + 3] = 0;
			for(size_t cj = 0; cj < 2; cj++){
				for(size_t ck = 0; ck < 2; ck++){
					for(size_t cl = 0; cl < 2; cl++){
						u[site] 	+= pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + cl];
						u[site + 1] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + cl];

						u[site + 2] += pij[0][cj]*u_j[site + 2 + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + cl]
									 + pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + 2 + ck]*pil[0][cl]*u_l[site + cl]
									 + pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + 2 + cl];

						u[site + 3] += pij[1][cj]*u_j[site + 2 + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + cl]
									 + pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + 2 + ck]*pil[1][cl]*u_l[site + cl]
									 + pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + 2 + cl];
					}
				}
			}
		}
	}
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeInternalNodeCorrection(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{

	if(this->numCorrectionSites == 0)
		return;

	// get the pointers to the partial likelihoods for this node and the two descendant subtrees
	std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*this->activeCorrectionOffset + left*this->nodeCorrectionOffset;
	std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*this->activeCorrectionOffset + right*this->nodeCorrectionOffset;
	std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[nodeIndex]*this->activeCorrectionOffset + nodeIndex*this->nodeCorrectionOffset;

	const TopologyNode &left_node = this->tau->getValue().getNode(left);
	const TopologyNode &right_node = this->tau->getValue().getNode(right);

	// iterate over all mixture categories

	for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
	{
		this->updateTransitionProbabilities( left, left_node.getBranchLength() );
		TransitionProbabilityMatrix	pij    		= this->transitionProbMatrices[mixture];

		this->updateTransitionProbabilities( right, right_node.getBranchLength() );
		TransitionProbabilityMatrix	pik    		= this->transitionProbMatrices[mixture];

		size_t offset = mixture*this->mixtureCorrectionOffset;

		std::vector<double>::iterator     			u       = p_node + offset;
		std::vector<double>::const_iterator     	u_j  	= p_left + offset;
		std::vector<double>::const_iterator     	u_k 	= p_right + offset;

		for(size_t site = 0; site < 8; site += 4){
			u[site] 	= 0;
			u[site + 1] = 0;
			u[site + 2] = 0;
			u[site + 3] = 0;
			for(size_t cj = 0; cj < 2; cj++){
				for(size_t ck = 0; ck < 2; ck++){
					u[site] 	+= pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck];
					u[site + 1] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck];

					u[site + 2] += pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + 2 + ck] + pij[0][cj]*u_j[site + 2 + cj]*pik[0][ck]*u_k[site + ck];
					u[site + 3] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + 2 + ck] + pij[1][cj]*u_j[site + 2 + cj]*pik[1][ck]*u_k[site + ck];
				}
			}
		}
	}
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeRootCorrection( size_t root, size_t left, size_t right, size_t middle)
{
	this->perSiteCorrection = 0.0;

	if(this->numCorrectionSites > 0){

		const std::vector<double> &f                    = this->getRootFrequencies();

		// get the pointers to the partial likelihoods for this node and the two descendant subtrees
		std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*this->activeCorrectionOffset + left*this->nodeCorrectionOffset;
		std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*this->activeCorrectionOffset + right*this->nodeCorrectionOffset;
		std::vector<double>::const_iterator   p_middle = correctionLikelihoods.begin() + this->activeLikelihood[middle]*this->activeCorrectionOffset + middle*this->nodeCorrectionOffset;
		std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[root]*this->activeCorrectionOffset + root*this->nodeCorrectionOffset;

		const TopologyNode &left_node = this->tau->getValue().getNode(left);
		const TopologyNode &right_node = this->tau->getValue().getNode(right);
		const TopologyNode &middle_node = this->tau->getValue().getNode(middle);

		// iterate over all mixture categories
		for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
		{
			this->perMixtureCorrections[mixture] = 0.0;

			//get the 1->1 transition probabilities for each branch
			this->updateTransitionProbabilities( left, left_node.getBranchLength() );
			TransitionProbabilityMatrix	pij    		= this->transitionProbMatrices[mixture];

			this->updateTransitionProbabilities( right, right_node.getBranchLength() );
			TransitionProbabilityMatrix	pik    		= this->transitionProbMatrices[mixture];

			this->updateTransitionProbabilities( middle, middle_node.getBranchLength() );
			TransitionProbabilityMatrix	pil    		= this->transitionProbMatrices[mixture];

			size_t offset = mixture*this->mixtureCorrectionOffset;

			std::vector<double>::iterator     			u       = p_node + offset;
			std::vector<double>::const_iterator     	u_j  	= p_left + offset;
			std::vector<double>::const_iterator     	u_k 	= p_right + offset;
			std::vector<double>::const_iterator     	u_l 	= p_middle + offset;

			for(size_t site = 0; site < 8; site += 4){
				u[site] = 0;
				u[site + 1] = 0;
				u[site + 2] = 0;
				u[site + 3] = 0;
				for(size_t cj = 0; cj < 2; cj++){
					for(size_t ck = 0; ck < 2; ck++){
						for(size_t cl = 0; cl < 2; cl++){
							u[site] 	+= pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + cl];
							u[site + 1] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + cl];

							u[site + 2] += pij[0][cj]*u_j[site + 2 + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + cl]
										 + pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + 2 + ck]*pil[0][cl]*u_l[site + cl]
										 + pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck]*pil[0][cl]*u_l[site + 2 + cl];

							u[site + 3] += pij[1][cj]*u_j[site + 2 + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + cl]
										 + pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + 2 + ck]*pil[1][cl]*u_l[site + cl]
										 + pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck]*pil[1][cl]*u_l[site + 2 + cl];
						}
					}
				}
			}

			bool jl = left_node.isTip();
			bool kl = right_node.isTip();
			bool ll = middle_node.isTip();

			//std::cerr << u[0] << "\t" << u[1] << std::endl;

			double prob = 1.0;
			if(this->type & NOABSENCESITES)
				prob -= u[0]*f[0] + u[1]*f[1];
			if(this->type & NOSINGLETONGAINS)
				prob -= u[2]*f[0] + u[3]*f[1];
			if(this->type & NOPRESENCESITES)
				prob -= u[4]*f[0] + u[5]*f[1];
			//If all of this node's children are leaves, then u[1] = u[3]
			if((this->type & NOSINGLETONLOSSES) && !(jl && kl && ll))
				prob -= u[6]*f[0] + u[7]*f[1];

			// correct rounding errors
			if(prob < 0)
				prob = 0;

			this->perMixtureCorrections[mixture] = prob;
		}

		// sum the likelihoods for all mixtures together
		for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
			this->perSiteCorrection += this->perMixtureCorrections[mixture];

		// add a correction for invariant sites
		if(this->type & (NOABSENCESITES | NOPRESENCESITES))
			this->perSiteCorrection += this->numSiteRates*this->pInv->getValue();

		// normalize the log-probability
		this->perSiteCorrection = log(this->perSiteCorrection) - log(this->numSiteRates);
	}
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::computeRootCorrection( size_t root, size_t left, size_t right)
{
	this->perSiteCorrection = 0.0;

	if(this->numCorrectionSites > 0){

		const std::vector<double> &f                    = this->getRootFrequencies();

		// get the pointers to the partial likelihoods for this node and the two descendant subtrees
		std::vector<double>::const_iterator   p_left  = correctionLikelihoods.begin() + this->activeLikelihood[left]*this->activeCorrectionOffset + left*this->nodeCorrectionOffset;
		std::vector<double>::const_iterator   p_right = correctionLikelihoods.begin() + this->activeLikelihood[right]*this->activeCorrectionOffset + right*this->nodeCorrectionOffset;
		std::vector<double>::iterator         p_node  = correctionLikelihoods.begin() + this->activeLikelihood[root]*this->activeCorrectionOffset + root*this->nodeCorrectionOffset;

		const TopologyNode &left_node = this->tau->getValue().getNode(left);
		const TopologyNode &right_node = this->tau->getValue().getNode(right);

		// iterate over all mixture categories
		for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
		{
			this->perMixtureCorrections[mixture] = 0.0;

			//get the 1->1 transition probabilities for each branch
			this->updateTransitionProbabilities( left, left_node.getBranchLength() );
			TransitionProbabilityMatrix	pij    		= this->transitionProbMatrices[mixture];

			this->updateTransitionProbabilities( right, right_node.getBranchLength() );
			TransitionProbabilityMatrix	pik    		= this->transitionProbMatrices[mixture];

			size_t offset = mixture*this->mixtureCorrectionOffset;

			std::vector<double>::iterator     			u       = p_node + offset;
			std::vector<double>::const_iterator     	u_j  	= p_left + offset;
			std::vector<double>::const_iterator     	u_k 	= p_right + offset;

			for(size_t site = 0; site < 8; site += 4){
				u[site] = 0;
				u[site + 1] = 0;
				u[site + 2] = 0;
				u[site + 3] = 0;
				for(size_t cj = 0; cj < 2; cj++){
					for(size_t ck = 0; ck < 2; ck++){
						u[site] 	+= pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + ck];
						u[site + 1] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + ck];

						u[site + 2] += pij[0][cj]*u_j[site + cj]*pik[0][ck]*u_k[site + 2 + ck] + pij[0][cj]*u_j[site + 2 + cj]*pik[0][ck]*u_k[site + ck];
						u[site + 3] += pij[1][cj]*u_j[site + cj]*pik[1][ck]*u_k[site + 2 + ck] + pij[1][cj]*u_j[site + 2 + cj]*pik[1][ck]*u_k[site + ck];
					}
				}
			}

			bool jl = left_node.isTip();
			bool kl = right_node.isTip();


			//std::cerr << u[0] << "\t" << u[1] << std::endl;

			double prob = 1.0;
			if(this->type & NOABSENCESITES)
				prob -= u[0]*f[0] + u[1]*f[1];
			if(this->type & NOSINGLETONGAINS)
				prob -= u[2]*f[0] + u[3]*f[1];
			if(this->type & NOPRESENCESITES)
				prob -= u[4]*f[0] + u[5]*f[1];
			//If both of this node's children are leaves, then u[1] = u[3]
			if((this->type & NOSINGLETONLOSSES) && !(jl && kl))
				prob -= u[6]*f[0] + u[7]*f[1];

			// correct rounding errors
			if(prob < 0)
				prob = 0;

			this->perMixtureCorrections[mixture] = prob;
		}

		// sum the likelihoods for all mixtures together
		for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
			this->perSiteCorrection += this->perMixtureCorrections[mixture];

		// add a correction for invariant sites
		if(this->type & (NOABSENCESITES | NOPRESENCESITES))
			this->perSiteCorrection += this->numSiteRates*this->pInv->getValue();

		// normalize the log-probability
		this->perSiteCorrection = log(this->perSiteCorrection) - log(this->numSiteRates);
	}
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::resizeLikelihoodVectors( void ) {

	RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::resizeLikelihoodVectors();

	if ( this->inMcmcMode == true){
		correctionLikelihoods = std::vector<double>(2*this->numNodes*this->numSiteRates*numCorrectionSites*this->numChars, 0.0);
	}

	perMixtureCorrections = std::vector<double>(this->numSiteRates, 0.0);

	activeCorrectionOffset	= this->numNodes*this->numSiteRates*numCorrectionSites*this->numChars;
	nodeCorrectionOffset	= this->numSiteRates*numCorrectionSites*this->numChars;
	mixtureCorrectionOffset	= this->numCorrectionSites*this->numChars;
}

template<class charType, class treeType>
double RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();

    // get the index of the root node
    size_t nodeIndex = root.getIndex();

    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex] * this->activeLikelihoodOffset  + nodeIndex*this->nodeOffset;

    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->pattern_block_size,0.0);

    // get pointer the likelihood
    double*   p_mixture     = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {

        // get pointers to the likelihood for this mixture category
        double*   p_site_mixture     = p_mixture;
        // iterate over all sites

        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointers to the likelihoods for this site and mixture category
            double* p_site_j   = p_site_mixture;
            // iterate over all starting states
            for (size_t i=0; i<this->numChars; ++i)
            {
                // add the probability of starting from this state
                tmp += *p_site_j;

                // increment pointers
                ++p_site_j;
            }
            // add the likelihood for this mixture category
            per_mixture_Likelihoods[site] += tmp;

            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();

    double p_inv = this->pInv->getValue();
    double oneMinusPInv = 1.0 - p_inv;
    std::vector< size_t >::const_iterator patterns = this->patternCounts.begin();
    if ( p_inv > 0.0 )
    {
        for (size_t site = 0; site < this->pattern_block_size; ++site, ++patterns)
        {

            if ( this->useScaling == true )
            {

                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ] * exp(this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site]) + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;

            }
            else // no scaling
            {

                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }

            }
        }
    }
    else
    {

        for (size_t site = 0; site < this->pattern_block_size; ++site, ++patterns)
        {

            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;

            if ( this->useScaling == true )
            {

                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;
            }

        }


    }


#ifdef RB_MPI

    if ( !this->processActive )
    {
        // send from the workers the log-likelihood to the master
        MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, this->activePID, 0);
    }

    if ( this->processActive )
    {
        for (size_t i=this->activePID+1; i<this->activePID+this->numProcesses; ++i)
        {
            double tmp = 0;
            MPI::COMM_WORLD.Recv(&tmp, 1, MPI::DOUBLE, (int)i, 0);
            sumPartialProbs += tmp;
        }
    }

    if ( this->processActive )
    {
        for (size_t i=this->activePID+1; i<this->activePID+this->numProcesses; ++i)
        {
            MPI::COMM_WORLD.Send(&sumPartialProbs, 1, MPI::DOUBLE, (int)i, 0);
        }
    }
    else
    {
        MPI::COMM_WORLD.Recv(&sumPartialProbs, 1, MPI::DOUBLE, this->activePID, 0);
    }

#endif

    return sumPartialProbs - this->perSiteCorrection*this->numSites;
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::redrawValue( void ) {

	//this->computeLnProbability();

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<charType>();

    size_t numTips = this->tau->getValue().getNumberOfTips();
    size_t numNodes = this->tau->getValue().getNumberOfNodes();

    RandomNumberGenerator* rng = GLOBAL_RNG;

    const std::vector< double > &pi = this->getRootFrequencies();

    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t rootIndex = this->tau->getValue().getRoot().getIndex();

    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData<charType> >(numTips, DiscreteTaxonData<charType>("") );


    // first sample a total number of characters (M)
    // from the marginal posterior M - N | N ~ NegBinomial(N+1, exp(lnCorrection) )
    if(this->numCorrectionSites > 0){
		//double gamma = RbStatistics::Gamma::rv(this->N,exp(this->perSiteCorrection), *rng);
    	double M = RbStatistics::NegativeBinomial::rv(this->N + 1,exp(this->perSiteCorrection), *rng);
		// resample numSites from Binomial( M + N, exp(lnCorrection) )
		this->numSites = RbStatistics::Binomial::rv( M + this->N, exp(this->perSiteCorrection), *rng);
    }

    // sample the rate categories conditioned on the likelihood of the unobservable site-patterns
    std::vector<size_t> perSiteRates;
	double total = 0.0;
	for ( size_t i = 0; i < this->numSiteRates; ++i )
		total += 1 - this->perMixtureCorrections[i];

    for ( size_t i = 0; i < this->numSites; ++i )
	{
		// draw the state
		double u = rng->uniform01()*total;
		size_t rateIndex = 0;

		double tmp = 0.0;
		while(tmp < u){
			tmp += 1 - this->perMixtureCorrections[rateIndex];
			if(tmp < u)
				rateIndex++;
		}
		perSiteRates.push_back( rateIndex );
	}

    // then sample the site-pattern, rejecting those that match the unobservable ones
    for ( size_t i = 0; i < this->numSites; i++ )
    {
    	size_t rateIndex = perSiteRates[i];

        std::vector<charType> siteData(numNodes, charType());

        if(rng->uniform01() < pi[1])
        	siteData[rootIndex].setState("1");
        else
        	siteData[rootIndex].setState("0");

		// recursively simulate the sequences
		size_t numLeaves = simulateRestriction( root, siteData, rateIndex );

		if((this->type & NOABSENCESITES) && numLeaves == 0){
			i--;
			continue;
		}else if((this->type & NOPRESENCESITES) && numLeaves == numTips){
			i--;
			continue;
		}else if((this->type & NOSINGLETONGAINS) && numLeaves == 1){
			i--;
			continue;
		}else if((this->type & NOSINGLETONLOSSES) && numLeaves == numTips - 1){
			i--;
			continue;
		}

		// add the taxon data to the character data
		for (size_t t = 0; t < numTips; ++t)
		{
			taxa[t].addCharacter(siteData[t]);
		}
    }

    // add the taxon data to the character data
	for (size_t i = 0; i < this->tau->getValue().getNumberOfTips(); ++i)
	{
		taxa[i].setTaxonName(this->tau->getValue().getNode(i).getName());
		this->value->addTaxonData( taxa[i] );
	}

    // compress the data and initialize internal variables
    this->reInitialized();

    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it)
	{
		(*it) = true;
	}

	// flip the active likelihood pointers
	for (size_t index = 0; index < this->changedNodes.size(); ++index)
	{
		if ( this->changedNodes[index] == false )
		{
			this->activeLikelihood[index] = (this->activeLikelihood[index] == 0 ? 1 : 0);
			this->changedNodes[index] = true;
		}
	}

}

template<class charType, class treeType>
size_t RevBayesCore::PhyloCTMCSiteHomogeneousRestriction<charType, treeType>::simulateRestriction( const TopologyNode &node, std::vector<charType> &data, size_t rateIndex) {

	size_t numLeaves = 0;
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();

    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    charType &parentState = data[ nodeIndex ];

    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);

        // update the transition probability matrix
        this->updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );

        charType &childState = data[ child.getIndex() ];

        unsigned long cp = parentState.getState();

		double *pij = this->transitionProbMatrices[ rateIndex ][cp-1];

		if(rng->uniform01() < pij[1])
			childState.setState("1");
		else
			childState.setState("0");

		if(child.isTip())
		    numLeaves += childState.getStringValue() == "1";
		else
			numLeaves += simulateRestriction( child, data, rateIndex );
	}

    return numLeaves;

}

#endif
