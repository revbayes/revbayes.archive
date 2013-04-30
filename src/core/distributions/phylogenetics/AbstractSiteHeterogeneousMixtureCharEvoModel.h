/**
 * @file
 * This file contains the declaration of the random variable class for a character state evolving along a tree.
 * This abstract base class can be derived for any character evolution model with HeterogeneousMixture sites or
 * with specifically instantiated per site models. The pruning algorithm is scatched in this base class 
 * and provides hooks for the specific efficient algorithms. The important functions you have to override are:
 * - computeRootLikelihood
 * - computeInternalNodeLikelihood
 * - computeTipLikelihood
 * The data is stored for convenience in this class in a matrix (std::vector<std::vector< unsigned > >) and can
 * be compressed.
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



#ifndef AbstractSiteHeterogeneousMixtureCharEvoModel_H
#define AbstractSiteHeterogeneousMixtureCharEvoModel_H

#include "AbstractCharacterData.h"
#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class AbstractSiteHeterogeneousMixtureCharEvoModel : public TypedDistribution< AbstractCharacterData >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<std::vector<double> > *sr, bool c, size_t nSites);
        AbstractSiteHeterogeneousMixtureCharEvoModel(const AbstractSiteHeterogeneousMixtureCharEvoModel &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractSiteHeterogeneousMixtureCharEvoModel(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractSiteHeterogeneousMixtureCharEvoModel*               clone(void) const = 0;                                                                      //!< Create an independent clone
        
        // virtual
        virtual void                                                        swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        
        // non-virtual
        double                                                              computeLnProbability(void);
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                             //!< The tree has changed and we want to know which part.
        void                                                                setValue(AbstractCharacterData *v);                                                   //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        
    protected:
        // helper method for this and derived classes
        void                                                                computeRootLikelihood(size_t root, const std::vector<size_t> &c);
        void                                                                computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right);
        void                                                                computeTipLikelihood(const TopologyNode &node);
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher);
        
        // pure virtual methods
        virtual void                                                        updateTransitionProbabilities(size_t nodeIdx, double brlen) = 0;
        virtual const std::vector<double>&                                  getRootFrequencies(void) = 0;
        virtual CharacterData<charType>*                                    simulate(const TopologyNode& node) = 0;
        
        // members
        double                                                              lnProb;
        const size_t                                                        numChars;
        size_t                                                              numRates;
        size_t                                                              numSites;
        const TypedDagNode<treeType>*                                       tau;
        const TypedDagNode<std::vector<double> >*                           siteRates;
        std::vector<TransitionProbabilityMatrix>                            transitionProbMatrices;

        // the likelihoods
        std::vector< std::vector< std::vector< std::vector< std::vector< double > > > > >      partialLikelihoods;
        std::vector<size_t>                                                 activeLikelihood;
        
        // the data
        std::vector<std::vector<unsigned int> >                             charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;
        std::vector<size_t>                                                 patternCounts;
        size_t                                                              numPatterns;
        bool                                                                compressed;
        
    private:
        // private methods
        void                                                                fillLikelihoodVector(const TopologyNode &n);
        
        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;
    };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>
#include <map>

template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const TypedDagNode<std::vector<double> > *sr, bool c, size_t nSites) : TypedDistribution< AbstractCharacterData >(  new CharacterData<charType>() ), 
            numSites( nSites ), 
            numChars( charType().getNumberOfStates() ),
            numRates( sr->getValue().size() ),
            tau( t ), 
            siteRates( sr ),
            transitionProbMatrices( std::vector<TransitionProbabilityMatrix>(numRates, TransitionProbabilityMatrix(numChars) ) ),
            partialLikelihoods( std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >( 2, std::vector< std::vector<std::vector<std::vector<double> > > >( tau->getValue().getNumberOfNodes(), std::vector<std::vector<std::vector<double> > >( numSites, std::vector<std::vector<double> >( numRates, std::vector<double>(numChars, 0.0) ) ) ) ) ),
            activeLikelihood( std::vector<size_t>(tau->getValue().getNumberOfNodes(), 0) )      ,
            charMatrix(), 
            gapMatrix(),
            patternCounts(),
            numPatterns( numSites ),
            compressed( c ),
            changedNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(),false) ),
            dirtyNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ) {
    
    // add the parameters to the parents list
    this->addParameter( tau );
    this->addParameter( siteRates );
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModel(const AbstractSiteHeterogeneousMixtureCharEvoModel &n) : TypedDistribution< AbstractCharacterData >( n ), 
            numSites( n.numSites ), 
            numChars( n.numChars ),
            numRates( n.numRates ),
            tau( n.tau ), 
            siteRates( n.siteRates ),
            transitionProbMatrices( n.transitionProbMatrices ),
            partialLikelihoods( n.partialLikelihoods ),
            activeLikelihood( n.activeLikelihood ),
            charMatrix( n.charMatrix ), 
            gapMatrix( n.gapMatrix ), 
            patternCounts( n.patternCounts ),
            numPatterns( n.numPatterns ),
            compressed( n.compressed ),
            changedNodes( n.changedNodes ),
            dirtyNodes( n.dirtyNodes ) {
    // parameters are automatically copied
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::~AbstractSiteHeterogeneousMixtureCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) 
    {
        //        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>* RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::clone( void ) const {
    
    return new AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
double RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeLnProbability( void ) {
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    size_t rootIndex = root.getIndex();
    
    if ( dirtyNodes[rootIndex] ) 
    {
        // mark as computed
        dirtyNodes[rootIndex] = false;
        
        
        // start by filling the likelihood vector for the two children of the root
        std::vector<size_t> childrenIndices;
        for (size_t i = 0; i < root.getNumberOfChildren(); ++i) 
        {
            const TopologyNode &child = root.getChild(i);
            size_t childIndex = child.getIndex();
            fillLikelihoodVector( child );
            childrenIndices.push_back( childIndex );
        }
        
        computeRootLikelihood( rootIndex, childrenIndices );
    }
    
    return this->lnProb;
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeRootLikelihood( size_t root, const std::vector<size_t> &children) {
    
    // reset the likelihood
    this->lnProb = 0.0;
    
    // sum the per site probabilities
    const std::vector<double> &f = getRootFrequencies();
    size_t left = children[0];
    size_t right = children[1];
    
    std::vector< std::vector< std::vector< double > > > &p_left   = this->partialLikelihoods[activeLikelihood[left]][left];
    std::vector< std::vector< std::vector< double > > > &p_right  = this->partialLikelihoods[activeLikelihood[right]][right];
     
    // iterate over all sites
    std::vector< std::vector< std::vector< double > > >::const_iterator   p_site_left     = p_left.begin();
    std::vector< std::vector< std::vector< double > > >::const_iterator   end             = p_left.end();
    std::vector< std::vector< std::vector< double > > >::const_iterator   p_site_right    = p_right.begin();
    std::vector< size_t >::const_iterator patterns = patternCounts.begin();
    for (; p_site_left != end; ++p_site_left, ++p_site_right, ++patterns) 
    {
        double tmp_lnProb_Rate = 0.0;
        
        // iterate over all rates
        std::vector< std::vector< double > >::const_iterator   p_site_rate_left     = p_site_left->begin();
        std::vector< std::vector< double > >::const_iterator   p_site_rate_right    = p_site_right->begin();
        for (size_t rate = 0; rate < numRates; ++rate, ++p_site_rate_left, ++p_site_rate_right)
        {
            
            double tmp = 0.0;
            std::vector<double>::const_iterator f_j             = f.begin();
            std::vector<double>::const_iterator f_end           = f.end();
            std::vector<double>::const_iterator p_site_left_j   = p_site_rate_left->begin();
            std::vector<double>::const_iterator p_site_right_j  = p_site_rate_right->begin();
            for (; f_j != f.end(); ++f_j, ++p_site_left_j, ++p_site_right_j) 
            {
                tmp += *p_site_left_j * *p_site_right_j * *f_j;
            }
            tmp_lnProb_Rate += tmp;
        }
        
        this->lnProb += log(tmp_lnProb_Rate / numRates) * *patterns;
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    // compute the transition probability matrix
//    rateMatrix->getValue().calculateTransitionProbabilities( node.getBranchLength()*rateMultiplier->getValue(), transitionProbabilities);
    //    const TransitionProbabilityMatrix &tpm = computeTransitionProbabilities( nodeIndex, node.getBranchLength()*rateMultiplier->getValue() );
    updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
  
    std::vector< std::vector< std::vector< double > > > &p_left = this->partialLikelihoods[this->activeLikelihood[left]][left];
    std::vector< std::vector< std::vector< double > > > &p_right = this->partialLikelihoods[this->activeLikelihood[right]][right];
    std::vector< std::vector< std::vector< double > > > &p_node = this->partialLikelihoods[this->activeLikelihood[nodeIndex]][nodeIndex];
    
    // compute the per site probabilities
    std::vector< std::vector< std::vector< double > > >::const_iterator   p_site_left     = p_left.begin();
    std::vector< std::vector< std::vector< double > > >::const_iterator   p_site_right    = p_right.begin();
    std::vector< std::vector< std::vector< double > > >::iterator         p_site          = p_node.begin();
    std::vector< std::vector< std::vector< double > > >::iterator         p_end           = p_node.end();
    for (; p_site != p_end; ++p_site, ++p_site_left, ++p_site_right) 
    {
        
        // iterate over all rates
        std::vector< std::vector< double > >::iterator          p_site_rate          = p_site->begin();
        std::vector< std::vector< double > >::const_iterator    p_site_rate_left     = p_site_left->begin();
        std::vector< std::vector< double > >::const_iterator    p_site_rate_right    = p_site_right->begin();
        for (size_t rate = 0; rate < numRates; ++rate, ++p_site_rate_left, ++p_site_rate_right, ++p_site_rate)
        {
            
            const TransitionProbabilityMatrix &tpm = transitionProbMatrices[rate];
            
            // iterate over the possible starting states
            std::vector<double>::iterator                       p_a     = p_site_rate->begin();
            std::vector<std::vector<double> >::const_iterator   tp_a    = tpm.begin();
            std::vector<std::vector<double> >::const_iterator   tp_end  = tpm.end();
            for (; tp_a != tp_end; ++tp_a, ++p_a) 
            {
            
                double sum = 0.0;
            
                // iterate over all possible terminal states
                std::vector<double>::const_iterator p_site_left_d   = p_site_rate_left->begin();
                std::vector<double>::const_iterator p_site_right_d  = p_site_rate_right->begin();
                std::vector<double>::const_iterator tp_a_d          = tp_a->begin();
                std::vector<double>::const_iterator tp_a_end        = tp_a->end();
                for (; tp_a_d != tp_a_end; ++tp_a_d, ++p_site_left_d, ++p_site_right_d ) 
                {
                    sum += *p_site_left_d * *p_site_right_d * *tp_a_d;
                }
                *p_a = sum;
            }
        }
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::computeTipLikelihood(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    std::vector< std::vector< std::vector< double> > > &p_node = this->partialLikelihoods[this->activeLikelihood[nodeIndex]][nodeIndex];
    
    std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    
    std::vector<unsigned int> &char_node = this->charMatrix[nodeIndex];
    
    // compute the transition probabilities
//    rateMatrix->getValue().calculateTransitionProbabilities( node.getBranchLength()*rateMultiplier->getValue(), transitionProbabilities );
//    const TransitionProbabilityMatrix &tpm = computeTransitionProbabilities( nodeIndex, node.getBranchLength()*rateMultiplier->getValue() );
    updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // compute the per site probabilities
    std::vector< std::vector< std::vector< double > > >::iterator p_site      = p_node.begin();
    std::vector< std::vector< std::vector< double > > >::iterator p_end       = p_node.end();
    std::vector<bool>::const_iterator           gap_site    = gap_node.begin();
    std::vector<unsigned int>::const_iterator   char_site   = char_node.begin();
    for (; p_site != p_end; ++p_site, ++char_site, ++gap_site) 
    {
        
        if ( *gap_site == true ) 
        {
            
            // iterate over all rates
            std::vector< std::vector< double > >::iterator          p_site_rate          = p_site->begin();
            for (size_t rate = 0; rate < numRates; ++rate, ++p_site_rate)
            {
                const TransitionProbabilityMatrix &tpm = transitionProbMatrices[rate];
                
                std::vector<double>::iterator                       p_a     = p_site_rate->begin();
                std::vector<std::vector<double> >::const_iterator   tp_a    = tpm.begin();
                std::vector<std::vector<double> >::const_iterator   tp_end  = tpm.end();
                for (; tp_a != tp_end; ++tp_a, ++p_a) 
                {
                
                    double tmp = 0.0;
                
                    std::vector<double>::const_iterator d   = tp_a->begin();
                    std::vector<double>::const_iterator end = tp_a->end();
                    for (; d != end; ++d) 
                    {
                        tmp += *d;
                    }
                    *p_a = tmp;
                }
            } 
        }
        else 
        {
            unsigned int org_val = *char_site;
            
            // iterate over all rates
            std::vector< std::vector< double > >::iterator          p_site_rate          = p_site->begin();
            for (size_t rate = 0; rate < numRates; ++rate, ++p_site_rate)
            {
                const TransitionProbabilityMatrix &tpm = transitionProbMatrices[rate];

                std::vector<double>::iterator                       p_a     = p_site_rate->begin();
                std::vector<std::vector<double> >::const_iterator   tp_a    = tpm.begin();
                std::vector<std::vector<double> >::const_iterator   tp_end  = tpm.end();
                for (; tp_a != tp_end; ++tp_a, ++p_a) 
                {
                
                    double tmp = 0.0;
                
                    unsigned int val = org_val;
                    std::vector<double>::const_iterator d   = tp_a->begin();
                    std::vector<double>::const_iterator end = tp_a->end();
                    for (; d != end; ++d) 
                    {
                        if ( (val & 1) == 1 ) 
                        {
                            tmp += *d;
                        }
                    
                        val >>= 1;
                    }
                    *p_a = tmp;
                }
            }
        }
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::fillLikelihoodVector(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    // check for recomputation
    if ( dirtyNodes[nodeIndex] ) 
    {
        // mark as computed
        dirtyNodes[nodeIndex] = false;
        
        if ( node.isTip() ) 
        {
            
            computeTipLikelihood(node);
        } 
        else 
        {
            
            // start by filling the likelihood vector for the two children of the root
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right );
            
            computeInternalNodeLikelihood(node,leftIndex,rightIndex);
        }
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n ) {
    
    recursivelyFlagNodeDirty( n );
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::keepSpecialization( DagNode* affecter ) {
    
    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it) 
    {
        (*it) = false;
    }
    
    for (std::vector<bool>::iterator it = this->changedNodes.begin(); it != this->changedNodes.end(); ++it) 
    {
        (*it) = false;
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    size_t index = n.getIndex();
    if ( !dirtyNodes[index] ) 
    {
        
        if ( !n.isRoot() ) 
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        
        dirtyNodes[index] = true;
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( !changedNodes[index] ) 
        {
            activeLikelihood[index] ^= 1;
            changedNodes[index] = true;
        }
        
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::redrawValue( void ) {
    
    delete this->value;
    this->value = this->simulate(tau->getValue().getRoot());
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::restoreSpecialization( DagNode* affecter ) {
    
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) 
    {
        (*it) = false;
    }
    
    for (size_t index = 0; index < changedNodes.size(); ++index) 
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index] ) 
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }
        
        // set all flags to false
        changedNodes[index] = false;
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::setValue(AbstractCharacterData *v) {
    // delegate to the parent class
    TypedDistribution< AbstractCharacterData >::setValue(v);
    
    charMatrix.clear();
    gapMatrix.clear();
    patternCounts.clear();
    numPatterns = 0;
    
    // resize the matrices
    size_t tips = tau->getValue().getNumberOfTips();
    charMatrix.resize(tips);
    gapMatrix.resize(tips);
    
    std::vector<bool> unique(numSites, true);
    // compress the character matrix if we're asked to
    if ( compressed ) 
    {
        // find the unique site patterns and compute their respective frequencies
        std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
        std::map<std::string,size_t> patterns;
        for (size_t site = 0; site < numSites; ++site) 
        {
            // create the site pattern
            std::string pattern = "";
            for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
            {
                if ( (*it)->isTip() ) 
                {
                    AbstractTaxonData& taxon = v->getTaxonData( (*it)->getName() );
                    CharacterState &c = taxon.getCharacter(site);
                    pattern += c.getStringValue();
                }
            }
            // check if we have already seen this site pattern
            std::map<std::string, size_t>::const_iterator index = patterns.find( pattern );
            if ( index != patterns.end() ) 
            {
                // we have already seen this pattern
                // increase the frequency counter
                patternCounts[ index->second ]++;
                
                // obviously this site isn't unique nor the first encounter
                unique[site] = false;
            }
            else 
            {
                // create a new pattern frequency counter for this pattern
                patternCounts.push_back(1);
                
                // insert this pattern with the corresponding index in the map
                patterns.insert( std::pair<std::string,size_t>(pattern,numPatterns) );
                
                // increase the pattern counter
                numPatterns++;
                
                // flag that this site is unique (or the first occurence of this pattern)
                unique[site] = true;
            }
        }
    } 
    else 
    {
        // we do not compress
        numPatterns = numSites;
        patternCounts = std::vector<size_t>(numSites,1);
    }
    
    
    // allocate and fill the cells of the matrices
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        if ( (*it)->isTip() ) 
        {
            size_t nodeIndex = (*it)->getIndex();
            AbstractTaxonData& taxon = v->getTaxonData( (*it)->getName() );
            
            // resize the column
            charMatrix[nodeIndex].resize(numPatterns);
            gapMatrix[nodeIndex].resize(numPatterns);
            size_t patternIndex = 0;
            for (size_t site = 0; site < numSites; ++site) 
            {
                // only add this site if it is unique
                if ( unique[site] ) 
                {
                    charType &c = static_cast<charType &>( taxon.getCharacter(site) );
                    charMatrix[nodeIndex][patternIndex] = c.getState();
                    gapMatrix[nodeIndex][patternIndex] = c.isGapState();
                    
                    // increase the pattern index
                    patternIndex++;
                }
            }
        }
    }
    
    // finally we resize the partial likelihood vectors to the new pattern counts
    partialLikelihoods = std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >( 2, std::vector<std::vector<std::vector<std::vector<double> > > >( tau->getValue().getNumberOfNodes(), std::vector<std::vector<std::vector<double> > >( numPatterns, std::vector<std::vector<double> >( numRates, std::vector<double>(numChars, 0.0) ) ) ) );
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == tau) 
    {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
    else if ( oldP == siteRates )
    {
        siteRates = static_cast< const TypedDagNode< std::vector< double > > * >( newP );
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    if ( affecter != tau ) 
    {
        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) 
        {
            (*it) = true;
        }
        
        for (size_t index = 0; index < changedNodes.size(); ++index) 
        {
            if ( !changedNodes[index] ) 
            {
                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                changedNodes[index] = true;
            }
        }
    }
    
}


#endif
