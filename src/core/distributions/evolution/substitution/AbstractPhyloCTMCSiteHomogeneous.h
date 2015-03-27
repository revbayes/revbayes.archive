#ifndef AbstractPhyloCTMCSiteHomogeneous_H
#define AbstractPhyloCTMCSiteHomogeneous_H

#include "AbstractDiscreteCharacterData.h"
#include "DiscreteTaxonData.h"
#include "DnaState.h"
#include "Rategenerator.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"
#include "ConstantNode.h"

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
        AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, size_t nMix, bool c, size_t nSites, bool amb );
        AbstractPhyloCTMCSiteHomogeneous(const AbstractPhyloCTMCSiteHomogeneous &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractPhyloCTMCSiteHomogeneous(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractPhyloCTMCSiteHomogeneous*                           clone(void) const = 0;                                                                      //!< Create an independent clone
    
        // non-virtual
        double                                                              computeLnProbability(void);
		virtual std::vector<charType>										drawAncestralStatesForNode(const TopologyNode &n);
        virtual void                                                        drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates);
        virtual void                                                        recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates);
        virtual void                                                        tipDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates);
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                                 //!< The tree has changed and we want to know which part.
        void																updateMarginalNodeLikelihoods(void);
        void                                                                setValue(AbstractDiscreteCharacterData *v, bool f=false);                                   //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        void                                                                reInitialized(void);
        
        void                                                                setClockRate(const TypedDagNode< double > *r);
        void                                                                setClockRate(const TypedDagNode< RbVector< double > > *r);
        void                                                                setPInv(const TypedDagNode< double > *);
        void                                                                setRateMatrix(const TypedDagNode< RateGenerator > *rm);
        void                                                                setRateMatrix(const TypedDagNode< RbVector< RateGenerator > > *rm);
        void                                                                setRootFrequencies(const TypedDagNode< RbVector< double > > *f);
        void                                                                setSiteRates(const TypedDagNode< RbVector< double > > *r);
        
        
    protected:
        // helper method for this and derived classes
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        void                                                                rescale(size_t nodeIndex);
        virtual void                                                        resizeLikelihoodVectors(void);

        virtual void                                                        updateTransitionProbabilities(size_t nodeIdx, double brlen);
        virtual const std::vector<double>&                                  getRootFrequencies(void) const;
        
        
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter

        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // pure virtual methods
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r) = 0;
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m) = 0;
        virtual void                                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx) = 0;
        virtual void                                                        computeRootLikelihood( size_t root, size_t left, size_t right) = 0;
        virtual void                                                        computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle) = 0;

        // virtual methods that you may want to overwrite
        virtual void                                                        computeMarginalNodeLikelihood(size_t nodeIdx, size_t parentIdx);
        virtual void                                                        computeMarginalRootLikelihood();
        virtual std::vector< std::vector< double > >*                       sumMarginalLikelihoods(size_t nodeIndex);

        
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
		double*																marginalLikelihoods;
        
        std::vector< std::vector< std::vector<double> > >                   perNodeSiteLogScalingFactors;
        bool                                                                useScaling;
        
        // the data
        std::vector<std::vector<unsigned long> >                            charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;
        std::vector<size_t>                                                 patternCounts;
        std::vector<bool>                                                   siteInvariant;
        std::vector<size_t>                                                 invariantSiteIndex;
        size_t                                                              numPatterns;
        bool                                                                compressed;
		std::vector<size_t>                                                 sitePattern;    // an array that keeps track of which pattern is used for each site
        
        // convenience variables available for derived classes too
        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;

        // offsets for nodes
        size_t                                                              activeLikelihoodOffset;
        size_t                                                              nodeOffset;
        size_t                                                              mixtureOffset;
        size_t                                                              siteOffset;
        size_t                                                              nodeOffsetMarginal;
        size_t                                                              siteOffsetMarginal;
        
        // flags
        bool                                                                usingAmbiguousCharacters;
        bool                                                                treatUnknownAsGap;
        bool                                                                treatAmbiguousAsGaps;
        
        // members
        const TypedDagNode< double >*                                       homogeneousClockRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousClockRates;
        const TypedDagNode< RateGenerator >*                                homogeneousRateMatrix;
        const TypedDagNode< RbVector< RateGenerator > >*                    heterogeneousRateMatrices;
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
        void                                                                recursiveMarginalLikelihoodComputation(size_t nIdx);
        void                                                                scale(size_t i, size_t l, size_t r);
        void                                                                scale(size_t i, size_t l, size_t r, size_t m);
        virtual void                                                        simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<size_t> &perSiteRates);
        virtual double                                                      sumRootLikelihood( void );
    
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

template<class charType, class treeType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<treeType> *t, size_t nChars, size_t nMix, bool c, size_t nSites,  bool amb) :
    TypedDistribution< AbstractDiscreteCharacterData >(  new DiscreteCharacterData<charType>() ),
    numNodes( t->getValue().getNumberOfNodes() ),
    numSites( nSites ),
    numChars( nChars ),
    numSiteRates( nMix ),
    tau( t ), 
    transitionProbMatrices( std::vector<TransitionProbabilityMatrix>(numSiteRates, TransitionProbabilityMatrix(numChars) ) ),
    partialLikelihoods( new double[2*numNodes*numSiteRates*numSites*numChars] ),
    activeLikelihood( std::vector<size_t>(numNodes, 0) ),
    marginalLikelihoods( new double[numNodes*numSiteRates*numSites*numChars] ),
    perNodeSiteLogScalingFactors( std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(numNodes*2, std::vector<double>(numSites, 0.0) ) ) ),
    useScaling( true ),
    charMatrix(),
    gapMatrix(),
    patternCounts(),
    siteInvariant( numSites, false ),
    invariantSiteIndex( numSites, 0 ),
    numPatterns( numSites ),
    compressed( c ),
    sitePattern( std::vector<size_t>(numSites, 0) ),
    changedNodes( std::vector<bool>(numNodes, false) ),
    dirtyNodes( std::vector<bool>(numNodes, true) ),
    usingAmbiguousCharacters( amb ),
    treatUnknownAsGap( true ),
    treatAmbiguousAsGaps( false )
{
    
    // initialize with default parameters
    homogeneousClockRate        = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates     = NULL;
    homogeneousRateMatrix       = new ConstantNode<RateGenerator>("rateMatrix", new RateMatrix_JC( numChars ) );
    heterogeneousRateMatrices   = NULL;
    rootFrequencies             = NULL;
    siteRates                   = NULL;
    siteRatesProbs              = NULL;
    pInv                        = new ConstantNode<double>("pInv", new double(0.0) );
    
	// initialize liklihood vectors to 0.0
	for (size_t i = 0; i < 2*numNodes*numSiteRates*numSites*numChars; i++)
    {
		partialLikelihoods[i] = 0.0;
	}
	for (size_t i = 0; i < numNodes*numSiteRates*numSites*numChars; i++)
    {
		marginalLikelihoods[i] = 0.0;
	}
    
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
    
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( tau );
    this->addParameter( homogeneousClockRate );
    this->addParameter( heterogeneousClockRates );
    this->addParameter( homogeneousRateMatrix );
    this->addParameter( heterogeneousRateMatrices );
    this->addParameter( rootFrequencies );
    this->addParameter( siteRates );
    this->addParameter( siteRatesProbs );
    this->addParameter( pInv );
    
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
    marginalLikelihoods( new double[numNodes*numSiteRates*numSites*numChars] ),
    perNodeSiteLogScalingFactors( n.perNodeSiteLogScalingFactors ),
    useScaling( n.useScaling ),
    charMatrix( n.charMatrix ),
    gapMatrix( n.gapMatrix ), 
    patternCounts( n.patternCounts ),
    siteInvariant( n.siteInvariant ),
    invariantSiteIndex( n.invariantSiteIndex ),
    numPatterns( n.numPatterns ),
    compressed( n.compressed ),
    sitePattern( n.sitePattern ),
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

    // copy the marginal likelihoods
    memcpy(marginalLikelihoods, n.marginalLikelihoods, numNodes*numSiteRates*numPatterns*numChars*sizeof(double));	
	
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
    delete [] marginalLikelihoods;
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::compress( void ) 
{
    
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
                AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );
                DiscreteCharacterState &c = taxon.getCharacter(siteIndices[site]);
                
                // if we treat unknown characters as gaps and this is an unknown character then we change it
                // because we might then have a pattern more
                if ( treatAmbiguousAsGaps && (c.isAmbiguous() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( treatUnknownAsGap && (c.getNumberOfStates() == c.getNumberObservedStates() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( !c.isGapState() && (c.isAmbiguous() || c.isMissingState()) )
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
                    AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );
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
				
				// remember which pattern this site uses
				sitePattern[site] = index->second;
            }
            else 
            {
                // create a new pattern frequency counter for this pattern
                patternCounts.push_back(1);
                
                // insert this pattern with the corresponding index in the map
                patterns.insert( std::pair<std::string,size_t>(pattern,numPatterns) );
                
				// remember which pattern this site uses
				sitePattern[site] = numPatterns;
				
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
            AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );
            
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
                        size_t index = c.getStateIndex();
                        
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
    resizeLikelihoodVectors();
    
}


template<class charType, class treeType>
double RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::computeLnProbability( void ) 
{
	
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    // only necessary if the root is actually dirty
    if ( dirtyNodes[rootIndex] ) 
    {
                
        // start by filling the likelihood vector for the children of the root
        if ( root.getNumberOfChildren() == 2 ) // rooted trees have two children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, leftIndex );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rightIndex );
            
            computeRootLikelihood( rootIndex, leftIndex, rightIndex );
            scale(rootIndex, leftIndex, rightIndex);
            
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
            
            computeRootLikelihood( rootIndex, leftIndex, rightIndex, middleIndex );
            scale(rootIndex, leftIndex, rightIndex, middleIndex);
            
        }
        else
        {
            throw RbException("The root node has an unexpected number of children. Only 2 (for rooted trees) or 3 (for unrooted trees) are allowed.");
        }

        
        // sum the partials up
        this->lnProb = sumRootLikelihood();
                
    }
    return this->lnProb;
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::computeMarginalNodeLikelihood( size_t nodeIndex, size_t parentNodeIndex )
{	
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, this->tau->getValue().getBranchLength(nodeIndex) );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node                  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    double*         p_node_marginal         = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    const double*   p_parent_node_marginal  = this->marginalLikelihoods + parentNodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double*   p_mixture                   = p_node;
    double*         p_mixture_marginal          = p_node_marginal;
    const double*   p_parent_mixture_marginal   = p_parent_node_marginal;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transitionProbMatrices[mixture].theMatrix;
        
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture                  = p_mixture;
        double*         p_site_mixture_marginal         = p_mixture_marginal;
        const double*   p_parent_site_mixture_marginal  = p_parent_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j                    = p_site_mixture;
            double*         p_site_marginal_j           = p_site_mixture_marginal;
            // iterate over all end states
            for (size_t j=0; j<numChars; ++j)
            {
                const double*   p_parent_site_marginal_k    = p_parent_site_mixture_marginal;
                *p_site_marginal_j = 0.0;

                // iterator over all start states
                for (size_t k=0; k<numChars; ++k)
                {
                    // transition probability for k->j
                    const double tp_kj = *p_parent_site_marginal_k * tp_begin[ k * numChars + j ];

                    // add the probability of starting from this state
                    *p_site_marginal_j += *p_site_j * tp_kj;
                    
                    // next parent state
                    ++p_parent_site_marginal_k;
                }
				
                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_marginal+=this->siteOffset; p_parent_site_mixture_marginal+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_marginal+=this->mixtureOffset; p_parent_mixture_marginal+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::computeMarginalRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();
    
    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node           = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    double*         p_node_marginal  = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double*   p_mixture           = p_node;
    double*         p_mixture_marginal  = p_node_marginal;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture          = p_mixture;
        double*         p_site_mixture_marginal = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j            = p_site_mixture;
            double*         p_site_marginal_j   = p_site_mixture_marginal;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                *p_site_marginal_j = *p_site_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_marginal+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_marginal+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
}



/**
 * Draw a vector of ancestral states from the marginal distribution (non-conditional of the other ancestral states).
 * Here we assume that the marginal likelihoods have been updated.
 */
template<class charType, class treeType>
std::vector<charType> RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::drawAncestralStatesForNode(const TopologyNode &node)
{
	
	size_t nodeIndex = node.getIndex();
	
	// get the marginal likelihoods
    std::vector< std::vector<double> >* marginals = sumMarginalLikelihoods(nodeIndex);
    
	RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector< charType > ancestralSeq = std::vector<charType>();
	
    for ( size_t i = 0; i < numSites; ++i )
    {
		size_t pattern = i;
		// if the matrix is compressed use the pattern for this site
		if (compressed) {
			pattern = sitePattern[i];
		}
		
        // create the character
        charType c;
        c.setToFirstState();

		// sum the likelihoods for each character state
		const std::vector<double> siteMarginals = (*marginals)[pattern];
		double sumMarginals = 0.0;
		for (int j = 0; j < siteMarginals.size(); j++)
        {
			sumMarginals += siteMarginals[j];
		}
		
		double u = rng->uniform01();
		if (sumMarginals == 0.0)
        {
			
			// randomly draw state if all states have 0 probability
			c.setState((size_t)(u*c.getNumberOfStates()));
			
		}
        else
        {
			
			// the marginals don't add up to 1, so rescale u
			u *= sumMarginals;
			
			// draw the character state
			size_t stateIndex = 0;
			while ( true )
            {
				
				u -= siteMarginals[stateIndex];
				
				if ( u > 0.0 )
                {
					
					c++;
					stateIndex++;
					
					if ( stateIndex == c.getNumberOfStates() )
                    {
						stateIndex = 0;
						c.setToFirstState();
					}
					
				}
                else
                {
					break;
				}
			}		
		}
		
        // add the character to the sequence
        ancestralSeq.push_back( c );
    }
	
    // we need to free the vector
    delete marginals;
    
	return ancestralSeq;
}


/**
 * Draw a vector of ancestral states from the joint-conditional distribution of states.
 */
template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates)
{
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get working variables
    const std::vector<double> &f = this->getRootFrequencies();
    std::vector<double> siteProbVector(1,1.0);
    if (siteRatesProbs != NULL)
        siteProbVector = siteRatesProbs->getValue();
    

    const TopologyNode &root = tau->getValue().getRoot();
    size_t nodeIndex = root.getIndex();
    size_t right = root.getChild(0).getIndex();
    size_t left = root.getChild(1).getIndex();
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double*   p_site           = p_node;
    const double*   p_left_site      = p_left;
    const double*   p_right_site     = p_right;

    
    // sample root states
    std::vector<double> p( this->numSiteRates*this->numChars, 0.0);
    std::vector<size_t> sampledSiteRates(this->numSites,0);
    for (size_t i = 0; i < this->numSites; i++)
    {

        // create the character
        charType c;
        c.setToFirstState();

        // sum to sample
        double sum = 0.0;
        
		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if (compressed) {
			pattern = sitePattern[i];
		}

        // get ptr to first mixture cat for site
        p_site          = p_node  + pattern * this->siteOffset;
        p_left_site     = p_left  + pattern * this->siteOffset;
        p_right_site    = p_right + pattern * this->siteOffset;
        
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            // get pointers to the likelihood for this mixture category
            const double* p_site_mixture_j       = p_site;
            const double* p_left_site_mixture_j  = p_left_site;
            const double* p_right_site_mixture_j = p_right_site;
            
            // iterate over all starting states
            for (size_t state = 0; state < this->numChars; ++state)
            {
                size_t k = this->numChars*mixture + state;
                p[k] = *p_site_mixture_j * *p_left_site_mixture_j * *p_right_site_mixture_j * f[state] * siteProbVector[mixture];
                sum += p[k];
                
                // increment the pointers to the next state for (site,rate)
                p_site_mixture_j++;
                p_left_site_mixture_j++;
                p_right_site_mixture_j++;
            }
        
            // increment the pointers to the next mixture category for given site
            p_site       += this->mixtureOffset;
            p_left_site  += this->mixtureOffset;
            p_right_site += this->mixtureOffset;
            
        } // end-for over all mixtures (=rate categories)

        // sample char from p
        bool stop = false;
        double u = rng->uniform01() * sum;
        for (size_t mixture = 0; mixture < this->numSiteRates; mixture++)
        {
            c.setToFirstState();
            for (size_t state = 0; state < this->numChars; state++)
            {
                size_t k = this->numChars * mixture + state;
                u -= p[k];
                if (u < 0.0)
                {
                    startStates[root.getIndex()][i] = c;
                    sampledSiteRates[i] = mixture;
                    stop = true;
                    break;
                }
                c++;
            }
            if (stop) break;
        }
        
        endStates[nodeIndex][i] = startStates[nodeIndex][i];
    }
    
    // recurse
    std::vector<TopologyNode*> children = root.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        // daughters identically inherit ancestral state
        startStates[ children[i]->getIndex() ] = endStates[ root.getIndex() ];
        
        // recurse towards tips
        if (!children[i]->isTip())
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        else
            tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
    }
}

template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates)
{
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get working variables
    size_t nodeIndex = node.getIndex();
    size_t left = node.getChild(0).getIndex();
    size_t right = node.getChild(1).getIndex();
//    size_t parentIndex = node.getParent().getIndex();
    
    // get transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
//    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
//    const double*   p_site           = p_node;
    const double*   p_left_site      = p_left;
    const double*   p_right_site     = p_right;
    
    // sample characters conditioned on start states, going to end states
    std::vector<double> p(this->numChars, 0.0);
    for (size_t i = 0; i < this->numSites; i++)
    {
        size_t cat = sampledSiteRates[i];
        size_t k = startStates[nodeIndex][i].getStateIndex();
        
        
        // sum to sample
        double sum = 0.0;
        
		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if (compressed) {
			pattern = sitePattern[i];
		}
        
        // get ptr to first mixture cat for site
//        p_site          = p_node  + cat * this->mixtureOffset + pattern * this->siteOffset;
        const double* p_left_site_mixture_j     = p_left  + cat * this->mixtureOffset + pattern * this->siteOffset;
        const double* p_right_site_mixture_j    = p_right + cat * this->mixtureOffset + pattern * this->siteOffset;
        
        // iterate over possible end states for each site given start state
        for (size_t j = 0; j < this->numChars; j++)
        {
            double tp_kj = this->transitionProbMatrices[cat][k][j];
            p[j] = tp_kj * *p_left_site_mixture_j * *p_right_site_mixture_j;
            sum += p[j];
            
//            p_site_mixture_j++;
            p_left_site_mixture_j++;
            p_right_site_mixture_j++;
        }

        // sample char from p
        charType c;
        c.setToFirstState();
        double u = rng->uniform01() * sum;
        for (size_t state = 0; state < this->numChars; state++)
        {
            u -= p[state];
            if (u < 0.0)
            {
                endStates[nodeIndex][i] = c;
                break;
            }
            c++;
        }
    }
    
    // recurse
    std::vector<TopologyNode*> children = node.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        // daughters identically inherit ancestral state
        startStates[ children[i]->getIndex() ] = endStates[ node.getIndex() ];
        
        // recurse towards tips
        if (!children[i]->isTip())
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        else
            tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
    }
}

template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::tipDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates)
{
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get working variables
    size_t nodeIndex = node.getIndex();
    const std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    const std::vector<unsigned long> &char_node = this->charMatrix[nodeIndex];

    
    // get transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double* p_site = p_node;
    
    // sample characters conditioned on start states, going to end states
    std::vector<double> p(this->numChars, 0.0);
    for (size_t i = 0; i < this->numSites; i++)
    {
        charType c;
        c.setToFirstState();
        for (size_t j = 0; j < char_node[i]; j++)
            c++;
        endStates[nodeIndex][i] = c;
    }
    
        /*
         
         
        size_t cat = sampledSiteRates[i];
        size_t k = startStates[nodeIndex][i].getStateIndex();
        
        // sum to sample
        double sum = 0.0;
        
		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if (compressed) {
			pattern = sitePattern[i];
		}
        
        // get ptr to first mixture cat for site
        const double* p_site_mixture_j = p_node  + cat * this->mixtureOffset + pattern * this->siteOffset;
        
        // iterate over possible end states for each site given start state
        for (size_t j = 0; j < this->numChars; j++)
        {
            double tp_kj = this->transitionProbMatrices[cat][k][j];
            p[j] = tp_kj * *p_site_mixture_j;
            sum += p[j];
            p_site_mixture_j++;
        }
        
        // sample char from p
        charType c;
        c.setToFirstState();
        double u = rng->uniform01() * sum;
        for (size_t state = 0; state < this->numChars; state++)
        {
            u -= p[state];
            if (u < 0.0)
            {
                endStates[nodeIndex][i] = c;
                break;
            }
            c++;
        }
         */
    // no further recursion
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
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            fillLikelihoodVector( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            fillLikelihoodVector( right, rightIndex );
                
            // now compute the likelihoods of this internal node
            computeInternalNodeLikelihood(node,nodeIndex,leftIndex,rightIndex);
                
            // rescale likelihood vector
            scale(nodeIndex,leftIndex,rightIndex);
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
const std::vector<double>& RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::getRootFrequencies( void ) const
{
    
    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL )
    {
        return rootFrequencies->getValue();
    }
    else
    {
        const RateMatrix *rm = dynamic_cast<const RateMatrix *>(&homogeneousRateMatrix->getValue());
        if ( rm != NULL )
        {
            return rm->getStationaryFrequencies();
        }
        else
        {
            throw RbException("If you want to use RateGenerators that are not RateMatrices then you need to specify the root frequencies directly.");
        }
        
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::keepSpecialization( DagNode* affecter )
{
    
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
        if ( changedNodes[index] == false )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            changedNodes[index] = true;
        }
        
    }
    
}



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::recursiveMarginalLikelihoodComputation( size_t nodeIndex )
{
    
    const TopologyNode &node = tau->getValue().getNode( nodeIndex );
    
    for ( size_t i=0; i<node.getNumberOfChildren(); ++i )
    {
        const TopologyNode &child = node.getChild(i);
        
        if ( !child.isTip() )
        {
            size_t childIndex = child.getIndex();
            computeMarginalNodeLikelihood( childIndex, nodeIndex );
            recursiveMarginalLikelihoodComputation( childIndex );
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
    root.setTaxonName( "Root" );
    
    // recursively simulate the sequences
    simulate( tau->getValue().getRoot(), taxa, perSiteRates );
    
    // add the taxon data to the character data
//    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
    for (size_t i = 0; i < tau->getValue().getNumberOfNodes(); ++i)
    {
        this->value->addTaxonData( taxa[i] );
    }
        
    // compress the data and initialize internal variables
    compress();
    
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


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::reInitialized( void )
{
    
    // we need to recompress because the tree may have changed
    compress();
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::resizeLikelihoodVectors( void )
{
    
    // we resize the partial likelihood vectors to the new dimensions
    delete [] partialLikelihoods;
    delete [] marginalLikelihoods;
    partialLikelihoods = new double[2*numNodes*numSiteRates*numPatterns*numChars];
    marginalLikelihoods = new double[numNodes*numSiteRates*numPatterns*numChars];
    
	// reinitialize likelihood vectors
	for (size_t i = 0; i < 2*numNodes*numSiteRates*numPatterns*numChars; i++) {
		partialLikelihoods[i] = 0.0;
	}
	for (size_t i = 0; i < numNodes*numSiteRates*numPatterns*numChars; i++) {
		marginalLikelihoods[i] = 0.0;
	}
	
    perNodeSiteLogScalingFactors = std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(numNodes, std::vector<double>(numPatterns, 0.0) ) );
    
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
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::scale( size_t nodeIndex, size_t left, size_t right )
{
    
    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    if ( useScaling == true && nodeIndex % 4 == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            
            // the max probability
            double max = 0.0;
            
            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;
                
                double*          p_site_mixture          = p_node + offset;
                
                for ( size_t i=0; i<this->numChars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }
                
            }
            
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] - log(max);
            
            
            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;
                
                double*          p_site_mixture          = p_node + offset;
                
                for ( size_t i=0; i<this->numChars; ++i)
                {
                    p_site_mixture[i] /= max;
                }
                
            }
            
        }
    }
    else if ( useScaling == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site];
        }
        
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::scale( size_t nodeIndex, size_t left, size_t right, size_t middle )
{
    
    double* p_node   = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    if ( useScaling == true && nodeIndex % 4 == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            
            // the max probability
            double max = 0.0;
            
            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;
                
                double*          p_site_mixture          = p_node + offset;
                
                for ( size_t i=0; i<this->numChars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }
                
            }
            
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site] - log(max);
            
            
            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;
                
                double*          p_site_mixture          = p_node + offset;
                
                for ( size_t i=0; i<this->numChars; ++i)
                {
                    p_site_mixture[i] /= max;
                }
                
            }
            
        }
    }
    else if ( useScaling == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site];
        }
        
    }
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setValue(AbstractDiscreteCharacterData *v, bool force)
{
    
    // delegate to the parent class
    TypedDistribution< AbstractDiscreteCharacterData >::setValue(v, force);
    
    // reset the number of sites
    this->numSites = v->getNumberOfIncludedCharacters();
    
	sitePattern.clear();
	sitePattern.resize(numSites);
	
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
            size_t stateIndex = 0;
            while ( true )
            {
                u -= *freqs;
                ++stateIndex;
                
                if ( u > 0.0 && stateIndex < this->numChars)
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
            std::stringstream ss;
            ss << "Node" << child.getIndex();
            taxon.setTaxonName( ss.str() );
            simulate( child, taxa, perSiteRates );
        }
        
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setClockRate(const TypedDagNode< double > *r)
{
    
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
    
    // add the new parameter
    this->addParameter( homogeneousClockRate );
    
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
    
    // add the new parameter
    this->addParameter( heterogeneousClockRates );
    
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
        this->removeParameter( pInv );
        pInv = NULL;
    }
    
    // set the value
    pInv = r;
    
    // add the new parameter
    this->addParameter( pInv );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setRateMatrix(const TypedDagNode< RateGenerator > *rm) {
    
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
    
    // add the new parameter
    this->addParameter( homogeneousRateMatrix );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::setRateMatrix(const TypedDagNode< RbVector< RateGenerator > > *rm) {
    
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
    
    // add the new parameter
    this->addParameter( heterogeneousRateMatrices );
    
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
        this->removeParameter( rootFrequencies );
        rootFrequencies = NULL;
    }
    
    if ( f != NULL )
    {
        // set the value
        rootFrequencies = f;
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // add the new parameter
    this->addParameter( rootFrequencies );
    
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
        this->removeParameter( siteRates );
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
    
    // add the new parameter
    this->addParameter( siteRates );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}


template<class charType, class treeType>
std::vector< std::vector<double> >* RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::sumMarginalLikelihoods( size_t nodeIndex )
{
    
    std::vector< std::vector<double> >* per_mixture_Likelihoods = new std::vector< std::vector<double> >(this->numPatterns, std::vector<double>(numChars, 0.0) );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node_marginal         = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    double*         p_mixture_marginal          = p_node_marginal;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*         p_site_mixture_marginal         = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            double*         p_site_marginal_j           = p_site_mixture_marginal;
            // iterate over all starting states
            for (size_t j=0; j<numChars; ++j)
            {
                // add the probability of being in this state
                (*per_mixture_Likelihoods)[site][j] += *p_site_marginal_j;
                
                // increment pointers
                ++p_site_marginal_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture_marginal+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture_marginal+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    return per_mixture_Likelihoods;
}




template<class charType, class treeType>
double RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();
    
    // get the index of the root node
    size_t nodeIndex = root.getIndex();
        
    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex] * this->activeLikelihoodOffset  + nodeIndex*this->nodeOffset;
    
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->numPatterns,0.0);
    
    // get pointers the likelihood for both subtrees
    double*   p_mixture     = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*   p_site_mixture     = p_mixture;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointers to the likelihoods for this site and mixture category
            double* p_site_j   = p_site_mixture;
            // iterate over all starting states
            for (size_t i=0; i<numChars; ++i)
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
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            
            if ( useScaling == true )
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
        
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            
            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;

            if ( useScaling == true )
            {
            
                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;
            }
            
        }
        
        
    }
    
    return sumPartialProbs;
}



/** Swap a parameter of the distribution */
template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
        homogeneousRateMatrix = static_cast<const TypedDagNode< RateGenerator >* >( newP );
    }
    else if (oldP == heterogeneousRateMatrices)
    {
        heterogeneousRateMatrices = static_cast<const TypedDagNode< RbVector< RateGenerator > >* >( newP );
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
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::touchSpecialization( DagNode* affecter, bool touchAll )
{
    
    
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



template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateMarginalNodeLikelihoods( void )
{

    // calculate the root marginal likelihood, then start the recursive call down the tree
    this->computeMarginalRootLikelihood();
	
    // update the marginal likelihoods by a recursive downpass
    this->recursiveMarginalLikelihoodComputation( tau->getValue().getRoot().getIndex() );
    
    
}




template<class charType, class treeType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen)
{
    
    // first, get the rate matrix for this branch
    const RateGenerator *rm;
    if ( this->branchHeterogeneousSubstitutionMatrices == true )
    {
        rm = &this->heterogeneousRateMatrices->getValue()[nodeIdx];
    }
    else
    {
        rm = &this->homogeneousRateMatrix->getValue();
    }
    
    // second, get the clock rate for the branch
    double rate;
    if ( this->branchHeterogeneousClockRates == true )
    {
        rate = this->heterogeneousClockRates->getValue()[nodeIdx];
    }
    else
    {
        rate = this->homogeneousClockRate->getValue();
    }
    
    // and finally compute the per site rate transition probability matrix
    const TopologyNode* node = tau->getValue().getNodes()[nodeIdx];
    if (node->isRoot()) throw RbException("ERROR: dnPhyloCTMC called updateTransitionProbabilities for the root node\n");
    double endAge = node->getAge();
    double startAge = endAge + node->getBranchLength();

    if ( this->rateVariationAcrossSites == true )
    {
        const std::vector<double> &r = this->siteRates->getValue();
        for (size_t i = 0; i < this->numSiteRates; ++i)
        {
            rm->calculateTransitionProbabilities( startAge, endAge,  rate * r[i], this->transitionProbMatrices[i] );
        }
    }
    else
    {
        rm->calculateTransitionProbabilities( startAge, endAge,  rate, this->transitionProbMatrices[0] );
    }
    
}






#endif
