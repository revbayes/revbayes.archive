#ifndef GeneralTreeHistoryCtmc_H
#define GeneralTreeHistoryCtmc_H

#include "AbstractTreeHistoryCtmc.h"
#include "RateGeneratorSequence_Biogeography.h"
#include "ContinuousCharacterData.h"
#include "DistributionExponential.h"
#include "RateGeneratorSequence.h"
#include "RbConstants.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

#include "ConstantNode.h"
#include "HomologousDiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "PathUniformizationSampleProposal.h"
#include "PathRejectionSampleProposal.h"

#include <cmath>
#include <cstring>

namespace RevBayesCore {

    template<class charType>
    class GeneralTreeHistoryCtmc : public AbstractTreeHistoryCtmc<charType> {

    public:
        GeneralTreeHistoryCtmc(const TypedDagNode< Tree > *t, size_t nChars, size_t nSites, bool useAmbigChar=false);
        GeneralTreeHistoryCtmc(const GeneralTreeHistoryCtmc &n);                                                                         //!< Copy constructor
        virtual                                            ~GeneralTreeHistoryCtmc(void);                                                //!< Virtual destructor

        // public member functions

        GeneralTreeHistoryCtmc*                             clone(void) const;                                                           //!< Create an independent clone
        void                                                initializeTipValues(void);
        void                                                drawInitValue(void);
        void                                                redrawValue(void);
        virtual void                                        simulate(void);

        bool                                                samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet);

        void                                                setRateGeneratorSequence(const TypedDagNode< RateGeneratorSequence > *rm);
        void                                                setRateGeneratorSequence(const TypedDagNode< RbVector< RateGeneratorSequence > > *rm);
        void                                                setRootFrequencies(const TypedDagNode< RbVector< double > > *f);
        void                                                setSiteRates(const TypedDagNode< RbVector< double > > *r);

    protected:

        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

        virtual double                                      computeRootLikelihood(const TopologyNode &n);
        virtual double                                      computeInternalNodeLikelihood(const TopologyNode &n);
        virtual double                                      computeTipLikelihood(const TopologyNode &node);
        virtual const std::vector<double>&                  getRootFrequencies(void);
        // (not needed)        void                         keepSpecialization(DagNode* affecter);
        // (not needed)        void                         restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);


    private:
        // members
        const TypedDagNode< RbVector< double > >*           rootFrequencies;
        const TypedDagNode< RbVector< double > >*           siteRates;
        const TypedDagNode< RateGeneratorSequence >*                      homogeneousRateGeneratorSequence;
        const TypedDagNode< RbVector< RateGeneratorSequence > >*          heterogeneousRateGeneratorSequences;

        // flags specifying which model variants we use
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                rateVariationAcrossSites;

        virtual void                                        simulate(const TopologyNode& node, BranchHistory* bh, std::vector< DiscreteTaxonData< charType > >& taxa);
        void                                                simulateHistory(const TopologyNode& node, BranchHistory* bh);
        unsigned*                                           computeCounts(const std::vector<CharacterEvent*>& s, unsigned* counts);

    };

}



#include "RbConstants.h"

template<class charType>
RevBayesCore::GeneralTreeHistoryCtmc<charType>::GeneralTreeHistoryCtmc(const TypedDagNode<Tree> *t, size_t nChars, size_t nSites, bool useAmbigChar) : AbstractTreeHistoryCtmc<charType>(  t, nChars, nSites, useAmbigChar ) {

    // initialize with default parameters
    rootFrequencies             = new ConstantNode< RbVector<double> >("rootFrequencies", new RbVector<double>(nChars, 1.0/nChars));
    siteRates                   = NULL;
    homogeneousRateGeneratorSequence          = NULL;
    heterogeneousRateGeneratorSequences       = NULL;

    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;

    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( homogeneousRateGeneratorSequence );
    this->addParameter( heterogeneousRateGeneratorSequences );
    this->addParameter( rootFrequencies );
    this->addParameter( siteRates );
}


template<class charType>
RevBayesCore::GeneralTreeHistoryCtmc<charType>::GeneralTreeHistoryCtmc(const GeneralTreeHistoryCtmc &d) : AbstractTreeHistoryCtmc<charType>( d ) {
    // initialize with default parameters
    rootFrequencies             = d.rootFrequencies;
    siteRates                   = d.siteRates;
    homogeneousRateGeneratorSequence          = d.homogeneousRateGeneratorSequence;
    heterogeneousRateGeneratorSequences       = d.heterogeneousRateGeneratorSequences;

    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = d.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = d.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = d.rateVariationAcrossSites;
}


template<class charType>
RevBayesCore::GeneralTreeHistoryCtmc<charType>::~GeneralTreeHistoryCtmc( void )
{
    ; // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template<class charType>
RevBayesCore::GeneralTreeHistoryCtmc<charType>* RevBayesCore::GeneralTreeHistoryCtmc<charType>::clone( void ) const {

    return new GeneralTreeHistoryCtmc<charType>( *this );
}

template<class charType>
unsigned* RevBayesCore::GeneralTreeHistoryCtmc<charType>::computeCounts(const std::vector<CharacterEvent*>& s, unsigned* counts)
{
    for (size_t i = 0; i < this->numChars; i++)
        counts[i] = 0;

    for (size_t i = 0; i < s.size(); i++)
        counts[ s[i]->getState() ] += 1;

    return counts;
}

template<class charType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType>::computeRootLikelihood(const TopologyNode &n)
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
    const std::vector<double>& rf = rootFrequencies->getValue();
    for (size_t i = 0; i < counts.size(); i++)
    {
        lnP += counts[i] * log( rf[i] );
    }

    return lnP;
}

template<class charType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType>::computeInternalNodeLikelihood(const TopologyNode &node)
{
    if (node.isRoot())
        return 0.0;

    size_t nodeIndex = node.getIndex();
    double branchLength = node.getBranchLength();
    const RateGeneratorSequence& rm = homogeneousRateGeneratorSequence->getValue();

    BranchHistory* bh = this->histories[nodeIndex];
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned counts[this->numChars];
    computeCounts(currState, counts);

    // check parent and child states to make sure they match with the
    // ancestral and descendant branches; otherwise, return -Inf
    std::vector<CharacterEvent*> end_state = bh->getChildCharacters();
    for(size_t i = 0; i < node.getNumberOfChildren(); ++i)
    {
        const TopologyNode &child = node.getChild(i);
        size_t child_index = child.getIndex();
        BranchHistory* child_bh = this->histories[child_index];
        std::vector<CharacterEvent*> child_state = child_bh->getParentCharacters();
        for(size_t j = 0; j < this->numSites; ++j)
        {
            if(end_state[j]->getState() != child_state[j]->getState())
            {
                // std::cerr << "Oh oh Mike!!!" << std::endl;
                // std::cerr << end_state[j]->getState() << " -- " << child_state[j]->getState() << std::endl;
                return RbConstants::Double::neginf;
            }
        }
    }

    const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;

    // stepwise events
    double lnL = 0.0;
    double t = 0.0;
    double dt = 0.0;
//    double da = 0.0;

    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        // next event time
        double idx = (*it_h)->getSiteIndex();
        dt = (*it_h)->getAge() - t; // CHECK THIS AGE
        size_t s = (*it_h)->getState();

        // lnL for stepwise events for p(x->y)
//        double tr = rm.getRate(currState, *it_h, counts);
        double tr = rm.getRate(currState, *it_h);
        double sr = rm.getSumOfRates(currState, counts);
        lnL += log(tr) -sr * dt * branchLength;

        // update counts
        counts[currState[idx]->getState()] -= 1;
        counts[s] += 1;

        // update time and state
        currState[idx] = *it_h;
        t += dt;
    }

    // lnL that nothing else happens
    double sr = rm.getSumOfRates(currState, counts);
    lnL += -sr * ( (1.0 - t) * branchLength );

    return lnL;
}



template<class charType>
double RevBayesCore::GeneralTreeHistoryCtmc<charType>::computeTipLikelihood(const TopologyNode &node)
{
    double lnL = 0.0;
    return lnL;
}

template<class charType>
const std::vector<double>& RevBayesCore::GeneralTreeHistoryCtmc<charType>::getRootFrequencies( void ) {

    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL )
    {
        return rootFrequencies->getValue();
    }
    else
    {
        throw RbException("BUG: Reached end of a non-void function in GeneralTreeHistoryCtmc.");
    }

}

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::initializeTipValues( void )
{
    //    if (this->dagNode->isClamped())
    {
        std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType>::tau->getValue().getNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            TopologyNode* node = nodes[i];
            if (node->isTip())
            {
                DiscreteTaxonData<charType>& d = static_cast< DiscreteTaxonData<charType>& >( this->value->getTaxonData( node->getName() ) );

                std::vector<CharacterEvent*> tipState;
                for (size_t j = 0; j < d.getNumberOfCharacters(); j++)
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

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::drawInitValue( void )
{
    if (this->tipsInitialized == false)
        initializeTipValues();

    std::set<size_t> indexSet;
    for (size_t i = 0; i < this->numSites; i++)
        indexSet.insert(i);

    // sample node states
    std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType>::tau->getValue().getNodes();
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
        drawInitValue();
    }
}


template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::redrawValue( void )
{
    if (!true)
    {
        if (this->tipsInitialized == false)
            initializeTipValues();

        std::set<size_t> indexSet;
        for (size_t i = 0; i < this->numSites; i++)
            indexSet.insert(i);

        // sample node states
        std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType>::tau->getValue().getNodes();
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
    else
    {
//        if (this->dagNode->isClamped())
//            initializeTipValues();

        simulate();
    }
}



template<class charType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType>::samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet)
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

        const RateGeneratorSequence& rm = homogeneousRateGeneratorSequence->getValue();

        // for sampling probs
        const std::vector<CharacterEvent*>& leftChildState  = this->histories[node.getChild(0).getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = this->histories[node.getChild(1).getIndex()]->getChildCharacters();

        // to update
        std::vector<CharacterEvent*> nodeChildState = this->histories[node.getIndex()]->getChildCharacters();
        for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
        {
            rm.calculateTransitionProbabilities(leftTpMatrix, *it);
            rm.calculateTransitionProbabilities(rightTpMatrix, *it);

            size_t desS1 = leftChildState[*it]->getState();
            size_t desS2 = rightChildState[*it]->getState();

            std::vector<double> g(this->numChars, 0.0);
            double gSum = 0.0;
            for (size_t i = 0; i < this->numChars; i++)
            {
                g[i] = leftTpMatrix[i][desS1] * rightTpMatrix[i][desS2];
                gSum += g[i];
            }

            double u = GLOBAL_RNG->uniform01() * gSum;
            unsigned int s = 0;
            for (unsigned i = 0; i < this->numChars; i++)
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


template<class charType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType>::samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet)
{

    if ( node.isRoot() == true )
    {
        return true;
    }

    PathRejectionSampleProposal<charType> p( this->getStochasticNode() );
    p.setRateGenerator( homogeneousRateGeneratorSequence );

    p.assignNode(const_cast<TopologyNode*>(&node));

    p.prepareProposal();
    p.doProposal();
    p.cleanProposal();

    return true;
}


template<class charType>
bool RevBayesCore::GeneralTreeHistoryCtmc<charType>::samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet)
{

    // ignore tips
    if ( node.isTip() == true )
    {
        return true;
    }

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


template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::setRateGeneratorSequence(const TypedDagNode< RateGeneratorSequence > *rm) {

    // remove the old parameter first
    if ( homogeneousRateGeneratorSequence != NULL )
    {
        this->removeParameter( homogeneousRateGeneratorSequence );
        homogeneousRateGeneratorSequence = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateGeneratorSequences );
        heterogeneousRateGeneratorSequences = NULL;
    }

    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateGeneratorSequence = rm;

    // add the new parameter
    this->addParameter( homogeneousRateGeneratorSequence );

    // redraw the current value
    if ( this->dag_node != NULL && !this->dag_node->isClamped() )
    {
        this->redrawValue();
    }

}

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::setRateGeneratorSequence(const TypedDagNode< RbVector< RateGeneratorSequence > > *rm) {

    // remove the old parameter first
    if ( homogeneousRateGeneratorSequence != NULL )
    {
        this->removeParameter( homogeneousRateGeneratorSequence );
        homogeneousRateGeneratorSequence = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateGeneratorSequences );
        heterogeneousRateGeneratorSequences = NULL;
    }

    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateGeneratorSequences = rm;

    // add the parameter
    this->addParameter( heterogeneousRateGeneratorSequences );

    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }

}


template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::setRootFrequencies(const TypedDagNode< RbVector< double > > *f) {

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


template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::setSiteRates(const TypedDagNode< RbVector< double > > *r) {

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
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::simulate(void)
{
    this->RevBayesCore::AbstractTreeHistoryCtmc<charType>::simulate();
}


template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::simulateHistory(const TopologyNode& node, BranchHistory* bh)
{
    double branchLength = node.getBranchLength();
    const RateGeneratorSequence& rm = homogeneousRateGeneratorSequence->getValue();

    // get parent BranchHistory state
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned counts[ this->numChars ];
    computeCounts(currState, counts);
    std::set<CharacterEvent*,CharacterEventCompare> history;

//    for (size_t i = 0; i < this->numChars; i++)
//         std::cout << counts[i] << " ";
//    std::cout <<"\n";

    // simulate path
    double t = 0.0;
    double dt = 0.0;
    while (t + dt < 1.0)
    {
        // sample next event time
        double sr = rm.getSumOfRates(currState, counts);
        dt = RbStatistics::Exponential::rv(sr * branchLength, *GLOBAL_RNG);
        if (t + dt < 1.0)
        {
            // next event type
            CharacterEvent* evt = new CharacterEvent(0, 0, t+dt);
            double u = GLOBAL_RNG->uniform01() * sr;

            bool found = false;
            size_t i, s = 0;
            for (i = 0; !found && i < this->numSites; i++)
            {
                evt->setSiteIndex(i);
                for (s = 0; !found && s < this->numChars; s++)
                {
                    // disregard virtual events (self-transitions)
                    if (s != currState[i]->getState())
                    {
                        evt->setState(s);
//                        double r = rm.getRate(currState, evt, counts);
                        double r = rm.getRate(currState, evt);

                        u -= r;
                        if (u <= 0.0)
                        {
                            found = true;
                            history.insert(evt);
                        }
                    }
                    if (found) break;
                }
                if (found) break;
            }

            // update counts
//            std::cout << currState[i]->getState() << " -> " << s << "\n";
//            std::cout << i << " " << s << "\n\n";
            counts[ currState[i]->getState() ] -= 1;
            counts[s] += 1;

            // update history
            t += dt;
            currState[i] = evt;
        }
    }

    bh->setHistory(history);

    for (size_t i = 0; i < this->numSites; i++)
    {
        size_t s = currState[i]->getState();
        currState[i] = new CharacterEvent(i, s, 1.0);
    }

    bh->setChildCharacters(currState);

}

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::simulate(const TopologyNode& node, BranchHistory* bh, std::vector< DiscreteTaxonData< charType > >& taxa)
{


//    RandomNumberGenerator* rng = GLOBAL_RNG;

    // get the sequence of this node
    size_t nodeIndex = node.getIndex();

    // get rate map for branch leading to node
//    const RateGeneratorSequence& rm = homogeneousRateGeneratorSequence->getValue();

    // if root, set tail state
    if (node.isRoot())
    {
        std::vector<CharacterEvent*> childState;
        std::vector<CharacterEvent*> parentState;
        const std::vector<double>& rfs = rootFrequencies->getValue();
        for (size_t i = 0; i < this->numSites; i++)
        {
            unsigned s = 0;
            double u = GLOBAL_RNG->uniform01();
            for (; s < this->numChars; s++)
            {
                u -= rfs[s];
                if (u <= 0.0)
                    break;
            }

            parentState.push_back(new CharacterEvent(i, s, 0.0));
            childState.push_back(new CharacterEvent(i, s, 1.0));
        }

        bh->setParentCharacters(parentState);
        bh->setChildCharacters(childState);

    }

    else
    {
        // simulate anagenic changes
        simulateHistory(node, bh);
//        bh->print();
    }

    const std::vector<CharacterEvent*>& childState = bh->getChildCharacters();
    for ( size_t i = 0; i < this->numSites; ++i )
    {
        // create the character
        charType c;
        c.setState( std::string(1, childState[i]->getState() ) );
        taxa[nodeIndex].addCharacter( c );
    }

    if ( node.isTip() )
    {
//        std::cout << "adding " << node.getName() << "\n";
        taxa[nodeIndex].setTaxon( node.getTaxon() );
    }
    else
    {
        const std::vector<TopologyNode*>& children = node.getChildren();
        for (size_t i = 0; i < children.size(); i++)
        {
            this->histories[ children[i]->getIndex() ] = new BranchHistory(this->numSites, this->numChars, children[i]->getIndex() );
            std::vector<CharacterEvent*> childParentCharacters = this->histories[children[i]->getIndex()]->getParentCharacters();
            for (size_t j = 0; j < this->numSites; j++)
            {
                childParentCharacters[j]->setState( childState[j]->getState() );
            }
        }

        for (size_t i = 0; i < children.size(); i++)
        {
            BranchHistory* bh_ch = this->histories[ children[i]->getIndex() ];
            simulate( *children[i], bh_ch, taxa );
        }
    }
}


/** Swap a parameter of the distribution */
template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == homogeneousRateGeneratorSequence)
    {
        homogeneousRateGeneratorSequence = static_cast<const TypedDagNode< RateGeneratorSequence >* >( newP );
    }
    else if (oldP == heterogeneousRateGeneratorSequences)
    {
        heterogeneousRateGeneratorSequences = static_cast<const TypedDagNode< RbVector< RateGeneratorSequence > >* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == siteRates)
    {
        siteRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType>::swapParameter(oldP,newP);
    }

}

template<class charType>
void RevBayesCore::GeneralTreeHistoryCtmc<charType>::touchSpecialization( DagNode* affecter, bool touchAll )
{

    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == rootFrequencies || affecter == rootFrequencies )
    {

        const TopologyNode &root = this->tau->getValue().getRoot();
        this->flagNodeDirty( root );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType>::touchSpecialization( affecter, touchAll );
    }

}

#endif /* defined(__rb_mlandis__GeneralTreeHistoryCtmc__) */
