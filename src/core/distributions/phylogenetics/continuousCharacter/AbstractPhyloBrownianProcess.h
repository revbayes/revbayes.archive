#ifndef AbstractPhyloBrownianProcess_H
#define AbstractPhyloBrownianProcess_H

#include "ContinuousCharacterData.h"
#include "RbVector.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <memory.h>

namespace RevBayesCore {
    
    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-23, version 1.0
     */
    template<class treeType>
    class AbstractPhyloBrownianProcess : public TypedDistribution< ContinuousCharacterData > {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractPhyloBrownianProcess(const TypedDagNode<treeType> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t nSites );
        virtual                                                            ~AbstractPhyloBrownianProcess(void);                                                     //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractPhyloBrownianProcess*                               clone(void) const = 0;                                                                  //!< Create an independent clone
        virtual double                                                      computeLnProbability(void) = 0;
        
        // non-virtual
        void                                                                setValue(ContinuousCharacterData *v, bool f=false);                                     //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        void                                                                reInitialized(void);
        
    protected:
        // helper method for this and derived classes
        double                                                              computeBranchTime(size_t nodeIdx, double brlen);
        double                                                              computeSiteRate(size_t siteIdx);
        virtual void                                                        resetValue(void);
        
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter
        
        // members
        double                                                              lnProb;
        size_t                                                              numNodes;
        size_t                                                              numSites;
        const TypedDagNode<treeType>*                                       tau;
        
        // members
        const TypedDagNode< double >*                                       homogeneousClockRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousClockRates;
        const TypedDagNode< double >*                                       homogeneousSiteRate;
        const TypedDagNode< RbVector< double > >*                           heterogeneousSiteRates;
        
        
    private:
        
        // private methods
        void                                                                simulate(const TopologyNode& node, std::vector< ContinuousTaxonData > &t);
        
    };
    
}

#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"

#include <cmath>

template<class treeType>
RevBayesCore::AbstractPhyloBrownianProcess<treeType>::AbstractPhyloBrownianProcess(const TypedDagNode<treeType> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t nSites) :
    TypedDistribution< ContinuousCharacterData >(  new ContinuousCharacterData() ),
    numNodes( t->getValue().getNumberOfNodes() ),
    numSites( nSites ),
    tau( t ),
    homogeneousClockRate( homCR ),
    heterogeneousClockRates( hetCR ),
    homogeneousSiteRate( homSR ),
    heterogeneousSiteRates( hetSR )
{
    
    // initialize with default parameters
    
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( tau );
    this->addParameter( homogeneousClockRate );
    this->addParameter( heterogeneousClockRates );
    this->addParameter( homogeneousSiteRate );
    this->addParameter( heterogeneousSiteRates );
    
    // now we need to reset the value
    this->redrawValue();
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class treeType>
RevBayesCore::AbstractPhyloBrownianProcess<treeType>::~AbstractPhyloBrownianProcess( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
}


template<class treeType>
double RevBayesCore::AbstractPhyloBrownianProcess<treeType>::computeBranchTime( size_t nodeIdx, double brlen )
{
    
    // second, get the clock rate for the branch
    double branchTime;
    if ( this->heterogeneousClockRates != NULL )
    {
        branchTime = this->heterogeneousClockRates->getValue()[nodeIdx] * brlen;
    }
    else
    {
        branchTime = this->homogeneousClockRate->getValue() * brlen;
    }
    
    return branchTime;
}


template<class treeType>
double RevBayesCore::AbstractPhyloBrownianProcess<treeType>::computeSiteRate(size_t siteIdx)
{
    
    // second, get the clock rate for the branch
    double siteRate;
    if ( this->heterogeneousSiteRates != NULL )
    {
        siteRate = this->heterogeneousSiteRates->getValue()[siteIdx];
    }
    else
    {
        siteRate = this->homogeneousSiteRate->getValue();
    }
    
    return siteRate;
}



template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::redrawValue( void )
{
    
    // delete the old value first
    delete this->value;
    
    // create a new character data object
    this->value = new ContinuousCharacterData();
    
    // create a vector of taxon data
    std::vector< ContinuousTaxonData > taxa = std::vector< ContinuousTaxonData >( numNodes, ContinuousTaxonData("") );
    
    // first, simulate the per site rates
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // simulate the root sequence
    ContinuousTaxonData &root = taxa[ tau->getValue().getRoot().getIndex() ];
    for ( size_t i = 0; i < numSites; ++i )
    {
        // create the character
        double c = rng->uniform01();
        
        // add the character to the sequence
        root.addCharacter( c );
    }
    
    // recursively simulate the sequences
    simulate( tau->getValue().getRoot(), taxa );
    
    // add the taxon data to the character data
    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
    {
        this->value->addTaxonData( taxa[i] );
    }
    
    // tell the derived classes
    this->resetValue();
}


template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::reInitialized( void )
{
    
    // tell the derived classes
    this->resetValue();
}


template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::resetValue( void )
{
    // nothing to do here.
    // derived class may overwrite this method
}


template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::setValue(ContinuousCharacterData *v, bool force)
{
    
    // delegate to the parent class
    TypedDistribution< ContinuousCharacterData >::setValue(v, force);
    
    // reset the number of sites
    this->numSites = v->getNumberOfIncludedCharacters();
    
    // tell the derived classes
    this->resetValue();
    
}


template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::simulate( const TopologyNode &node, std::vector< ContinuousTaxonData > &taxa)
{
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    const ContinuousTaxonData &parent = taxa[ nodeIndex ];
    
    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);
        
        // get the branch length for this child
        double branchLength = child.getBranchLength();
        
        // get the branch specific rate
        double branchSigma = sqrt( computeBranchTime( child.getIndex(), branchLength ) );
        
        ContinuousTaxonData &taxon = taxa[ child.getIndex() ];
        for ( size_t i = 0; i < numSites; ++i )
        {
            // get the ancestral character for this site
            double parentState = parent.getCharacter( i );
            
            // compute the standard deviation for this site
            double standDev = branchSigma * computeSiteRate( i );
            
            // create the character
            double c = RbStatistics::Normal::rv( parentState, standDev, *rng);
            
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
            simulate( child, taxa );
        }
        
    }
    
}





/** Swap a parameter of the distribution */
template<class treeType>
void RevBayesCore::AbstractPhyloBrownianProcess<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == homogeneousClockRate)
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates)
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == homogeneousSiteRate)
    {
        homogeneousSiteRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousSiteRates)
    {
        heterogeneousSiteRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
        numNodes = tau->getValue().getNumberOfNodes();
    }
    
}





#endif
