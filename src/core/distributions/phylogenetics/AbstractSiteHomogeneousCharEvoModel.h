/**
 * @file
 * This file contains the declaration of the random variable class for a character state evolving along a tree.
 * This abstract base class can be derived for any character evolution model with homogeneous sites or
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



#ifndef AbstractSiteHomogeneousCharEvoModel_H
#define AbstractSiteHomogeneousCharEvoModel_H

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
    class AbstractSiteHomogeneousCharEvoModel : public TypedDistribution<CharacterData<charType> >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractSiteHomogeneousCharEvoModel(const TypedDagNode<treeType> *t, bool c, size_t nSites);
        AbstractSiteHomogeneousCharEvoModel(const AbstractSiteHomogeneousCharEvoModel &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractSiteHomogeneousCharEvoModel(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractSiteHomogeneousCharEvoModel*                        clone(void) const = 0;                                                                      //!< Create an independent clone
        
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
        virtual void                                                        computeRootLikelihood(const std::vector<size_t> &c) = 0;
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right) = 0;
        virtual void                                                        computeTipLikelihood(const TopologyNode &node) = 0;
        virtual CharacterData<charType>*                                    simulate(const TopologyNode& node) = 0;
        
        // members
        double                                                              lnProb;
        size_t                                                              numSites;
        const size_t                                                        numChars;
        const TypedDagNode<treeType>*                                       tau;
        
        std::vector<std::vector<std::vector<std::vector<double> > > >       partialLikelihoods;
        std::vector<size_t>                                                 activeLikelihood;
        
        std::vector<std::vector<unsigned int> >                             charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;
        std::vector<size_t>                                                 patternCounts;
        size_t                                                              numPatterns;
        bool                                                                compressed;
        
    private:
        // private methods
        void                                                                fillLikelihoodVector(const TopologyNode &n);
        
        std::vector<bool>                                                   changed;
        std::vector<bool>                                                   dirty;
                
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
RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::AbstractSiteHomogeneousCharEvoModel(const TypedDagNode<treeType> *t, bool c, size_t nSites) : TypedDistribution<CharacterData<charType> >(  new CharacterData<charType>() ), 
        numSites( nSites ), 
        numChars( charType().getNumberOfStates() ),
        tau( t ), 
        partialLikelihoods( std::vector<std::vector<std::vector<std::vector<double> > > >( 2, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) ) ),
        activeLikelihood( std::vector<size_t>(tau->getValue().getNumberOfNodes(), 0) )      ,
        charMatrix(), 
        gapMatrix(),
        patternCounts(),
        numPatterns( numSites ),
        compressed( c ),
        changed( std::vector<bool>(tau->getValue().getNumberOfNodes(),false) ),
        dirty( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ) {
            
    // add the parameters to the parents list
    this->addParameter( tau );
    tau->getValue().getTreeChangeEventHandler().addListener( this );
        
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::AbstractSiteHomogeneousCharEvoModel(const AbstractSiteHomogeneousCharEvoModel &n) : TypedDistribution<CharacterData<charType> >( n ), 
        numSites( n.numSites ), 
        numChars( n.numChars ),
        tau( n.tau ), 
        partialLikelihoods( n.partialLikelihoods ),
        activeLikelihood( n.activeLikelihood ),
        charMatrix( n.charMatrix ), 
        gapMatrix( n.gapMatrix ), 
        patternCounts( n.patternCounts ),
        numPatterns( n.numPatterns ),
        compressed( n.compressed ),
        changed( n.changed ),
        dirty( n.dirty ) {
    // parameters are automatically copied
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::~AbstractSiteHomogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) {
        //        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>* RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    return new AbstractSiteHomogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
double RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::computeLnProbability( void ) {
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    size_t rootIndex = root.getIndex();
    
    if ( dirty[rootIndex] ) {
        // mark as computed
        dirty[rootIndex] = false;
        
        
        // start by filling the likelihood vector for the two children of the root
        std::vector<size_t> childrenIndices;
        for (size_t i = 0; i < root.getNumberOfChildren(); ++i) {
            const TopologyNode &child = root.getChild(i);
            size_t childIndex = child.getIndex();
            fillLikelihoodVector( child );
            childrenIndices.push_back( childIndex );
        }
        
        computeRootLikelihood( childrenIndices );
    }
    
    return this->lnProb;
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::fillLikelihoodVector(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    // check for recomputation
    if ( dirty[nodeIndex] ) {
        // mark as computed
        dirty[nodeIndex] = false;
        
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
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n ) {
    recursivelyFlagNodeDirty( n );
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::keepSpecialization( DagNode* affecter ) {
    for (std::vector<bool>::iterator it = this->dirty.begin(); it != this->dirty.end(); ++it) {
        (*it) = false;
    }
    for (std::vector<bool>::iterator it = this->changed.begin(); it != this->changed.end(); ++it) {
        (*it) = false;
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    size_t index = n.getIndex();
    if ( !dirty[index] ) {
        if ( !n.isRoot() ) {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        dirty[index] = true;
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( !changed[index] ) {
            activeLikelihood[index] ^= 1;
            changed[index] = true;
        }
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::redrawValue( void ) {
    delete this->value;
    this->value = this->simulate(tau->getValue().getRoot());
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::restoreSpecialization( DagNode* affecter ) {
    for (std::vector<bool>::iterator it = dirty.begin(); it != dirty.end(); ++it) {
        (*it) = false;
    }
    for (size_t index = 0; index < changed.size(); ++index) {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changed[index] ) {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }
        // set all flags to false
        changed[index] = false;
    }
    
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::setValue(CharacterData<charType> *v) {
    // delegate to the parent class
    TypedDistribution<CharacterData<charType> >::setValue(v);
    
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
    if ( compressed ) {
        // find the unique site patterns and compute their respective frequencies
        std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
        std::map<std::string,size_t> patterns;
        for (size_t site = 0; site < numSites; ++site) {
            // create the site pattern
            std::string pattern = "";
            for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
                if ( (*it)->isTip() ) {
                    TaxonData<charType>& taxon = v->getTaxonData( (*it)->getName() );
                    charType &c = taxon.getCharacter(site);
                    pattern += c.getStringValue();
                }
            }
            // check if we have already seen this site pattern
            std::map<std::string, size_t>::const_iterator index = patterns.find( pattern );
            if ( index != patterns.end() ) {
                // we have already seen this pattern
                // increase the frequency counter
                patternCounts[ index->second ]++;
            
                // obviously this site isn't unique nor the first encounter
                unique[site] = false;
            }
            else {
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
    else {
        // we do not compress
        numPatterns = numSites;
        patternCounts = std::vector<size_t>(numSites,1);
    }

    
    // allocate and fill the cells of the matrices
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        if ( (*it)->isTip() ) {
            size_t nodeIndex = (*it)->getIndex();
            TaxonData<charType>& taxon = v->getTaxonData( (*it)->getName() );
            
            // resize the column
            charMatrix[nodeIndex].resize(numPatterns);
            gapMatrix[nodeIndex].resize(numPatterns);
            size_t patternIndex = 0;
            for (size_t site = 0; site < numSites; ++site) {
                // only add this site if it is unique
                if ( unique[site] ) {
                    charType &c = taxon.getCharacter(site);
                    charMatrix[nodeIndex][patternIndex] = c.getState();
                    gapMatrix[nodeIndex][patternIndex] = c.isGapState();
                    
                    // increase the pattern index
                    patternIndex++;
                }
            }
        }
    }
    
    // finally we resize the partial likelihood vectors to the new pattern counts
    partialLikelihoods = std::vector<std::vector<std::vector<std::vector<double> > > >( 2, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numPatterns, std::vector<double>(numChars, 0.0) ) ) );

}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHomogeneousCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    if ( affecter != tau ) {
        for (std::vector<bool>::iterator it = dirty.begin(); it != dirty.end(); ++it) {
            (*it) = true;
        }
        
        for (size_t index = 0; index < changed.size(); ++index) {
            if ( !changed[index] ) {
                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                changed[index] = true;
            }
        }
    }
    
}


#endif
