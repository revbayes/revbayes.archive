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



#ifndef AbstractSiteHeterogeneousMixtureCharEvoModelSlow_H
#define AbstractSiteHeterogeneousMixtureCharEvoModelSlow_H

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
    class AbstractSiteHeterogeneousMixtureCharEvoModelSlow : public TypedDistribution<CharacterData<charType> >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractSiteHeterogeneousMixtureCharEvoModelSlow(const TypedDagNode<treeType> *t, const std::vector<const TypedDagNode<double> *> &sr, bool c, size_t nSites);
        AbstractSiteHeterogeneousMixtureCharEvoModelSlow(const AbstractSiteHeterogeneousMixtureCharEvoModelSlow &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractSiteHeterogeneousMixtureCharEvoModelSlow(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractSiteHeterogeneousMixtureCharEvoModelSlow*                       clone(void) const = 0;                                                                      //!< Create an independent clone
        
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
        virtual void                                                                    computeRootLikelihood(size_t root, size_t left, size_t right, size_t rate) = 0;
        virtual void                                                                    computeInternalNodeLikelihood(const TopologyNode &node, size_t left, size_t right, size_t rate) = 0;
        virtual void                                                                    computeTipLikelihood(const TopologyNode &node, size_t rate) = 0;
        virtual CharacterData<charType>*                                                simulate(const TopologyNode& node) = 0;
        
        // members
        double                                                                          lnProb;
        size_t                                                                          numSites;
        const size_t                                                                    numChars;
        size_t                                                                          numRates;
        const TypedDagNode<treeType>*                                                   tau;
        std::vector<const TypedDagNode<double> *>                                       siteRates;
        
        std::vector<std::vector<std::vector<std::vector<double> > > >                   partialLikelihoods;
        std::vector<std::vector<std::vector<std::vector<double> > > >                   storedPartialLikelihoods;
        std::vector<std::vector<double> >                                               rootLikelihoods;
        std::vector<std::vector<double> >                                               storedRootLikelihoods;
        
        std::vector<std::vector<unsigned int> >                                         charMatrix;
        std::vector<std::vector<bool> >                                                 gapMatrix;
        std::vector<size_t>                                                             patternCounts;
        size_t                                                                          numPatterns;
        bool                                                                            compressed;
        
    private:
        // private methods
        void                                                                            fillLikelihoodVector(const TopologyNode &n, size_t s);
        void                                                                            sumRootLikelihood(void);

        std::vector< bool >                                                             dirtyNodes;
        std::vector< std::vector<bool> >                                                dirty;
        
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
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModelSlow(const TypedDagNode<treeType> *t, const std::vector<const TypedDagNode<double> *> &sr, bool c, size_t nSites) : TypedDistribution<CharacterData<charType> >(  new CharacterData<charType>() ), 
numSites( nSites ), 
numChars( charType().getNumberOfStates() ),
numRates( sr.size() ),
tau( t ), 
siteRates( sr ),
partialLikelihoods( std::vector<std::vector<std::vector<std::vector<double> > > >(numRates, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) ) ),
storedPartialLikelihoods( std::vector<std::vector<std::vector<std::vector<double> > > >(numRates, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numSites, std::vector<double>(numChars, 0.0) ) ) ) ),
rootLikelihoods( std::vector<std::vector<double> >(numRates, std::vector<double>(numSites, 0.0) ) ),
storedRootLikelihoods( std::vector<std::vector<double> >(numRates, std::vector<double>(numSites, 0.0) ) ),
gapMatrix(),
patternCounts(),
numPatterns( numSites ),
compressed( c ),
dirtyNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ),
dirty( std::vector< std::vector<bool> >(tau->getValue().getNumberOfNodes(), std::vector<bool>( siteRates.size(), true ) ) ) {
    
    // add the parameters to the parents list
    this->addParameter( tau );
    for (std::vector<const TypedDagNode<double> * >::iterator it = siteRates.begin(); it != siteRates.end(); ++it) {
        this->addParameter( *it );
    }
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::AbstractSiteHeterogeneousMixtureCharEvoModelSlow(const AbstractSiteHeterogeneousMixtureCharEvoModelSlow &n) : TypedDistribution<CharacterData<charType> >( n ), 
numSites( n.numSites ), 
numChars( n.numChars ),
numRates( n.numRates ),
tau( n.tau ), 
siteRates( n.siteRates ),
partialLikelihoods( n.partialLikelihoods ),
storedPartialLikelihoods( n.storedPartialLikelihoods ),
rootLikelihoods( n.rootLikelihoods ),
storedRootLikelihoods( n.storedRootLikelihoods ),
charMatrix( n.charMatrix ), 
gapMatrix( n.gapMatrix ), 
patternCounts( n.patternCounts ),
numPatterns( n.numPatterns ),
compressed( n.compressed ),
dirtyNodes( n.dirtyNodes ),
dirty( n.dirty ) {
    // parameters are automatically copied
    
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::~AbstractSiteHeterogeneousMixtureCharEvoModelSlow( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) {
        //        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }
}


template<class charType, class treeType>
RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>* RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::clone( void ) const {
    return new AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>( *this );
}


template<class charType, class treeType>
double RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::computeLnProbability( void ) {
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    size_t rootIndex = root.getIndex();
    
//    if ( dirtyNodes[rootIndex] ) {
//        // mark as computed
//        dirtyNodes[rootIndex] = false;
        
        for (size_t index_rates = 0; index_rates < numRates; ++index_rates) {
        
//            if ( dirtyRates[index_rates] ) {
//                // mark as computed
//                dirtyRates[index_rates] = false;
            
            
                // start by filling the likelihood vector for the two children of the root
                const TopologyNode &left = root.getChild(0);
                size_t leftIndex = left.getIndex();
                fillLikelihoodVector( left, index_rates );
                const TopologyNode &right = root.getChild(1);
                size_t rightIndex = right.getIndex();
                fillLikelihoodVector( right, index_rates );
            
                computeRootLikelihood(rootIndex, leftIndex,rightIndex,index_rates);
//            }
//        }
    }
    
    sumRootLikelihood();
    
    return this->lnProb;
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::fillLikelihoodVector(const TopologyNode &node, size_t rate) {    
    
    // get the index in the node sequence
//    size_t nodeIndex = node.getIndex();
//    
//    // check for recomputation
//    if ( dirtyNodes[nodeIndex] ) {
//        // mark as computed
//        dirtyNodes[nodeIndex] = false;
        
        if ( node.isTip() ) {
            
            computeTipLikelihood(node,rate);
        } else {
            
            // start by filling the likelihood vector for the two children of the root
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, rate );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rate );
            
            computeInternalNodeLikelihood(node,leftIndex,rightIndex,rate);
        }
//    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n ) {
    recursivelyFlagNodeDirty( n );
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::keepSpecialization( DagNode* affecter ) {
    // flags for nodes
    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it) {
        (*it) = false;
    }
    
    // flags for rates
//    for (std::vector<bool>::iterator it = this->dirty.begin(); it != this->dirty.end(); ++it) {
//        (*it) = false;
//    }
    
    // store the partial likelihoods
    storedPartialLikelihoods = partialLikelihoods;
    storedRootLikelihoods = rootLikelihoods;
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    size_t index = n.getIndex();
    if ( !dirtyNodes[index] ) {
        if ( !n.isRoot() ) {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        dirtyNodes[index] = true;
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::redrawValue( void ) {
    delete this->value;
    this->value = this->simulate(tau->getValue().getRoot());
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::restoreSpecialization( DagNode* affecter ) {
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) {
        (*it) = false;
    }
//    for (std::vector<bool>::iterator it = dirtyRates.begin(); it != dirtyRates.end(); ++it) {
//        (*it) = false;
//    }
    
    // restore the partial likelihoods
    partialLikelihoods = storedPartialLikelihoods;
    rootLikelihoods = storedRootLikelihoods;
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::setValue(CharacterData<charType> *v) {
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
    partialLikelihoods = std::vector<std::vector<std::vector<std::vector<double> > > >(numRates, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numPatterns, std::vector<double>(numChars, 0.0) ) ) );
    storedPartialLikelihoods = std::vector<std::vector<std::vector<std::vector<double> > > >(numRates, std::vector<std::vector<std::vector<double> > >(tau->getValue().getNumberOfNodes(), std::vector<std::vector<double> >( numPatterns, std::vector<double>(numChars, 0.0) ) ) );
    rootLikelihoods = std::vector<std::vector<double> >(numRates, std::vector<double>(numPatterns, 0.0 ) );
    storedRootLikelihoods = std::vector<std::vector<double> >(numRates, std::vector<double>(numPatterns, 0.0 ) );
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::sumRootLikelihood( void ) {
    this->lnProb = 0.0;
    
    // for each site
    std::vector<size_t>::iterator freq = patternCounts.begin();
    for (size_t i = 0; i < numPatterns; ++i, ++freq) {
        
        // for each rate
        double perSiteLikelihood = 0.0;
        for (size_t j = 0; j < numRates; ++j) {
            perSiteLikelihood += 0.25 * exp( rootLikelihoods[j][i] );
        }
        
        // add the ln-likelihood times the pattern frequency
        lnProb += log( perSiteLikelihood ) * *freq;
        
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
    
    for (unsigned int i = 0 ; i < numRates ; i++) {
        if (oldP == siteRates[i]) {
            siteRates[i] = static_cast<const TypedDagNode< double > * >( newP );
        }
        
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractSiteHeterogeneousMixtureCharEvoModelSlow<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    
    // skip if the topology is the toucher
    if ( affecter != tau ) {
        
        // it was neither the topology nor the site rates (maybe the rate matrix ...)
        // for safety we touch all and recompute all conditional likelihoods
//        for (std::vector<bool>::iterator it = dirtyRates.begin(); it != dirtyRates.end(); ++it) {
//            (*it) = true;
//        }
        
        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) {
            (*it) = true;
        }
            
    }
    
}


#endif
