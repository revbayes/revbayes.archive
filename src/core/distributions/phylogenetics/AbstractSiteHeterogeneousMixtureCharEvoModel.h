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



#ifndef AbstractSiteHeterogeneousMixtureCharEvoModel_H
#define AbstractSiteHeterogeneousMixtureCharEvoModel_H

#include "CharacterData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class AbstractSiteHeterogeneousMixtureCharEvoModel : public TypedDistribution<CharacterData<charType> >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<treeType> *t, const std::vector< TypedDagNode<double> *> &sr, bool c, size_t nSites);
        AbstractSiteHeterogeneousMixtureCharEvoModel(const AbstractSiteHeterogeneousMixtureCharEvoModel &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractSiteHeterogeneousMixtureCharEvoModel(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractSiteHeterogeneousMixtureCharEvoModel*                           clone(void) const = 0;                                                                      //!< Create an independent clone
        
        // virtual
        virtual void                                                                    swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        
        // non-virtual
        double                                                                          computeLnProbability(void);
        void                                                                            fireTreeChangeEvent(const TopologyNode &n);                                             //!< The tree has changed and we want to know which part.
        void                                                                            setValue(CharacterData<charType> *v);                                                   //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                            redrawValue(void);
        
    protected:
        // helper method for this and derived classes
        void                                                                            recursivelyFlagNodeDirty(const TopologyNode& n);
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                                    keepSpecialization(DagNode* affecter);
        virtual void                                                                    restoreSpecialization(DagNode *restorer);
        virtual void                                                                    touchSpecialization(DagNode *toucher);
        
        // pure virtual methods
        virtual void                                                                    computeRootLikelihood(size_t left, size_t right) = 0;
        virtual void                                                                    computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right) = 0;
        virtual void                                                                    computeTipLikelihood(const TopologyNode &node) = 0;
        virtual CharacterData<charType>*                                                simulate(const TopologyNode& node) = 0;
        
        // members
        double                                                                          lnProb;
        size_t                                                                          numSites;
        const size_t                                                                    numChars;
        const TypedDagNode<treeType>*                                                   tau;
        std::vector<const TypedDagNode<double> *>                                       siteRates;
        
        std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >     partialLikelihoods;
        std::vector<std::vector<double> >                                               rootLikelihoods;
        std::vector<std::vector< size_t > >                                             activeLikelihood;
        
        std::vector<std::vector<unsigned int> >                                         charMatrix;
        std::vector<std::vector<bool> >                                                 gapMatrix;
        std::vector<size_t>                                                             patternCounts;
        size_t                                                                          numPatterns;
        bool                                                                            compressed;
        
    private:
        // private methods
        void                                                                            fillLikelihoodVector(const TopologyNode &n);
        
        std::vector< std::vector<bool> >                                                changed;
        std::vector<bool>                                                               dirtyNodes;
        std::vector<bool>                                                               dirtyRates;
        
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
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModel(const TypedDagNode<Tree> *t, bool c, size_t nSites) : TypedDistribution<CharacterData<charType> >(  new CharacterData<charType>() ), 
numSites( nSites ), 
numChars( charType().getNumberOfStates() ),
tau( t ), 
siteRates( sr ),
partialLikelihoods( std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >( 2, std::vector<std::vector<std::vector<std::vector<double> > > >(siteRates.size(), std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) ) ) ),
rootLikelihoods( 2, std::vector<double>(numSites, 0.0) ),
activeLikelihood( std::vector< std::vector<size_t> >(siteRates.size(), std::vector<size_t>(tau->getValue().getNumberOfNodes(),0) ) ),
charMatrix(), 
gapMatrix(),
patternCounts(),
numPatterns( numSites ),
compressed( c ),
changedNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(),false) ),
changedRates( std::vector<bool>(siteRates.size(),false) ),
dirtyNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ),
dirtyRates( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ) {
    
    // add the parameters to the parents list
    this->addParameter( tau );
    for (std::vector<const TypedDagNode<double> * >::iterator it = siteRates.begin(); it != siteRates.end(); ++it) {
        this->addParameter( *it );
    }
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModel(const AbstractSiteHeterogeneousMixtureCharEvoModel &n) : TypedDistribution<CharacterData<charType> >( n ), 
numSites( n.numSites ), 
numChars( n.numChars ),
tau( n.tau ), 
siteRates( n.siteRates ),
partialLikelihoods( n.partialLikelihoods ),
rootLikelihoods( n.rootLikelihoods ),
activeLikelihood( n.activeLikelihood ),
charMatrix( n.charMatrix ), 
gapMatrix( n.gapMatrix ), 
patternCounts( n.patternCounts ),
numPatterns( n.numPatterns ),
compressed( n.compressed ),
changedNodes( n.changedNodes ),
changedRates( n.changedRates ),
dirtyNodes( n.dirtyNodes ),
dirtyRates( n.dirtyRates ) {
    // parameters are automatically copied
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::~AbstractSiteHeterogeneousMixtureCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) {
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
    
    size_t numRates = siteRates.size();
    for (size_t index_rates = 0; index_rates < numRates; ++index_rates) {
        
        if ( dirtyRates[index_rates] ) {
            // mark as computed
            dirtyRates[index_rates] = false;
            
            
            // start by filling the likelihood vector for the two children of the root
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, index_rates );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, index_rates );
            
            computeRootLikelihood(leftIndex,rightIndex,index_rates);
        }
    }
    
    if ( dirtyNodes[rootIndex] ) {
        // mark as computed
        dirtyNodes[rootIndex] = false;
        
        
        // start by filling the likelihood vector for the two children of the root
        const TopologyNode &left = root.getChild(0);
        size_t leftIndex = left.getIndex();
        fillLikelihoodVector( left );
        const TopologyNode &right = root.getChild(1);
        size_t rightIndex = right.getIndex();
        fillLikelihoodVector( right );
        
        computeRootLikelihood(leftIndex,rightIndex);
    }
    
    return this->lnProb;
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::fillLikelihoodVector(const TopologyNode &node) {    
    
    // get the index in the node sequence
    size_t nodeIndex = node.getIndex();
    
    // check for recomputation
    if ( dirtyNodes[nodeIndex] ) {
        // mark as computed
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
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n ) {
    recursivelyFlagNodeDirty( n );
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::keepSpecialization( DagNode* affecter ) {
    // flags for nodes
    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it) {
        (*it) = false;
    }
    for (std::vector<bool>::iterator it = this->changedNodes.begin(); it != this->changedNodes.end(); ++it) {
        (*it) = false;
    }
    
    // flags for sites
    for (std::vector<bool>::iterator it = this->dirtyRates.begin(); it != this->dirtyRates.end(); ++it) {
        (*it) = false;
    }
    for (std::vector<bool>::iterator it = this->changedRates.begin(); it != this->changedRates.end(); ++it) {
        (*it) = false;
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    int index = n.getIndex();
    if ( !dirtyNodes[index] ) {
        if ( !n.isRoot() ) {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        dirtyNodes[index] = true;
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( !changedNodes[index] ) {
            size_t numRates = siteRates->getValue().size();
            for ( size_t i = 0; i < numRates; ++i ) {
                // if the active likelihood has been swapped already for this site, then we shouldn't do it again
                if ( !changedRates[i] ) {
                    activeLikelihood[i][index] ^= 1 ;
                }
            }
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
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) {
        (*it) = false;
    }
    for (std::vector<bool>::iterator it = dirtyRates.begin(); it != dirtyRates.end(); ++it) {
        (*it) = false;
    }
    size_t numRates = siteRates->getValue().size();
    size_t numNodes = dirtyNodes.size();
    // first we only flip the ones where the nodes but not the rates have been touched
    for (size_t index_nodes = 0; index_nodes < numNodes; ++index_nodes) {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index_nodes] ) {
            
            for (size_t index_rates = 0; index_rates < numRates; ++index_rates) {
                if ( !changedRates[index_rates] ) {
                    activeLikelihood[index_rates][index_nodes] ^= 1;
                }
            }
            // set all flags to false
            changedNodes[index_nodes] = false;
        }
    }
    
    // then we flip the likelihood pointers where the rates have been touched
    for (size_t index_rates = 0; index_rates < numRates; ++index_rates) {
        if (changedRates[index_rates]) {
            for (size_t index_nodes = 0; index_nodes < numNodes; ++index_nodes) {
                activeLikelihood[index_rates][index_nodes] ^= 1;
            }
            changedRates[index_rates] = false;
        }
    }
    
    
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::setValue(CharacterData<charType> *v) {
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
            unsigned int nodeIndex = (*it)->getIndex();
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
    partialLikelihoods = std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >( 2, std::vector<std::vector<std::vector<std::vector<double> > > >(siteRates.size(), std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numPatterns, std::vector<double>(numChars, 0.0) ) ) ) );
    rootLikelihoods = std::vector< std::vector< double > >(2, std::vector<double>( numPatterns, 0.0 ) );
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
    size_t numRates = siteRates.size();
    for (unsigned int i = 0 ; i < numRates ; i++) {
        if (oldP == siteRates[i]) {
            siteRates[i] = static_cast<const TypedDagNode< double > * >( newP );
        }
        
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    
    // skip if the topology is the toucher
    if ( affecter != tau ) {
        size_t numRates = siteRates.size();
        size_t numNodes = changedNodes.size();
        bool found = false;
        for (unsigned int i = 0 ; i < numRates ; i++) {
            if (affecter == siteRates[i]) {
                found = true;
                dirtyRates[i] = true;
                
                // have we flipped the active likelihood pointer for this rate already?
                if ( !changedRates[index_rates] ) {
                    for (size_t index = 0; index < numNodes; ++index) {
                        // have we flipped the active likelihood pointer for this node already?
                        if ( !changedNodes[index] ) {
                            activeLikelihood[index_rates][index] ^= 1;
                        }
                    }
                    changedRates[index_rates] = true;
                }
                
            }
        }
        
        if ( !found ) {
            // it was neither the topology nor the site rates (maybe the rate matrix ...)
            // for safety we touch all and recompute all conditional likelihoods
            for (std::vector<bool>::iterator it = dirtyRates.begin(); it != dirtyRates.end(); ++it) {
                (*it) = true;
            }
            
            // flip all unflipped active likelihood pointers
            for (size_t index_rates; index_rates < numRates; ++index_rates) {
                // have we flipped the active likelihood pointer for this rate already?
                if ( !changedRates[index_rates] ) {
                    for (size_t index = 0; index < numNodes; ++index) {
                        // have we flipped the active likelihood pointer for this node already?
                        if ( !changedNodes[index] ) {
                            activeLikelihood[index_rates][index] ^= 1;
                        }
                    }
                    changedRates[index_rates] = true;
                }
            }
        }
    }
    
}


#endif
