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



#ifndef AbstractHeterogeneousCharEvoModel_H
#define AbstractHeterogeneousCharEvoModel_H

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
    class AbstractHeterogeneousCharEvoModel : public TypedDistribution<CharacterData<charType> >, public TreeChangeEventListener {
        
    public:
        AbstractHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, size_t nSites);
        AbstractHeterogeneousCharEvoModel(const AbstractHeterogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                                    ~AbstractHeterogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        virtual AbstractHeterogeneousCharEvoModel*                          clone(void) const = 0;                                                                          //!< Create an independent clone
 
        // virtual
        virtual void                                                        swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters

        // non-virtual
        double                                                              computeLnProbability(void);
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                             //!< The tree has changed and we want to know which part.
        void                                                                setValue(CharacterData<charType> *v);                                                   //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        
    protected:
        // helper method for this and derived classes
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher);
        
        // pure virtual methods
        virtual void                                                        computeRootLikelihood(size_t left, size_t right) ;
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right);
        virtual void                                                        computeTipLikelihood(const TopologyNode &node);
        
        virtual void                                                        computeRootLikelihood(size_t left, size_t right, size_t site);
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right, size_t site);
        virtual void                                                        computeTipLikelihood(const TopologyNode &node, size_t site);
        
        virtual const std::vector<TransitionProbabilityMatrix *>&           computeTransitionProbabilities(const TopologyNode &node) = 0;
        virtual const TransitionProbabilityMatrix&                          computeTransitionProbabilities(const TopologyNode &node, size_t site) = 0;
        virtual const std::vector< std::vector< double > *>&                getRootFrequencies(void) = 0;
        virtual const std::vector< double >&                                getRootFrequencies(size_t site) = 0;
        virtual CharacterData<charType>*                                    simulate(const TopologyNode& node) = 0;
        
        // members
        double                                                              lnProb;
        size_t                                                              numSites;
        const size_t                                                        numChars;
        const TypedDagNode<treeType>*                                       tau;
        
        std::vector<std::vector<std::vector<std::vector<double> > > >       partialLikelihoods;
        std::vector< std::vector<size_t> >                                  activeLikelihoodForNodes;
                
        std::vector<std::vector<unsigned int> >                             charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;

        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;
        std::vector< std::set<size_t> >                                     dirtySitesForNodes;
        std::vector< std::set<size_t> >                                     changedSitesForNodes;
        
    private:
        // private methods
        void                                                                fillLikelihoodVector(const TopologyNode &n);
        void                                                                fillLikelihoodVector(const TopologyNode &n, size_t site);
             
        bool                                                                french;
   };
    
}


#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>


template<class charType, class treeType>
RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::AbstractHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, size_t nSites) : TypedDistribution<CharacterData<charType> >(  new CharacterData<charType>() ), 
        numSites( nSites ), 
        numChars( 4 ),
        tau( t ), 
        partialLikelihoods( std::vector<std::vector<std::vector<std::vector<double> > > >( 2, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) ) ),
        activeLikelihoodForNodes( tau->getValue().getNumberOfNodes(), std::vector<size_t>(numSites, 0) ),
        charMatrix(), 
        gapMatrix(),
changedNodes( tau->getValue().getNumberOfNodes(), false ) {
    // add the parameters to the parents list
    this->addParameter( tau );
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
    
}


template<class charType, class treeType>
RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::AbstractHeterogeneousCharEvoModel(const AbstractHeterogeneousCharEvoModel &n) : TypedDistribution<CharacterData<charType> >( n ), 
numSites( n.numSites ), 
numChars( n.numChars ),
tau( n.tau ), 
partialLikelihoods( n.partialLikelihoods ),
activeLikelihoodForNodes( n.activeLikelihoodForNodes ),
charMatrix( n.charMatrix ), 
gapMatrix( n.gapMatrix ) {
    // parameters are automatically copied
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
}


template<class charType, class treeType>
RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::~AbstractHeterogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) {
        //        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }
}


template<class charType, class treeType>
RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>* RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    return new AbstractHeterogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
double RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeLnProbability( void ) {
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    size_t rootIndex = root.getIndex();
    
    if ( dirtyNodes[rootIndex] ) {
        dirtyNodes[rootIndex] = false;
        // we need to compute for all sites
        
        // start by filling the likelihood vector for the two children of the root
        const TopologyNode &left = root.getChild(0);
        size_t leftIndex = left.getIndex();
        fillLikelihoodVector( left );
        const TopologyNode &right = root.getChild(1);
        size_t rightIndex = right.getIndex();
        fillLikelihoodVector( right );
        
        computeRootLikelihood(leftIndex,rightIndex);
        
    } 
    
    
    if ( dirtySitesForNodes[rootIndex].size() > 0) {
        // it's more complex
        
        std::set<size_t>::const_iterator it_sEnd = dirtySitesForNodes[rootIndex].end();
        for (std::set<size_t>::const_iterator it_sites = dirtySitesForNodes[rootIndex].begin(); it_sites != it_sEnd; ++it_sites) {
            // start by filling the likelihood vector for the two children of the root
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, *it_sites );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, *it_sites );
            
            computeRootLikelihood(leftIndex,rightIndex, *it_sites);
        }
    } 
    
    return this->lnProb;
}



template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::fillLikelihoodVector(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    if ( dirtyNodes[nodeIndex] ) {
        dirtyNodes[nodeIndex] = false;
            
        if ( node.isTip() ) {
                
            computeTipLikelihood(node);
        } else {
                
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
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::fillLikelihoodVector(const TopologyNode &node, size_t site) {    
        
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
        
        
    // check for recomputation
    std::set<size_t>& sitesForThisNode = dirtySitesForNodes[nodeIndex];
    std::set<size_t>::iterator it = sitesForThisNode.find( site );
    if ( it != dirtySitesForNodes[nodeIndex].end() ) {
        // mark as computed
        sitesForThisNode.erase( it );
            
        if ( node.isTip() ) {
                
            computeTipLikelihood(node,site);
        } else {
                
            // start by filling the likelihood vector for the two children of the root
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, site );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, site );
                
            computeInternalNodeLikelihood(node,leftIndex,rightIndex);
        }
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n ) {
    recursivelyFlagNodeDirty( n );
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::keepSpecialization( DagNode* affecter ) {
    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it) {
        (*it) = false;
    }
    for (std::vector<bool>::iterator it = this->changedNodes.begin(); it != this->changedNodes.end(); ++it) {
        (*it) = false;
    }
    for (std::vector< std::set<size_t> >::iterator it = this->changedSitesForNodes.begin(); it != this->changedSitesForNodes.end(); ++it) {
        (*it).clear();
    }
    for (std::vector< std::set<size_t> >::iterator it = this->dirtySitesForNodes.begin(); it != this->dirtySitesForNodes.end(); ++it) {
        (*it).clear();
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    size_t index = n.getIndex();
    if ( !dirtyNodes[index] ) {
        if ( !n.isRoot() ) {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        dirtyNodes[index] = true;
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( !changedNodes[index] ) {
            std::vector<size_t>& activeLikelihoodForThisNode = activeLikelihoodForNodes[index];
            size_t index_site = 0;
            for (std::vector<size_t>::iterator it = activeLikelihoodForThisNode.begin(); it != activeLikelihoodForThisNode.end(); ++it, ++index_site) {
                if ( changedSitesForNodes[index].find( index_site ) != changedSitesForNodes[index].end() ) {
                    // XOR
                    (*it) ^= 1;
                }
            }
            changedNodes[index] = true;
        }
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::redrawValue( void ) {
    delete this->value;
    this->value = this->simulate(tau->getValue().getRoot());
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::restoreSpecialization( DagNode* affecter ) {
    
    
    std::vector< std::set<size_t> >::iterator it_nodes = changedSitesForNodes.begin(); 
    std::vector< std::set<size_t> >::iterator it_end = changedSitesForNodes.end();
    size_t index_node = 0;
    for ( ; it_nodes != it_end; ++it_nodes, ++index_node) {
        if ( !changedNodes[ index_node ] ) {
            std::set< size_t >::const_iterator it_site = it_nodes->begin();
            std::set< size_t >::const_iterator it_siteEnd = it_nodes->end();
            for (; it_site != it_siteEnd; ++it_site) {
                activeLikelihoodForNodes[index_node][*it_site] ^= 1;
            }
        }
    }
    
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) {
        (*it) = false;
    }
    for (size_t index = 0; index < changedNodes.size(); ++index) {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index] ) {
            std::vector<size_t>& activeLikelihoodForThisNode = activeLikelihoodForNodes[index];
            for (std::vector<size_t>::iterator it = activeLikelihoodForThisNode.begin(); it != activeLikelihoodForThisNode.end(); ++it) {
                // XOR
                (*it) ^= 1;
            }
            
            // set all flags to false
            changedNodes[index] = false;
        }
    }
    
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::setValue(CharacterData<charType> *v) {
    // delegate to the parent class
    TypedDistribution<CharacterData<charType> >::setValue(v);
    
    charMatrix.clear();
    gapMatrix.clear();
    
    // resize the matrices
    size_t tips = tau->getValue().getNumberOfTips();
    charMatrix.resize(tips);
    gapMatrix.resize(tips);
    
    
    // allocate and fill the cells of the matrices
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        if ( (*it)->isTip() ) {
            size_t nodeIndex = (*it)->getIndex();
            TaxonData<charType>& taxon = v->getTaxonData( (*it)->getName() );
            
            // resize the column
            charMatrix[nodeIndex].resize(numSites);
            gapMatrix[nodeIndex].resize(numSites);
            for (size_t site = 0; site < numSites; ++site) {
                charType &c = taxon.getCharacter(site);
                charMatrix[nodeIndex][site] = c.getState();
                gapMatrix[nodeIndex][site] = c.isGapState();

            }
        }
    }
    
    // finally we resize the partial likelihood vectors to the new pattern counts
    partialLikelihoods = std::vector<std::vector<std::vector<std::vector<double> > > >( 2, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) );
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode<TimeTree>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    if ( affecter != tau ) {
        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) {
            (*it) = true;
        }
        
        for (size_t index = 0; index < changedNodes.size(); ++index) {
            if ( !changedNodes[index] ) {
                for (size_t siteIndex = 0; siteIndex < numSites; ++siteIndex) {
                    if ( changedSitesForNodes[index].find( siteIndex ) == changedSitesForNodes[index].end()) {
                        activeLikelihoodForNodes[index][siteIndex] ^= 1;
                    }
                }
                changedNodes[index] = true;
            }
        }
        
        
    }
    
}









//////////////////////////////////////////////////////



template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    const std::vector<TransitionProbabilityMatrix *> &tpm = computeTransitionProbabilities(node);
    std::vector< TransitionProbabilityMatrix *>::const_iterator transitionProbabilities = tpm.begin();
    
    for (size_t siteIndex = 0; siteIndex < numSites; ++numSites, ++transitionProbabilities) {
        
        std::vector<double> &p_left = this->partialLikelihoods[this->activeLikelihoodForNodes[left][siteIndex]][left][siteIndex];
        std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihoodForNodes[right][siteIndex]][right][siteIndex];
        std::vector<double> &p_node = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][siteIndex]][nodeIndex][siteIndex];
        
        // iterate over the possible starting states
        std::vector<double>::iterator                       p_a     = p_node.begin();
        std::vector<std::vector<double> >::const_iterator   tp_a    = (*transitionProbabilities)->begin();
        std::vector<std::vector<double> >::const_iterator   tp_end  = (*transitionProbabilities)->end();
        for (; tp_a != tp_end; ++tp_a, ++p_a) {
            
            double sum = 0.0;
            
            // iterate over all possible terminal states
            std::vector<double>::const_iterator p_site_left_d   = p_left.begin();
            std::vector<double>::const_iterator p_site_right_d  = p_right.begin();
            std::vector<double>::const_iterator tp_a_d          = tp_a->begin();
            std::vector<double>::const_iterator tp_a_end        = tp_a->end();
            for (; tp_a_d != tp_a_end; ++tp_a_d, ++p_site_left_d, ++p_site_right_d ) {
                sum += *p_site_left_d * *p_site_right_d * *tp_a_d;
            }
            *p_a = sum;
            
        }
    }
}




template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right, size_t site) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    const TransitionProbabilityMatrix &tpm = computeTransitionProbabilities(node,site);
    
    
    std::vector<double> &p_left = this->partialLikelihoods[this->activeLikelihoodForNodes[left][site]][left][site];
    std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihoodForNodes[right][site]][right][site];
    std::vector<double> &p_node = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][site]][nodeIndex][site];
        
    // iterate over the possible starting states
    std::vector<double>::iterator                       p_a     = p_node.begin();
    std::vector<std::vector<double> >::const_iterator   tp_a    = tpm.begin();
    std::vector<std::vector<double> >::const_iterator   tp_end  = tpm.end();
    for (; tp_a != tp_end; ++tp_a, ++p_a) {
            
        double sum = 0.0;
            
        // iterate over all possible terminal states
        std::vector<double>::const_iterator p_site_left_d   = p_left.begin();
        std::vector<double>::const_iterator p_site_right_d  = p_right.begin();
        std::vector<double>::const_iterator tp_a_d          = tp_a->begin();
        std::vector<double>::const_iterator tp_a_end        = tp_a->end();
        for (; tp_a_d != tp_a_end; ++tp_a_d, ++p_site_left_d, ++p_site_right_d ) {
            sum += *p_site_left_d * *p_site_right_d * *tp_a_d;
        }
        *p_a = sum;
        
    }
    
}





template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeRootLikelihood( size_t left, size_t right ) {
    
    // get the index in the node sequence
    size_t nodeIndex = tau->getValue().getRoot().getIndex();
    
    // sum the per site probabilities
    const std::vector< std::vector<double>* > &rf = getRootFrequencies();
    
    this->lnProb = 0.0;
    
    std::vector< std::vector<double>* >::const_iterator it_rf = rf.begin();
    for (size_t siteIndex = 0; siteIndex < numSites; ++numSites, ++it_rf) {
        
        std::vector<double> &p_left = this->partialLikelihoods[this->activeLikelihoodForNodes[left][siteIndex]][left][siteIndex];
        std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihoodForNodes[right][siteIndex]][right][siteIndex];
        std::vector<double> &p_node = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][siteIndex]][nodeIndex][siteIndex];
        
        const std::vector<double> *f = *it_rf;
            
        double tmp = 0.0;
        std::vector<double>::const_iterator f_j             = f->begin();
        std::vector<double>::const_iterator f_end           = f->end();
        std::vector<double>::const_iterator p_site_left_j   = p_left.begin();
        std::vector<double>::const_iterator p_site_right_j  = p_right.begin();
        for (; f_j != f_end; ++f_j, ++p_site_left_j, ++p_site_right_j) {
            tmp += *p_site_left_j * *p_site_right_j * *f_j;
        }
        p_node[0] = log(tmp);
        this->lnProb += p_node[0];
        
    }
}





template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeRootLikelihood( size_t left, size_t right, size_t site ) {
    
    // get the index in the node sequence
    size_t nodeIndex = tau->getValue().getRoot().getIndex();
    
    // sum the per site probabilities
    const std::vector<double> &rf = getRootFrequencies(site);
    
//    this->lnProb = 0.0;
         
    std::vector<double> &p_left = this->partialLikelihoods[this->activeLikelihoodForNodes[left][site]][left][site];
    std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihoodForNodes[right][site]][right][site];
    std::vector<double> &p_node = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][site]][nodeIndex][site];
        
    double tmp = 0.0;
    std::vector<double>::const_iterator f_j             = rf.begin();
    std::vector<double>::const_iterator f_end           = rf.end();
    std::vector<double>::const_iterator p_site_left_j   = p_left.begin();
    std::vector<double>::const_iterator p_site_right_j  = p_right.begin();
    for (; f_j != f_end; ++f_j, ++p_site_left_j, ++p_site_right_j) {
        tmp += *p_site_left_j * *p_site_right_j * *f_j;
    }
    this->lnProb -= p_node[0]; ////////
    p_node[0] = log(tmp);
    this->lnProb += p_node[0];
        
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeTipLikelihood(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    const std::vector<TransitionProbabilityMatrix *> &tpm = computeTransitionProbabilities(node);
    
    std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    
    std::vector<unsigned int> &char_node = this->charMatrix[nodeIndex];
    
    // compute the per site probabilities
    std::vector<bool>::const_iterator           gap_site    = gap_node.begin();
    std::vector<unsigned int>::const_iterator   char_site   = char_node.begin();
    std::vector< TransitionProbabilityMatrix *>::const_iterator transitionProbabilities = tpm.begin();
    for (size_t siteIndex = 0; siteIndex < numSites; ++siteIndex, ++char_site, ++gap_site, ++transitionProbabilities) {
        
        std::vector<double> &p_site = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][siteIndex]][nodeIndex][siteIndex];
        
        if ( *gap_site == true ) {
            std::vector<double>::iterator                       p_a     = p_site.begin();
            std::vector<std::vector<double> >::const_iterator   tp_a    = (*transitionProbabilities)->begin();
            std::vector<std::vector<double> >::const_iterator   tp_end  = (*transitionProbabilities)->end();
            for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
                double tmp = 0.0;
                
                std::vector<double>::const_iterator d   = tp_a->begin();
                std::vector<double>::const_iterator end = tp_a->end();
                for (; d != end; ++d) {
                    tmp += *d;
                }
                *p_a = tmp;
            } 
        }
        else {
            unsigned int org_val = *char_site;
            std::vector<double>::iterator                       p_a     = p_site.begin();
            std::vector<std::vector<double> >::const_iterator   tp_a    = (*transitionProbabilities)->begin();
            std::vector<std::vector<double> >::const_iterator   tp_end  = (*transitionProbabilities)->end();
            for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
                double tmp = 0.0;
                
                unsigned int val = org_val;
                std::vector<double>::const_iterator d   = tp_a->begin();
                std::vector<double>::const_iterator end = tp_a->end();
                for (; d != end; ++d) {
                    if ( (val & 1) == 1 ) {
                        tmp += *d;
                    }
                    
                    val >>= 1;
                }
                *p_a = tmp;
            }
        }
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractHeterogeneousCharEvoModel<charType, treeType>::computeTipLikelihood(const TopologyNode &node, size_t site) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    const std::vector<TransitionProbabilityMatrix *> &tpm = computeTransitionProbabilities(node);
    
    std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    
    std::vector<unsigned int> &char_node = this->charMatrix[nodeIndex];
    
    // compute the per site probabilities
    std::vector<bool>::const_iterator           gap_site    = gap_node.begin();
    std::vector<unsigned int>::const_iterator   char_site   = char_node.begin();
    std::vector< TransitionProbabilityMatrix *>::const_iterator transitionProbabilities = tpm.begin();
        
    std::vector<double> &p_site = this->partialLikelihoods[this->activeLikelihoodForNodes[nodeIndex][site]][nodeIndex][site];
        
    if ( *gap_site == true ) {
        std::vector<double>::iterator                       p_a     = p_site.begin();
        std::vector<std::vector<double> >::const_iterator   tp_a    = (*transitionProbabilities)->begin();
        std::vector<std::vector<double> >::const_iterator   tp_end  = (*transitionProbabilities)->end();
        for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
            double tmp = 0.0;
                
            std::vector<double>::const_iterator d   = tp_a->begin();
            std::vector<double>::const_iterator end = tp_a->end();
            for (; d != end; ++d) {
                tmp += *d;
            }
            *p_a = tmp;
        } 
    }
    else {
        unsigned int org_val = *char_site;
        std::vector<double>::iterator                       p_a     = p_site.begin();
        std::vector<std::vector<double> >::const_iterator   tp_a    = (*transitionProbabilities)->begin();
        std::vector<std::vector<double> >::const_iterator   tp_end  = (*transitionProbabilities)->end();
        for (; tp_a != tp_end; ++tp_a, ++p_a) {
                
            double tmp = 0.0;
                
            unsigned int val = org_val;
            std::vector<double>::const_iterator d   = tp_a->begin();
            std::vector<double>::const_iterator end = tp_a->end();
            for (; d != end; ++d) {
                if ( (val & 1) == 1 ) {
                    tmp += *d;
                }
                    
                val >>= 1;
            }
            *p_a = tmp;
        }
    }
    
    
}


#endif
