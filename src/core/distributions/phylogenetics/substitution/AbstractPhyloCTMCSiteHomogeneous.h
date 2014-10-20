#ifndef AbstractPhyloCTMCSiteHomogeneous_H
#define AbstractPhyloCTMCSiteHomogeneous_H

#include "AbstractDiscreteCharacterData.h"
#include "DiscreteTaxonData.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

#include <memory.h>

namespace RevBayesCore {
    
    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     * This file contains the distribution class for a character state evolving along a tree.
     * This abstract base class can be derived for any character evolution model with homogeneous mixture sites. A
     * homogeneous mixture model over sites is a model where all sites are drawn from the same distribution and the
     * specific instance of the per site parameter is integrated over. The per site parameter could be a rate scaler (e.g. the + gamma models)
     * or different rate matrices or anything else.
     *
     * The pruning algorithm is implemented in this base class and calles some few pure virtual methods. 
     * The important functions you have to override are:
     * - computeRootLikelihood(size_t root, size_t l, size_t r, size_t m)
     * - computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r)
     * - computeTipLikelihood(const TopologyNode &node, size_t nIdx)
     * - getRootFrequencies()
     * - updateTransitionProbabilities()
     *
     *
     * The data are stored for convenience in this class in a matrix (std::vector<std::vector< unsigned > >) and can
     * be compressed.
     *
     * The partial likelihoods are stored in a c-style array called partialLikelihoods. The dimension are
     * partialLikelihoods[active][nodeIndex][siteRateIndex][siteIndex][charIndex], however, since this is a one-dimensional c-style array,
     * you have to access the partialLikelihoods via 
     * partialLikelihoods[active*numNodes*numSiteRates*numPatterns*numChars + 
     *                    nodeIndex*numSiteRates*numPatterns*numChars + 
     *                    siteRateIndex*numPatterns*numChars +
     *                    siteIndex*numChars +
     *                    charIndex]
     * Since this is a bit complex, we have some offset variables for convenience:
     * activeLikelihoodOffset      =  numNodes*numSiteRates*numPatterns*numChars;
     * nodeOffset                  =  numSiteRates*numPatterns*numChars;
     * mixtureOffset               =  numPatterns*numChars;
     * siteOffset                  =  numChars;
     * This gives the more convenient access via
     * partialLikelihoods[active*activeLikelihoodOffset + nodeIndex*nodeOffset + siteRateIndex*mixtureOffset + siteIndex*siteOffset + charIndex]
     *
     * Our implementation of the partial likelihoods mean that we can store the partial likelihood of a node, but not for site rates.
     * We also use twice as much memory because we store the partial likelihood along each branch and not only for each internal node.
     * This gives us a speed improvement during MCMC proposal in the order of a factor 2.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-06-17, version 1.0
     */
    template<class charType, class treeType>
    class AbstractPhyloCTMCSiteHomogeneous : public TypedDistribution< AbstractDiscreteCharacterData >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, size_t nMix, bool c, size_t nSites);
        AbstractPhyloCTMCSiteHomogeneous(const AbstractPhyloCTMCSiteHomogeneous &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractPhyloCTMCSiteHomogeneous(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractPhyloCTMCSiteHomogeneous*                 clone(void) const = 0;                                                                      //!< Create an independent clone
    
        // non-virtual
        double                                                              computeLnProbability(void);
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                                 //!< The tree has changed and we want to know which part.
        void                                                                setValue(AbstractDiscreteCharacterData *v);                                                 //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        void                                                                reInitialized(void);
        
        void                                                                setClockRate(const TypedDagNode< double > *r);
        void                                                                setClockRate(const TypedDagNode< RbVector< double > > *r);
        void                                                                setPInv(const TypedDagNode< double > *);
        void                                                                setRateMatrix(const TypedDagNode< RateMatrix > *rm);
        void                                                                setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm);
        void                                                                setRootFrequencies(const TypedDagNode< RbVector< double > > *f);
        void                                                                setSiteRates(const TypedDagNode< RbVector< double > > *r);
        
        
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual std::set<const DagNode*>                                    getParameters(void) const;                                          //!< Return parameters
        virtual void                                                        swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    protected:
        // helper method for this and derived classes
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        void                                                                rescale(size_t nodeIndex);
        void                                                                resizeLikelihoodVectors(void);

        virtual void                                                        updateTransitionProbabilities(size_t nodeIdx, double brlen);
        virtual const std::vector<double>&                                  getRootFrequencies(void) const;
                
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher);
        
        // pure virtual methods
        virtual void                                                        computeRootLikelihood(size_t root, size_t l, size_t r) = 0;
        virtual void                                                        computeRootLikelihood(size_t root, size_t l, size_t r, size_t m) = 0;
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r) = 0;
        virtual void                                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx) = 0;
        
        // members
        double                                                              lnProb;
        size_t                                                              numNodes;
        size_t                                                              numSites;
        const size_t                                                        numChars;
        size_t                                                              numSiteRates;
        const TypedDagNode<treeType>*                                       tau;
        std::vector<TransitionProbabilityMatrix>                            transitionProbMatrices;
        
        // the likelihoods
        double*                                                             partialLikelihoods;
        std::vector<size_t>                                                 activeLikelihood;
        std::vector<double>                                                 scalingFactors;
        
        // the data
        std::vector<std::vector<unsigned long> >                            charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;
        std::vector<size_t>                                                 patternCounts;
        std::vector<bool>                                                   siteInvariant;
        std::vector<size_t>                                                 invariantSiteIndex;
        size_t                                                              numPatterns;
        bool                                                                compressed;
        
        // convenience variables available for derived classes too
        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;

        // offsets for nodes
        size_t                                                              activeLikelihoodOffset;
        size_t                                                              nodeOffset;
        size_t                                                              mixtureOffset;
        size_t                                                              siteOffset;
        
        // flags
        bool                                                                usingAmbiguousCharacters;
        bool                                                                treatUnknownAsGap;
        bool                                                                treatAmbiguousAsGaps;
        
        // members
        const TypedDagNode< double >*                                       homogeneousClockRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousClockRates;
        const TypedDagNode< RateMatrix >*                                   homogeneousRateMatrix;
        const TypedDagNode< RbVector< RateMatrix > >*                       heterogeneousRateMatrices;
        const TypedDagNode< RbVector< double > >*                           rootFrequencies;
        const TypedDagNode< RbVector< double > >*                           siteRates;
        const TypedDagNode< RbVector< double > >*                           siteRatesProbs;
        const TypedDagNode< double >*                                       pInv;
        
        
        // flags specifying which model variants we use
        bool                                                                branchHeterogeneousClockRates;
        bool                                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                                rateVariationAcrossSites;

        
    private:
        // private methods
        void                                                                compress(void);
        void                                                                fillLikelihoodVector(const TopologyNode &n, size_t nIdx);
        void                                                                simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<size_t> &perSiteRates);

    
    };
    
}


#include "DiscreteCharacterState.h"
#include "DiscreteCharacterData.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix_JC.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>

//#define USE_SCALING

template<class charType, class treeType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, size_t nMix, bool c, size_t nSites) :
    TypedDistribution< AbstractDiscreteCharacterData >(  new DiscreteCharacterData<charType>() ),
    numNodes( t->getValue().getNumberOfNodes() ),
    numSites( nSites ),
    numChars( nChars ),
    numSiteRates( nMix ),
    tau( t ), 
    transitionProbMatrices( std::vector<TransitionProbabilityMatrix>(numSiteRates, TransitionProbabilityMatrix(numChars) ) ),
    partialLikelihoods( new double[2*numNodes*numSiteRates*numSites*numChars] ),
    activeLikelihood( std::vector<size_t>(numNodes, 0) ),
    scalingFactors( std::vector<double>(numNodes*2, 1.0) ),
    charMatrix(), 
    gapMatrix(),
    patternCounts(),
    numPatterns( numSites ),
    siteInvariant( numSites, false ),
    invariantSiteIndex( numSites, 0 ),
    compressed( c ),
    changedNodes( std::vector<bool>(numNodes,false) ),
    dirtyNodes( std::vector<bool>(numNodes, true) ),
    usingAmbiguousCharacters( true ),
    treatUnknownAsGap( true ),
    treatAmbiguousAsGaps( true )
{
    
    // initialize with default parameters
    homogeneousClockRate        = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates     = NULL;
    homogeneousRateMatrix       = new ConstantNode<RateMatrix>("rateMatrix", new RateMatrix_JC( numChars ) );
    heterogeneousRateMatrices   = NULL;
    rootFrequencies             = NULL;
    siteRates                   = NULL;
    siteRatesProbs              = NULL;
    pInv                        = new ConstantNode<double>("pInv", new double(0.0) );
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;

    
    // We don'e want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    tau->incrementReferenceCount();
    
    activeLikelihoodOffset      =  numNodes*numSiteRates*numPatterns*numChars;
    nodeOffset                  =  numSiteRates*numPatterns*numChars;
    mixtureOffset               =  numPatterns*numChars;
    siteOffset                  =  numChars;
    
}


template<class charType, class treeType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::AbstractPhyloCTMCSiteHomogeneous(const AbstractPhyloCTMCSiteHomogeneous &n) :
    TypedDistribution< AbstractDiscreteCharacterData >( n ),
    numNodes( n.numNodes ),
    numSites( n.numSites ),
    numChars( n.numChars ),
    numSiteRates( n.numSiteRates ),
    tau( n.tau ), 
    transitionProbMatrices( n.transitionProbMatrices ),
    partialLikelihoods( new double[2*numNodes*numSiteRates*numSites*numChars] ),
    activeLikelihood( n.activeLikelihood ),
    scalingFactors( n.scalingFactors ),
    charMatrix( n.charMatrix ), 
    gapMatrix( n.gapMatrix ), 
    patternCounts( n.patternCounts ),
    numPatterns( n.numPatterns ),
    siteInvariant( n.siteInvariant ),
    invariantSiteIndex( n.invariantSiteIndex ),
    compressed( n.compressed ),
    changedNodes( n.changedNodes ),
    dirtyNodes( n.dirtyNodes ),
    usingAmbiguousCharacters( n.usingAmbiguousCharacters ),
    treatUnknownAsGap( n.treatUnknownAsGap ),
    treatAmbiguousAsGaps( n.treatAmbiguousAsGaps )
{
    
    // initialize with default parameters
    homogeneousClockRate        = n.homogeneousClockRate;
    heterogeneousClockRates     = n.heterogeneousClockRates;
    homogeneousRateMatrix       = n.homogeneousRateMatrix;
    heterogeneousRateMatrices   = n.heterogeneousRateMatrices;
    rootFrequencies             = n.rootFrequencies;
    siteRates                   = n.siteRates;
    siteRatesProbs              = n.siteRatesProbs;
    pInv                        = n.pInv;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = n.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = n.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = n.rateVariationAcrossSites;

    // We don't want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    tau->incrementReferenceCount();
    
    // copy the partial likelihoods
    memcpy(partialLikelihoods, n.partialLikelihoods, 2*numNodes*numSiteRates*numPatterns*numChars*sizeof(double));
    
    activeLikelihoodOffset      =  numNodes*numSiteRates*numPatterns*numChars;
    nodeOffset                  =  numSiteRates*numPatterns*numChars;
    mixtureOffset               =  numPatterns*numChars;
    siteOffset                  =  numChars;
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class charType, class treeType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::~AbstractPhyloCTMCSiteHomogeneous( void ) {
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL ) 
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );
        if ( tau->decrementReferenceCount() == 0 )
        {
            delete tau;
        }
        
    }
    
    // free the partial likelihoods
    delete [] partialLikelihoods;
}


template<class charType, class treeType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>* RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::clone( void ) const 
{
    
    return new AbstractPhyloCTMCSiteHomogeneous<charType, treeType>( *this );
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::compress( void ) 
{
    
//    compressed = false;
    
    charMatrix.clear();
    gapMatrix.clear();
    patternCounts.clear();
    numPatterns = 0;
    
    // resize the matrices
    size_t tips = tau->getValue().getNumberOfTips();
    charMatrix.resize(tips);
    gapMatrix.resize(tips);
    
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices = std::vector<size_t>(numSites,0);
    size_t siteIndex = 0;
    for (size_t i = 0; i < numSites; ++i)
    {
        while ( this->value->isCharacterExcluded(siteIndex) )
        {
            siteIndex++;
            if ( siteIndex >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }
        siteIndices[i] = siteIndex;
        siteIndex++;
    }
    // test if there were additional sites that we did not use
    while ( siteIndex < this->value->getNumberOfCharacters() )
    {
        if ( !this->value->isCharacterExcluded(siteIndex)  )
        {
            throw RbException( "The character matrix cannot set to this variable because it has too many included characters." );
        }
        siteIndex++;
    }
    
    // check whether there are ambiguous characters (besides gaps)
    bool ambiguousCharacters = false;
    // find the unique site patterns and compute their respective frequencies
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    for (size_t site = 0; site < numSites; ++site) 
    {
        
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
        {
            if ( (*it)->isTip() ) 
            {
                // \todo modify this so that the distribution is actually defined on discrete character data
                AbstractTaxonData& taxon = value->getTaxonData( (*it)->getName() );
                DiscreteCharacterState &c = static_cast<DiscreteCharacterState &>( taxon.getCharacter(siteIndices[site]) );
                
                // if we treat unknown characters as gaps and this is an unknown character then we change it
                // because we might then have a pattern more
                if ( treatAmbiguousAsGaps && c.isAmbiguous() )
                {
                    c.setGapState( true );
                }
                else if ( treatUnknownAsGap && c.getNumberOfStates() == c.getNumberObservedStates() )
                {
                    c.setGapState( true );
                }
                else if ( !c.isGapState() && c.isAmbiguous() )
                {
                    ambiguousCharacters = true;
                    break;
                }
            }
        }
        
        // break the loop if there was an ambiguous character
        if ( ambiguousCharacters ) 
        {
            break;
        }
    }
    
    // set the global variable if we use ambiguous characters
    usingAmbiguousCharacters = ambiguousCharacters;

    
    std::vector<bool> unique(numSites, true);
    // compress the character matrix if we're asked to
    if ( compressed ) 
    {
        // find the unique site patterns and compute their respective frequencies
        std::map<std::string,size_t> patterns;
        for (size_t site = 0; site < numSites; ++site) 
        {
            // create the site pattern
            std::string pattern = "";
            for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
            {
                if ( (*it)->isTip() ) 
                {
                    AbstractTaxonData& taxon = value->getTaxonData( (*it)->getName() );
                    CharacterState &c = taxon.getCharacter(siteIndices[site]);
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
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        if ( (*it)->isTip() ) 
        {
            size_t nodeIndex = (*it)->getIndex();
            AbstractTaxonData& taxon = value->getTaxonData( (*it)->getName() );
            
            // resize the column
            charMatrix[nodeIndex].resize(numPatterns);
            gapMatrix[nodeIndex].resize(numPatterns);
            size_t patternIndex = 0;
            for (size_t site = 0; site < numSites; ++site) 
            {
                // only add this site if it is unique
                if ( unique[site] ) 
                {
                    charType &c = static_cast<charType &>( taxon.getCharacter(siteIndices[site]) );
                    gapMatrix[nodeIndex][patternIndex] = c.isGapState();

                    if ( ambiguousCharacters ) 
                    {
                        // we use the actual state
                        charMatrix[nodeIndex][patternIndex] = c.getState();
                    }
                    else
                    {
                        // we use the index of the state
                        size_t index = 0;
                        unsigned long state = c.getState();
                        state >>= 1;
                        
                        while ( state != 0 ) // there are still observed states left
                        {
                            
                            // remove this state from the observed states
                            state >>= 1;
                            
                            // increment the index
                            ++index;
                        } // end-while over all observed states for this character
                        
                        charMatrix[nodeIndex][patternIndex] = index;
                    }

                    // increase the pattern index
                    patternIndex++;
                }
            }
        }
    }
    
    // reset the vector if a site is invariant
    siteInvariant.resize( numPatterns );
    invariantSiteIndex.resize( numPatterns );
    size_t length = charMatrix.size();
    for (size_t i=0; i<numPatterns; ++i)
    {
        bool inv = true;
        unsigned long c = charMatrix[0][i];
        
//        unsigned long val = c;
//        size_t d = 0;
//        while ( val > 0 ) // there are still observed states left
//        {
//            // remove this state from the observed states
//            val >>= 1;
//            
//            // increment the pointer to the next transition probability
//            ++d;
//        } // end-while over all observed states for this character
//        
//        invariantSiteIndex[i] = d;

        invariantSiteIndex[i] = c;
        
        for (size_t j=1; j<length; ++j)
        {
            if ( c != charMatrix[j][i] )
            {
                inv = false;
                break;
            }
        }
        
        siteInvariant[i] = inv;
    }
    
    // finally we resize the partial likelihood vectors to the new pattern counts
    this->resizeLikelihoodVectors();
    
}


template<class charType, class treeType>
double RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::computeLnProbability( void ) 
{
	
	// TAH Run under prior
//	this->lnProb = 0.0;
//    return this->lnProb;
	
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    // only necessary if the root is actually dirty
    if ( dirtyNodes[rootIndex] ) 
    {
                
        // mark as computed
        dirtyNodes[rootIndex] = false;
        
        
        // start by filling the likelihood vector for the children of the root
        if ( root.getNumberOfChildren() == 2 ) // rooted trees have two children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, leftIndex );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rightIndex );
            
            // compute the likelihood of the root
            computeRootLikelihood( rootIndex, leftIndex, rightIndex );
        }
        else if ( root.getNumberOfChildren() == 3 ) // unrooted trees have three children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, leftIndex );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rightIndex );
            const TopologyNode &middle = root.getChild(2);
            size_t middleIndex = middle.getIndex();
            fillLikelihoodVector( middle, middleIndex );
            
            // compute the likelihood of the root
            computeRootLikelihood( rootIndex, leftIndex, rightIndex, middleIndex );
        }
        else
        {
            throw RbException("The root node has an unexpected number of children. Only 2 (for rooted trees) or 3 (for unrooted trees) are allowed.");
        }
        
        
#ifdef USE_SCALING
        for (size_t i = 0; i<numNodes; ++i)
        {
            double sf = this->scalingFactors[this->activeLikelihood[i]*numNodes+i];
            this->lnProb += numSites * log( sf );
        }
#endif
        
    }
    
    return this->lnProb;
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::fillLikelihoodVector(const TopologyNode &node, size_t nodeIndex) 
{    
    
    // check for recomputation
    if ( dirtyNodes[nodeIndex] ) 
    {
        // mark as computed
        dirtyNodes[nodeIndex] = false;
        
        if ( node.isTip() ) 
        {
            // this is a tip node
            // compute the likelihood for the tip and we are done
            computeTipLikelihood(node, nodeIndex);
        } 
        else 
        {
            // this is an internal node
            
            // start by filling the likelihood vector for the two children of this node
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rightIndex );
            
            // now compute the likelihoods of this internal node
            computeInternalNodeLikelihood(node,nodeIndex,leftIndex,rightIndex);
            
#ifdef USE_SCALING
            // rescale the partial likelihoods
            rescale( nodeIndex );
#endif
        }
    }
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n )
{
    
    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::keepSpecialization( DagNode* affecter )
{
    
    // test
    // SH-20140822: Who and why is this in here?
//    this->computeLnProbability();
    
    // reset all flags
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
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {
    
    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();
    
    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( !dirtyNodes[index] ) 
    {
        // the root doesn't have an ancestor
        if ( !n.isRoot() ) 
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        
        // set the flag
        dirtyNodes[index] = true;
        
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( !changedNodes[index] ) 
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            changedNodes[index] = true;
        }
        
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::redrawValue( void )
{
    
    // delete the old value first
    delete this->value;
    
    // create a new character data object
    this->value = new DiscreteCharacterData<charType>();
    
    // create a vector of taxon data 
    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData< charType > >( numNodes, DiscreteTaxonData<charType>("") );
    
    // first, simulate the per site rates
    RandomNumberGenerator* rng = GLOBAL_RNG;
    std::vector<size_t> perSiteRates;
    for ( size_t i = 0; i < numSites; ++i ) 
    {
        // draw the state
        double u = rng->uniform01();
        size_t rateIndex = size_t(u*numSiteRates);
        perSiteRates.push_back( rateIndex );
    }
    
    // simulate the root sequence
    const std::vector< double > &stationaryFreqs = getRootFrequencies();
    DiscreteTaxonData< charType > &root = taxa[ tau->getValue().getRoot().getIndex() ];
    for ( size_t i = 0; i < numSites; ++i ) 
    {
        // create the character
        charType c;
        c.setToFirstState();
        // draw the state
        double u = rng->uniform01();
        std::vector< double >::const_iterator freq = stationaryFreqs.begin();
        while ( true ) 
        {
            u -= *freq;
            
            if ( u > 0.0 )
            {
                ++c;
                ++freq;
            }
            else 
            {
                break;
            }
            
        }
        
        // add the character to the sequence
        root.addCharacter( c );
    }
    
    // recursively simulate the sequences
    simulate( tau->getValue().getRoot(), taxa, perSiteRates );
    
    // add the taxon data to the character data
    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i) 
    {
        this->value->addTaxonData( taxa[i] );
    }
    
    // compress the data and initialize internal variables
    this->compress();
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::reInitialized( void )
{
    
    // we need to recompress because the tree may have changed
    compress();
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::rescale( size_t nodeIndex )
{
    //
    
    double* p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    // iterate over all mixture categories
    double max = 0.0;
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;

        // iterate over the number of sites
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            // iterate over the possible starting states
            for (size_t c1 = 0; c1 < this->numChars; ++c1)
            {
                double m = p_site_mixture[c1];
                if ( m > max )
                {
                    max = m;
                }
            }
            
            p_site_mixture+=this->siteOffset;
        }
        
    }
    
    this->scalingFactors[this->activeLikelihood[nodeIndex]*numNodes+nodeIndex] = max;
    
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        
        // iterate over the number of sites
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            // iterate over the possible starting states
            for (size_t c1 = 0; c1 < this->numChars; ++c1)
            {
                p_site_mixture[c1] /= max;
            }
            
            p_site_mixture+=this->siteOffset;
        }
        
    }
    
                

}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::resizeLikelihoodVectors( void )
{
    
    // we resize the partial likelihood vectors to the new dimensions
    delete [] partialLikelihoods;
    partialLikelihoods = new double[2*numNodes*numSiteRates*numPatterns*numChars];
    
    transitionProbMatrices = std::vector<TransitionProbabilityMatrix>(numSiteRates, TransitionProbabilityMatrix(numChars) );
    
    // set the offsets for easier iteration through the likelihood vector 
    activeLikelihoodOffset      =  numNodes*numSiteRates*numPatterns*numChars;
    nodeOffset                  =  numSiteRates*numPatterns*numChars;
    mixtureOffset               =  numPatterns*numChars;
    siteOffset                  =  numChars;
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::restoreSpecialization( DagNode* affecter )
{
    
    // reset the flags
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it) 
    {
        (*it) = false;
    }
    
    // restore the active likelihoods vector
    for (size_t index = 0; index < changedNodes.size(); ++index) 
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index] == true )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }
        
        // set all flags to false
        changedNodes[index] = false;
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setValue(AbstractDiscreteCharacterData *v)
{
    
    // delegate to the parent class
    TypedDistribution< AbstractDiscreteCharacterData >::setValue(v);
    
    // reset the number of sites
    this->numSites = v->getNumberOfIncludedCharacters();
    
    // now compress the data and resize the likelihood vectors
    this->compress();
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::simulate( const TopologyNode &node, std::vector< DiscreteTaxonData< charType > > &taxa, const std::vector<size_t> &perSiteRates)
{
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    const DiscreteTaxonData< charType > &parent = taxa[ nodeIndex ];
    
    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it) 
    {
        const TopologyNode &child = *(*it);
        
        // update the transition probability matrix
        updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );
        
        DiscreteTaxonData< charType > &taxon = taxa[ child.getIndex() ];
        for ( size_t i = 0; i < numSites; ++i ) 
        {
            // get the ancestral character for this site
            unsigned long parentState = parent.getCharacter( i ).getStateIndex();
            
            double *freqs = transitionProbMatrices[ perSiteRates[i] ][ parentState ];
            
            // create the character
            charType c;
            c.setToFirstState();
            // draw the state
            double u = rng->uniform01();
            while ( true ) 
            {
                u -= *freqs;
                
                if ( u > 0.0 )
                {
                    ++c;
                    ++freqs;
                }
                else
                {
                    break;
                }
            }
            
            // add the character to the sequence
            taxon.addCharacter( c );
        }
        
        if ( child.isTip() ) 
        {
            taxon.setTaxonName( child.getName() );
        }
        else 
        {
            // recursively simulate the sequences
            simulate( child, taxa, perSiteRates );
        }
        
    }
    
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::getRootFrequencies( void ) const
{
    
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
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen)
{
    
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
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setClockRate(const TypedDagNode< double > *r)
{
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL )
    {
//        delete homogeneousClockRate;
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
//        delete heterogeneousClockRates;
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setClockRate(const TypedDagNode< RbVector< double > > *r)
{
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL )
    {
//        delete homogeneousClockRate;
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
//        delete heterogeneousClockRates;
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setPInv(const TypedDagNode< double > *r)
{
    
    // remove the old parameter first
    if ( pInv != NULL )
    {
//        delete pInv;
        pInv = NULL;
    }
    
    // set the value
    pInv = r;
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setRateMatrix(const TypedDagNode< RateMatrix > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL )
    {
//        delete homogeneousRateMatrix;
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
//        delete heterogeneousRateMatrices;
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateMatrix = rm;
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL )
    {
//        delete homogeneousRateMatrix;
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
//        delete heterogeneousRateMatrices;
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateMatrices = rm;
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setRootFrequencies(const TypedDagNode< RbVector< double > > *f)
{
    
    // remove the old parameter first
    if ( rootFrequencies != NULL )
    {
//        delete rootFrequencies;
        rootFrequencies = NULL;
    }
    
    if ( f != NULL )
    {
        // set the value
        //        branchHeterogeneousSubstitutionMatrices = true;
        rootFrequencies = f;
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setSiteRates(const TypedDagNode< RbVector< double > > *r) {
    
    // remove the old parameter first
    if ( siteRates != NULL )
    {
//        delete siteRates;
        siteRates = NULL;
    }
    
    if ( r != NULL )
    {
        // set the value
        rateVariationAcrossSites = true;
        siteRates = r;
        this->numSiteRates = r->getValue().size();
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
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}


/** Get the parameters of the distribution */
template<class charType, class treeType>
std::set<const RevBayesCore::DagNode*> RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::getParameters( void ) const
{
    std::set<const DagNode*> parameters = std::set<const DagNode*>();
    
    parameters.insert( tau );
    parameters.insert( homogeneousClockRate );
    parameters.insert( heterogeneousClockRates );
    parameters.insert( homogeneousRateMatrix );
    parameters.insert( heterogeneousRateMatrices );
    parameters.insert( rootFrequencies );
    parameters.insert( siteRates );
    parameters.insert( siteRatesProbs );
    parameters.insert( pInv );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == homogeneousClockRate)
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates)
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
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
        rootFrequencies = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == siteRates)
    {
        siteRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == siteRatesProbs)
    {
        siteRatesProbs = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == pInv)
    {
        pInv = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == tau)
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );
        tau->decrementReferenceCount();
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
        tau->incrementReferenceCount();
        numNodes = tau->getValue().getNumberOfNodes();
    }
    
}

template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    bool touchAll = false;
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == heterogeneousClockRates )
    {
        const std::set<size_t> &indices = heterogeneousClockRates->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 )
        {
            // just flag everyting for recomputation
            touchAll = true;
        }
        else
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it)
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            }
        }
    }
    else if ( affecter == heterogeneousRateMatrices )
    {
        
        const std::set<size_t> &indices = heterogeneousRateMatrices->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 )
        {
            // just flag everyting for recomputation
            touchAll = true;
        }
        else
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it)
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            }
        }
    }
    else if ( affecter == rootFrequencies )
    {
        
        const TopologyNode &root = this->tau->getValue().getRoot();
        this->recursivelyFlagNodeDirty( root );
    }
    else if ( affecter != tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    {
        touchAll = true;
    }
    
    if ( touchAll )
    {
        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
        {
            (*it) = true;
        }
        
        // flip the active likelihood pointers
        for (size_t index = 0; index < changedNodes.size(); ++index)
        {
            if ( changedNodes[index] == false )
            {
                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                changedNodes[index] = true;
            }
        }
    }
    
}




#endif
