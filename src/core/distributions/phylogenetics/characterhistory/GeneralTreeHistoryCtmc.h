//
//  GeneralTreeHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/29/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeneralTreeHistoryCtmc__
#define __rb_mlandis__GeneralTreeHistoryCtmc__

#include "AbstractTreeHistoryCtmc.h"
#include "RateMap_Biogeography.h"
#include "ContinuousCharacterData.h"
#include "DistributionExponential.h"
#include "RateMap.h"
#include "RbConstants.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

#include "ConstantNode.h"
#include "DiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "PathUniformizationSampleProposal.h"

#include <cmath>
#include <cstring>

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class GeneralTreeHistoryCtmc : public AbstractTreeHistoryCtmc<charType, treeType> {
        
    public:
        GeneralTreeHistoryCtmc(const TypedDagNode< treeType > *t, size_t nChars, size_t nSites, bool useAmbigChar=false);
        GeneralTreeHistoryCtmc(const GeneralTreeHistoryCtmc &n);                                                                         //!< Copy constructor
        virtual                                            ~GeneralTreeHistoryCtmc(void);                                                //!< Virtual destructor
        
        // public member functions
        
        GeneralTreeHistoryCtmc*                             clone(void) const;                                                           //!< Create an independent clone
        void                                                initializeValue(void);
        void                                                redrawValue(void);
        virtual void                                        simulate(void);
        
        bool                                                samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet);
        
        void                                                setRateMap(const TypedDagNode< RateMap > *rm);
        void                                                setRateMap(const TypedDagNode< RbVector< RateMap > > *rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                setSiteRates(const TypedDagNode< std::vector< double > > *r);
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    protected:
        
        virtual double                                      computeRootLikelihood(const TopologyNode &n);
        virtual double                                      computeInternalNodeLikelihood(const TopologyNode &n);
        virtual double                                      computeTipLikelihood(const TopologyNode &node);
        virtual const std::vector<double>&                  getRootFrequencies(void);
        // (not needed)        void                         keepSpecialization(DagNode* affecter);
        // (not needed)        void                         restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher);
        
        
    private:
        // members
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
        const TypedDagNode< std::vector< double > >*        siteRates;
        const TypedDagNode< RateMap >*                      homogeneousRateMap;
        const TypedDagNode< RbVector< RateMap > >*          heterogeneousRateMaps;
        
        // flags specifying which model variants we use
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                rateVariationAcrossSites;
  
        virtual void                                        simulate(const TopologyNode& node, BranchHistory* bh, std::vector< DiscreteTaxonData< charType > >& taxa);
        void                                                simulateHistory(const TopologyNode& node, BranchHistory* bh);
        unsigned*                                           computeCounts(const std::vector<CharacterEvent*>& s);
        
    };
    
}



#include "RbConstants.h"

template<class charType, class treeType>
RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::GeneralTreeHistoryCtmc(const TypedDagNode<treeType> *t, size_t nChars, size_t nSites, bool useAmbigChar) : AbstractTreeHistoryCtmc<charType, treeType>(  t, nChars, nSites, useAmbigChar ) {
    
    // initialize with default parameters
    rootFrequencies             = new ConstantNode<std::vector<double> >("rootFrequencies", new std::vector<double>(nChars, 1.0/nChars));
    siteRates                   = NULL;
    homogeneousRateMap          = NULL;
    heterogeneousRateMaps       = NULL;

    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;
}


template<class charType, class treeType>
RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::GeneralTreeHistoryCtmc(const GeneralTreeHistoryCtmc &d) : AbstractTreeHistoryCtmc<charType, treeType>( d ) {
    // initialize with default parameters
    rootFrequencies             = d.rootFrequencies;
    siteRates                   = d.siteRates;
    homogeneousRateMap          = d.homogeneousRateMap;
    heterogeneousRateMaps       = d.heterogeneousRateMaps;
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = d.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = d.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = d.rateVariationAcrossSites;
}


template<class charType, class treeType>
RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::~GeneralTreeHistoryCtmc( void )
{
    ; // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template<class charType, class treeType>
RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>* RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::clone( void ) const {
    
    return new GeneralTreeHistoryCtmc<charType, treeType>( *this );
}

template<class charType, class treeType>
unsigned* RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::computeCounts(const std::vector<CharacterEvent*>& s)
{
    unsigned counts[ this->numChars ];
    for (size_t i = 0; i < s.size(); i++)
        counts[i] = 0;
    for (size_t i = 0; i < s.size(); i++)
        counts[ s[i]->getState() ] += 1;
    return counts;
}

template<class charType, class treeType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::computeRootLikelihood(const TopologyNode &n)
{
    // Generalize approach from Murray et al. 2006?
    double lnP = 0.0;
    
    BranchHistory* bh = this->histories[n.getIndex()];
    const std::vector<CharacterEvent*> rootState = bh->getChildCharacters();
    
    // get counts per state
    std::vector<int> counts(this->numChars, 0);
    for (size_t i = 0; i < rootState.size(); i++)
        counts[ rootState[i]->getState() ]++;
    
    // get log prob
    const std::vector<double> rf = homogeneousRateMap->getValue().getRootFrequencies();
    for (size_t i = 0; i < counts.size(); i++)
        lnP += counts[i] * log( rf[i] );
    
    return lnP;
}

template<class charType, class treeType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node)
{
    if (node.isRoot())
        return 0.0;
    
    size_t nodeIndex = node.getIndex();
    double branchLength = node.getBranchLength();
    const RateMap& rm = homogeneousRateMap->getValue();
    
    BranchHistory* bh = this->histories[nodeIndex];
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned* counts = computeCounts(currState);

    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // stepwise events
    double lnL = 0.0;
    double t = 0.0;
    double dt = 0.0;
    double da = 0.0;
    
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        unsigned s = (*it_h)->getState();
        
        // lnL for stepwise events for p(x->y)
        double tr = rm.getRate(node, currState, *it_h, counts);
        double sr = rm.getSumOfRates(node, currState, counts);
        lnL += log(tr) -sr * dt * branchLength;
        
        // update counts
        counts[currState[idx]->getState()] -= 1;
        counts[s] += 1;
        
        // update time and state
        currState[idx] = *it_h;
        t += dt;
    }
    
    // lnL that nothing else happens
    double sr = rm.getSumOfRates(node, currState, counts);
    lnL += -sr * ( (1.0 - t) * branchLength );
    
    return lnL;
}



template<class charType, class treeType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::computeTipLikelihood(const TopologyNode &node)
{
    double lnL = 0.0;
    return lnL;
}

template<class charType, class treeType>
const std::vector<double>& RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::getRootFrequencies( void ) {
    
    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL )
    {
        return rootFrequencies->getValue();
    }
    else
    {
        throw RbException("BUG: Reached end of a non-void function in GeneralTreeHistoryCtmc.");
    }
    
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::initializeValue( void )
{
    //    if (this->dagNode->isClamped())
    {
        std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType,treeType>::tau->getValue().getNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            TopologyNode* node = nodes[i];
            if (node->isTip())
            {
                DiscreteTaxonData<charType>& d = static_cast< DiscreteTaxonData<charType>& >( this->value->getTaxonData( node->getName() ) );
                
                std::vector<CharacterEvent*> tipState;
                for (size_t j = 0; j < d.size(); j++)
                {
                    unsigned s = 0;
//                    if (!this->usingAmbiguousCharacters)
                    s = (unsigned)d[j].getStateIndex();
                    
                    CharacterEvent* evt = new CharacterEvent(j, s, 1.0);
                    tipState.push_back( evt );
                }
                
                this->histories[node->getIndex()]->setChildCharacters(tipState);
                tipState.clear();
            }
        }
        this->tipsInitialized = true;
    }
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::redrawValue( void )
{
    if (this->tipsInitialized == false)
        initializeValue();
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < this->numSites; i++)
        indexSet.insert(i);
    
    // sample node states
    std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType,treeType>::tau->getValue().getNodes();
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];
        
        int samplePathEndCount = 0;
        do
        {
            samplePathEndCount++;
        } while (samplePathEnd(*nd,indexSet) == false && samplePathEndCount < 100);
        
        int samplePathStartCount = 0;
        do
        {
            samplePathStartCount++;
        } while (samplePathStart(*nd,indexSet) == false && samplePathStartCount < 100);
    }
    
    // sample paths
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];
        
        int samplePathHistoryCount = 0;
        do
        {
            
            samplePathHistoryCount++;
        } while (samplePathHistory(*nd,indexSet) == false && samplePathHistoryCount < 100);
        
        this->histories[i]->print();
    }
    
    double lnL = this->computeLnProbability();
    
    if (lnL == RbConstants::Double::neginf)
    {
        for (size_t i = 0; i < nodes.size(); i++)
        {
            this->fireTreeChangeEvent(*nodes[i]);
        }
        redrawValue();
    }
}



template<class charType, class treeType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    if (node.isTip())
    {
        return true; // do nothing
    }
    else
    {
        TransitionProbabilityMatrix leftTpMatrix(this->numChars);
        TransitionProbabilityMatrix rightTpMatrix(this->numChars);
//        TransitionProbabilityMatrix ancTpMatrix(this->numChars);
        
        const RateMap& rm = homogeneousRateMap->getValue();
        
        // for sampling probs
        const std::vector<CharacterEvent*>& leftChildState  = this->histories[node.getChild(0).getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = this->histories[node.getChild(1).getIndex()]->getChildCharacters();
        
        // to update
        std::vector<CharacterEvent*> nodeChildState = this->histories[node.getIndex()]->getChildCharacters();
        for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
        {
            rm.calculateTransitionProbabilities(node.getChild(0), leftTpMatrix, *it);
            rm.calculateTransitionProbabilities(node.getChild(1), rightTpMatrix, *it);
            
            unsigned int desS1 = leftChildState[*it]->getState();
            unsigned int desS2 = rightChildState[*it]->getState();
            
            std::vector<double> g(this->numChars, 0.0);
            double gSum = 0.0;
            for (size_t i = 0; i < this->numChars; i++)
            {
                g[i] = leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                gSum += g[i];
            }
            
            double u = GLOBAL_RNG->uniform01() * gSum;
            unsigned int s = 0;
            for (size_t i = 0; i < this->numChars; i++)
            {
                u -= g[i];
                if (u <= 0.0)
                {
                    s = i;
                    break;
                }
            }
            
            nodeChildState[*it]->setState(s);
        }
    }
    return true;
}


template<class charType, class treeType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    
    if (node.isRoot())
        return true;
    
    PathRejectionSampleProposal<charType,treeType> p(   this->getStochasticNode(),
                                                        const_cast<StochasticNode<treeType>* >(  static_cast<const StochasticNode<treeType>* >(this->tau)),
                                                        const_cast<DeterministicNode<RateMap>* >( static_cast<const DeterministicNode<RateMap>* >(homogeneousRateMap)),
                                                        1.0);
    
//    PathUniformizationSampleProposal<charType,treeType> p(   this->getStochasticNode(),
//                                                             const_cast<StochasticNode<treeType>* >(  static_cast<const StochasticNode<treeType>* >(this->tau)),
//                                                             const_cast<DeterministicNode<RateMap>* >( static_cast<const DeterministicNode<RateMap>* >(homogeneousRateMap)),
//                                                             1.0);

    
//    ConstantNode<RateMatrix>* q_sample = new ConstantNode<RateMatrix*>("q_sample", new RateMatrix_Blosum62());
//    PathRejectionSampleProposal<charType,treeType> p(   this->getStochasticNode(),
//                                                     const_cast<StochasticNode<treeType>* >(  static_cast<const StochasticNode<treeType>* >(this->tau)),
//                                                     const_cast<DeterministicNode<RateMap>* >( static_cast<const DeterministicNode<RateMap>* >(homogeneousRateMap)),
//                                                     1.0);

    
    p.assignNode(const_cast<TopologyNode*>(&node));
    p.assignSiteIndexSet(indexSet);
    
    
//    BranchHistory* bh = this->histories[ node.getIndex() ];
//    std::cout << "Before samplePathHistory() " << node.getIndex() << "\n";
//    bh->print();
    
    p.prepareProposal();
    p.doProposal();
    p.cleanProposal();

//    std::cout << "After samplePathHistory() " << node.getIndex() << "\n";
//    bh->print();
    
    return true;
}


template<class charType, class treeType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    
    // ignore tips
    if (node.isTip())
        return true;
    
    // update daughter lineages' parent states with the current node's child state
    const std::vector<CharacterEvent*>& nodeChildState = this->histories[ node.getIndex() ]->getChildCharacters();
    const std::vector<TopologyNode*>& children = node.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        std::vector<CharacterEvent*> childParentState(nodeChildState.size());
        for (size_t j = 0; j < childParentState.size(); j++)
        {
            childParentState[j] = new CharacterEvent(j, nodeChildState[j]->getState(), 0.0);
        }
        this->histories[ children[i]->getIndex() ]->setParentCharacters( childParentState );
    }
    
    return true;
}


template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::setRateMap(const TypedDagNode< RateMap > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMap != NULL )
        homogeneousRateMap = NULL;
    else
        heterogeneousRateMaps = NULL;
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateMap = rm;
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::setRateMap(const TypedDagNode< RbVector< RateMap > > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMap != NULL )
    {
        this->removeParameter( homogeneousRateMap );
        homogeneousRateMap = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateMaps );
        heterogeneousRateMaps = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateMaps = rm;
    
    // add the parameter
    this->addParameter( heterogeneousRateMaps );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::setRootFrequencies(const TypedDagNode< std::vector< double > > *f) {
    
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
        
        // add the parameter
        this->addParameter( rootFrequencies );
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}


template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::setSiteRates(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( siteRates != NULL )
        siteRates = NULL;
    
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
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::simulate(void)
{
    this->RevBayesCore::AbstractTreeHistoryCtmc<charType,treeType>::simulate();
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::simulateHistory(const TopologyNode& node, BranchHistory* bh)
{
    /*
    unsigned nodeIndex = node.getIndex();
    
    const RateMap_Biogeography* rm;
    if (branchHeterogeneousSubstitutionMatrices)
        rm = &static_cast<const RateMap_Biogeography&>(heterogeneousRateMaps->getValue()[nodeIndex]);
    else
        rm = &static_cast<const RateMap_Biogeography&>(homogeneousRateMap->getValue());
    
    // get parent BranchHistory state
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    
    // simulate path
    std::set<CharacterEvent*,CharacterEventCompare> history;
    double startAge = ( node.isRoot() ? node.getAge()*5 : node.getParent().getAge() );
    double branchLength = startAge - node.getAge();
    
    
    bool failed = false;
    std::vector<double> rates(this->numSites, 0.0);
    std::multiset<CharacterEvent*> tmpHistory;
    do
    {
        // delete previously rejected events
        tmpHistory.clear();
        failed = false;
        int n1 = (int)numOn(currState);
        
        
        // simulate
        double t = 0.0;
        
        double currAge = startAge;
        int epochIdx = getEpochIndex(startAge);
        double epochAge = epochs[epochIdx];
        
        while (t < 1.0)
        {
            // get competing event rates
            double r = 0.0;
            for (size_t i = 0; i < this->numSites; i++)
            {
                unsigned s = ( currState[i]->getState() == 0 ? 1 : 0 );
                CharacterEvent nextState(i, s, currAge);
                rates[i] = rm->getRate(node, currState, &nextState, currAge);
                r += rates[i];
            }
            
            // next event time
            double dt = 0.0;
            if (r > 0.0)
                dt = RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
            double da = dt * branchLength;
            
            // sample time from next interval (by memorylessness)
            if (currAge - da < epochAge || r == 0.0)
            {
                t = (startAge - epochAge) / branchLength;
                currAge = epochAge;
                epochIdx++;
                epochAge = epochs[epochIdx];
            }
            // or, within this interval
            else
            {
                t += dt;
                currAge -= da;
                
                if (t < 1.0)
                {
                    CharacterEvent* evt = NULL;
                    double u = GLOBAL_RNG->uniform01() * r;
                    for (size_t i = 0; i < rates.size(); i++)
                    {
                        u -= rates[i];
                        if (u <= 0.0)
                        {
                            unsigned s = 0;
                            if (currState[i]->getState() == 0)
                                s = 1;
                            
                            if (s == 0)
                                n1--;
                            else if (s == 1)
                                n1++;
                            if (n1 == 0)
                                failed = true;
                            
                            evt = new CharacterEvent(i,s,t);
                            break;
                        }
                    }
                    tmpHistory.insert(evt);
                    currState[ evt->getIndex() ] = evt;
                }
            }
        }
        
        if (failed)
        {
            for (std::multiset<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
                delete *it_h;
        }
        
    } while (failed);
    
    for (std::multiset<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
    {
        history.insert(*it);
    }
    bh->setHistory(history);
    
    for (size_t i = 0; i < this->numSites; i++)
    {
        unsigned s = currState[i]->getState();
        currState[i] = new CharacterEvent(i, s, 1.0);
    }
    
    bh->setChildCharacters(currState);
    */
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::simulate(const TopologyNode& node, BranchHistory* bh, std::vector< DiscreteTaxonData< charType > >& taxa)
{

    /*
     
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    
    // get rate map for branch leading to node
    const RateMap* rm;
    if (branchHeterogeneousSubstitutionMatrices)
        rm = &heterogeneousRateMaps->getValue()[nodeIndex];
    else
        rm = &homogeneousRateMap->getValue();
    
    // if root, set tail state
    if (node.isRoot())
    {
        std::vector<CharacterEvent*> parentState;
        for (size_t i = 0; i < this->numSites; i++)
        {
            unsigned s = 0;
            if (rm->isAreaAvailable(i, node.getAge()))
                s = 1;
            parentState.push_back(new CharacterEvent(i, s, node.getAge()*5));
        }
        
        bh->setParentCharacters(parentState);
    }
    
    // simulate anagenic changes
    simulateHistory(node, bh);
    //    bh->print();
    
    const std::vector<CharacterEvent*>& childState = bh->getChildCharacters();
    for ( size_t i = 0; i < this->numSites; ++i )
    {
        // create the character
        charType c;
        std::string s = "0";
        if (childState[i]->getState() == 1)
            s = "1";
        c.setState( s );
        //        c.setToFirstState();
        //        for (size_t j = 0; j < childState[i]->getState(); j++)
        //            c++;
        
        // add the character to the sequence
        taxa[nodeIndex].addCharacter( c );
    }
    
    if ( node.isTip() )
    {
        std::cout << "adding " << node.getName() << "\n";
        taxa[nodeIndex].setTaxonName( node.getName() );
    }
    else
    {
        const std::vector<TopologyNode*>& children = node.getChildren();
        for (size_t i = 0; i < children.size(); i++)
        {
            this->histories[ children[i]->getIndex() ] = new BranchHistory(this->numSites, this->numChars, children[i]->getIndex() );
        }
        
        simulateCladogenesis(node);
        
        for (size_t i = 0; i < children.size(); i++)
        {
            BranchHistory* bh_ch = this->histories[ children[i]->getIndex() ];
            simulate( *children[i], bh_ch, taxa );
        }
    }
    */
}


/** Get the parameters of the distribution */
template<class charType, class treeType>
std::set<const RevBayesCore::DagNode*> RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::getParameters( void ) const
{
    std::set<const DagNode*> parameters = AbstractTreeHistoryCtmc<charType, treeType>::getParameters();
    
    parameters.insert( homogeneousRateMap );
    parameters.insert( heterogeneousRateMaps );
    parameters.insert( rootFrequencies );
    parameters.insert( siteRates );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::swapParameter( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == homogeneousRateMap)
    {
        homogeneousRateMap = static_cast<const TypedDagNode< RateMap >* >( newP );
    }
    else if (oldP == heterogeneousRateMaps)
    {
        heterogeneousRateMaps = static_cast<const TypedDagNode< RbVector< RateMap > >* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == siteRates)
    {
        siteRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType, treeType>::swapParameter(oldP,newP);
    }
    
}

template<class charType, class treeType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == rootFrequencies || affecter == rootFrequencies )
    {
        
        const TopologyNode &root = this->tau->getValue().getRoot();
        this->flagNodeDirty( root );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType, treeType>::touchSpecialization( affecter );
    }
    
}

#endif /* defined(__rb_mlandis__GeneralTreeHistoryCtmc__) */
