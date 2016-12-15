#ifndef TreeHistoryCtmc_H
#define TreeHistoryCtmc_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "BranchHistory.h"
#include "DiscreteTaxonData.h"
#include "HomologousDiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "DnaState.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <cmath>

namespace RevBayesCore {

    template<class charType>
    class TreeHistoryCtmc : public TypedDistribution< AbstractHomologousDiscreteCharacterData >, public TreeChangeEventListener, public MemberObject< RbVector<double> >, public MemberObject< RbVector<int> > {

    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        TreeHistoryCtmc(const TypedDagNode<Tree> *t, size_t nChars, size_t nSites, bool useAmbigChar=false);
        TreeHistoryCtmc(const TreeHistoryCtmc &n);                                                                              //!< Copy constructor
        virtual                                                            ~TreeHistoryCtmc(void);                                      //!< Virtual destructor

        // public member functions
        // pure virtual
        virtual TreeHistoryCtmc*                                            clone(void) const = 0;                                              //!< Create an independent clone
        virtual void                                                        redrawValue(void) = 0;
        virtual void                                                        drawInitValue(void) = 0;
        virtual void                                                        initializeTipValues(void) = 0;
        void                                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<int> &rv) const;     //!< Map the member methods to internal function calls
        void                                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;     //!< Map the member methods to internal function calls
        virtual double                                                      getBranchRate(size_t idx) const = 0;
        virtual std::vector<double>                                         getRootFrequencies(void) const = 0;
        virtual bool                                                        samplePathStart(const TopologyNode& node) = 0;
        virtual bool                                                        samplePathEnd(const TopologyNode& node) = 0;
        virtual bool                                                        samplePathHistory(const TopologyNode& node) = 0;

        // non-virtual
        double                                                              computeLnProbability(void);
        void                                                                fireTreeChangeEvent(const TopologyNode &n, const unsigned& m=0);                         //!< The tree has changed and we want to know which part.
        BranchHistory&                                                      getHistory(const TopologyNode& nd);
        const BranchHistory&                                                getHistory(const TopologyNode& nd) const;
        std::vector<BranchHistory*>                                         getHistories(void);
        const std::vector<BranchHistory*>&                                  getHistories(void) const;
        size_t                                                              getNumberOfSites(void) const;
        const Tree&                                                         getTree(void) const;
        void                                                                setHistory(const BranchHistory& bh, const TopologyNode& nd);
        void                                                                setHistories(const std::vector<BranchHistory*>& bh);
        void                                                                setValue(AbstractHomologousDiscreteCharacterData *v, bool f=false);           //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                setTipProbs(const HomologousCharacterData* tp);

        virtual const std::vector<double>&                                  getTipProbs(const TopologyNode& nd);
        virtual const std::vector<std::vector<double> >&                    getTipProbs(void);

        virtual void                                                        simulate(void);


    protected:

        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

        // helper method for this and derived classes
        void                                                                flagNodeDirty(const TopologyNode& n);
        double                                                              computeBranchRate(size_t index);

        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);

        // pure virtual methods
        virtual double                                                      computeRootLikelihood(const TopologyNode &nd) = 0;
        virtual double                                                      computeInternalNodeLikelihood(const TopologyNode &nd) = 0;
        virtual double                                                      computeTipLikelihood(const TopologyNode &nd) = 0;

        // members
        double                                                              lnProb;
        const size_t                                                        num_states;
        size_t                                                              num_sites;
        size_t                                                              numSiteRates;
        const TypedDagNode<Tree>*                                           tau;

        // the likelihoods
        std::vector<size_t>                                                 activeLikelihood;
        std::vector<std::vector<double> >                                   historyLikelihoods;

        // the data
        std::vector<std::vector<unsigned long> >                            charMatrix;
        std::vector<std::vector<bool> >                                     gapMatrix;
        std::vector<BranchHistory*>                                         histories;
        std::vector<std::vector<double> >                                   tipProbs;

        // convenience variables available for derived classes too
        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;

        // flags
        bool                                                                usingAmbiguousCharacters;
        bool                                                                treatUnknownAsGap;
        bool                                                                treatAmbiguousAsGaps;
        bool                                                                tipsInitialized;

    private:
        // private methods
        void                                                                fillLikelihoodVector(const TopologyNode &n);
        void                                                                initializeHistoriesVector(void);
        virtual void                                                        simulate(const TopologyNode& node, BranchHistory* bh, std::vector<DiscreteTaxonData< charType > >& taxa) = 0;
    };
}


template<class charType>
RevBayesCore::TreeHistoryCtmc<charType>::TreeHistoryCtmc(const TypedDagNode<Tree> *t, size_t nChars, size_t nSites, bool useAmbigChar) : TypedDistribution< AbstractHomologousDiscreteCharacterData >(  new HomologousDiscreteCharacterData<charType>() ),
    num_states( nChars ),
    num_sites( nSites ),
    numSiteRates( 1 ),
    tau( t ),
    activeLikelihood( std::vector<size_t>(tau->getValue().getNumberOfNodes(), 0) ),
    historyLikelihoods(),
    charMatrix(),
    gapMatrix(),
    histories(),
    changedNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(),false) ),
    dirtyNodes( std::vector<bool>(tau->getValue().getNumberOfNodes(), true) ),
    usingAmbiguousCharacters( useAmbigChar ),
    treatUnknownAsGap( true ),
    treatAmbiguousAsGaps( true ),
    tipsInitialized( false )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( tau );

    // We don't want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );

    // initialize histories
    initializeHistoriesVector();

}


template<class charType>
RevBayesCore::TreeHistoryCtmc<charType>::TreeHistoryCtmc(const TreeHistoryCtmc &n) : TypedDistribution< AbstractHomologousDiscreteCharacterData >( n ),
    num_states( n.num_states ),
    num_sites( n.num_sites ),
    numSiteRates( n.numSiteRates ),
    tau( n.tau ),
    activeLikelihood( n.activeLikelihood ),
    historyLikelihoods( n.historyLikelihoods ),
    charMatrix( n.charMatrix ),
    gapMatrix( n.gapMatrix ),
    histories( n.histories ),
    tipProbs( n.tipProbs ),
    changedNodes( n.changedNodes ),
    dirtyNodes( n.dirtyNodes ),
    usingAmbiguousCharacters( n.usingAmbiguousCharacters ),
    treatUnknownAsGap( n.treatUnknownAsGap ),
    treatAmbiguousAsGaps( n.treatAmbiguousAsGaps ),
    tipsInitialized( n.tipsInitialized )
{
    // We don'e want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );

}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class charType>
RevBayesCore::TreeHistoryCtmc<charType>::~TreeHistoryCtmc( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!

    // remove myself from the tree listeners
    if ( tau != NULL )
    {

        tau->getValue().getTreeChangeEventHandler().removeListener( this );

    }

}



template<class charType>
double RevBayesCore::TreeHistoryCtmc<charType>::computeBranchRate(size_t index)
{


    return 1.0;
}

template<class charType>
double RevBayesCore::TreeHistoryCtmc<charType>::computeLnProbability( void )
{

    // we need to check here if we still are listining to this tree for change events
    // the tree could have been replaced without telling us
    if ( tau->getValue().getTreeChangeEventHandler().isListening( this ) == false )
    {
        tau->getValue().getTreeChangeEventHandler().addListener( this );
        dirtyNodes = std::vector<bool>(tau->getValue().getNumberOfNodes(), true);
    }

    this->lnProb = 0.0;

    const std::vector<TopologyNode*>& nodes = tau->getValue().getNodes();

    for (size_t i = 0; i < nodes.size(); i++)
    {
        const TopologyNode& nd = *nodes[i];
        size_t nodeIndex = nd.getIndex();
        fillLikelihoodVector(nd);
        double nodeLnProb = historyLikelihoods[ activeLikelihood[nodeIndex] ][nodeIndex];
        this->lnProb += nodeLnProb;
    }

    return this->lnProb;
}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<int> &rv) const
{

    if ( n == "numCharacterChanges" )
    {
        rv.clear();
        rv.resize( num_sites );

        int index = static_cast<const TypedDagNode<int>* >( args[0] )->getValue() - 1;

        //        const BranchHistory& bh = branch_histories[ index ];
        const std::multiset<CharacterEvent*,CharacterEventCompare> &states = this->histories[index]->getHistory();

        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        for (it = states.begin(); it != states.end(); ++it)
        {
            size_t s = (*it)->getSiteIndex();
            rv[s] += 1;
        }

    }
    else
    {
        throw RbException("The character history process does not have a member method called '" + n + "'.");
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<double> &rv) const
{

    if ( n == "stateFrequencies" )
    {
        rv.clear();
        rv.resize( num_states );

        int index = static_cast<const TypedDagNode<int>* >( args[0] )->getValue() - 1;

//        const BranchHistory& bh = branch_histories[ index ];
        const std::vector<CharacterEvent*> &states = this->histories[index]->getChildCharacters();

        if ( states.size() != num_sites )
        {
            throw RbException("Wrong number of internal states in TreeHistoryCtmc.");
        }

        double delta = 1.0/num_sites;
        for (size_t i = 0; i < num_sites; ++i)
        {
            size_t s = states[i]->getState();
            rv[s] += delta;
        }

    }
    else if ( n == "relativeTimeInStates" )
    {
        rv.clear();
        rv.resize( num_states );

        int node_index = static_cast<const TypedDagNode<int>* >( args[0] )->getValue() - 1;
        int site_index = static_cast<const TypedDagNode<int>* >( args[1] )->getValue() - 1;

        //        const BranchHistory& bh = branch_histories[ index ];
        const std::vector<CharacterEvent*> &states = this->histories[node_index]->getParentCharacters();

        double branch_length = tau->getValue().getNode(node_index).getBranchLength();

        size_t current_state = states[site_index]->getState();
        double previous_time = 0.0;
        const std::multiset<CharacterEvent*,CharacterEventCompare> &events = this->histories[node_index]->getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        for (it = events.begin(); it != events.end(); ++it)
        {
            CharacterEvent *event = (*it);
            size_t s = event->getSiteIndex();
            if ( s == site_index )
            {
                double t = event->getTime();
                rv[current_state] += ((t-previous_time)/branch_length);
                current_state = event->getState();
                previous_time = t;
            }
        }
        rv[current_state] += ((branch_length-previous_time)/branch_length);

    }
    else
    {
        throw RbException("The character history process does not have a member method called '" + n + "'.");
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::fillLikelihoodVector(const TopologyNode &node)
{
    size_t nodeIndex = node.getIndex();
    if ( dirtyNodes[nodeIndex] == false && false )
    {
        return;
    }

    // compute
    double lnL = computeInternalNodeLikelihood(node);

    if ( node.isTip() == true )
    {
        lnL += computeTipLikelihood(node);
    }

    if ( node.isRoot() == true )
    {
        lnL += computeRootLikelihood(node);
    }

    historyLikelihoods[ activeLikelihood[nodeIndex] ][nodeIndex] = lnL;

    // mark as computed
    dirtyNodes[nodeIndex] = false;

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n, const unsigned& m )
{

    // call a recursive flagging of all node above (closer to the root) and including this node
    flagNodeDirty(n);

}



template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::flagNodeDirty( const RevBayesCore::TopologyNode &n )
{

    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();

    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( dirtyNodes[index] == false )
    {
        // set the flag
        dirtyNodes[index] = true;

        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( changedNodes[index] == false )
        {
            //            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            //activeLikelihood[index] = 0;
            changedNodes[index] = true;
        }

    }

}

template<class charType>
const RevBayesCore::BranchHistory&  RevBayesCore::TreeHistoryCtmc<charType>::getHistory(const TopologyNode& nd) const
{
    return histories[nd.getIndex()];
}

template<class charType>
RevBayesCore::BranchHistory&  RevBayesCore::TreeHistoryCtmc<charType>::getHistory(const TopologyNode& nd)
{
    return *histories[nd.getIndex()];
}


template<class charType>
const std::vector<RevBayesCore::BranchHistory*>& RevBayesCore::TreeHistoryCtmc<charType>::getHistories(void) const
{
    return histories;
}


template<class charType>
std::vector<RevBayesCore::BranchHistory*> RevBayesCore::TreeHistoryCtmc<charType>::getHistories(void)
{
    return histories;
}


template<class charType>
size_t RevBayesCore::TreeHistoryCtmc<charType>::getNumberOfSites( void ) const
{

    return num_sites;
}


template<class charType>
const std::vector<std::vector<double> >& RevBayesCore::TreeHistoryCtmc<charType>::getTipProbs(void)
{
    return tipProbs;
}


template<class charType>
const std::vector<double>& RevBayesCore::TreeHistoryCtmc<charType>::getTipProbs(const TopologyNode& nd)
{
    return tipProbs[nd.getIndex()];
}


template<class charType>
const RevBayesCore::Tree& RevBayesCore::TreeHistoryCtmc<charType>::getTree( void ) const
{

    return tau->getValue();
}



template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::initializeHistoriesVector( void )
{
    for (size_t i = 0; i < histories.size(); ++i)
    {
        BranchHistory *tmp = histories[i];
        delete tmp;
    }
    histories.clear();

    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    histories.resize(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        TopologyNode* nd = nodes[i];
        histories[nd->getIndex()] = new BranchHistory(num_sites,num_states,nd->getIndex());
    }

    historyLikelihoods.resize(2);
    for (size_t i = 0; i < 2; ++i)
    {
        historyLikelihoods[i].resize(nodes.size(), 0.0);
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::keepSpecialization( DagNode* affecter )
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


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::restoreSpecialization( DagNode* affecter )
{

    // reset the flags
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
    {
        (*it) = false;
    }

    //std::cout << "affecter " << affecter->getName() << "\n";

    // restore the active likelihoods vector
    for (size_t index = 0; index < changedNodes.size(); ++index)
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index] )
        {

//            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }

        // set all flags to false
        changedNodes[index] = false;
    }


    return;
}

template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::setHistory(const BranchHistory& bh, const TopologyNode& nd)
{
    // free memory
    delete histories[ nd.getIndex() ];

    // create new branch history object
    histories[ nd.getIndex() ] = new BranchHistory(bh);

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::setHistories(const std::vector<BranchHistory*>& bh)
{
    for (size_t i = 0; i < bh.size(); ++i)
    {
        delete histories[i];
        histories[i] = bh[i];
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::setTipProbs(const HomologousCharacterData* tp)
{

    throw RbException("Bogus implementation for setTipProb (Sebastian)");

    tipProbs.clear();

    size_t numTaxa = tp->getNumberOfTaxa();
    size_t numCharacters = tp->getNumberOfCharacters();

    const std::vector<TopologyNode*>& nodes = this->tau->getValue().getNodes();

    tipProbs.resize(numTaxa);
    const ContinuousCharacterData* ccdp = static_cast<const ContinuousCharacterData*>(tp);
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];
        if (!nd->isTip())
        {
            continue;
        }

        const ContinuousTaxonData* cd = &ccdp->getTaxonData(nd->getName());
        for (size_t j = 0; j < numCharacters; j++)
        {
            double v = cd->getCharacter(j);
            //tipProbs[nd->getIndex()].push_back(1-v);
            tipProbs[nd->getIndex()].push_back(v);
        }

    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::setValue(AbstractHomologousDiscreteCharacterData *v, bool force)
{

    // delegate to the parent class
    TypedDistribution< AbstractHomologousDiscreteCharacterData >::setValue(v, force);


    tipsInitialized = false;
    this->num_sites = v->getNumberOfCharacters();
    initializeHistoriesVector();

    drawInitValue();

    if ( this->dag_node != NULL )
    {
        this->dag_node->getLnProbability();
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::simulate(void)
{

    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<charType>();

    // create a vector of taxon data
    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData< charType > >( tau->getValue().getNumberOfNodes(), DiscreteTaxonData<charType>( Taxon("") ) );

    // recursively simulate, starting with the root heading tipwards
    const TopologyNode& nd = tau->getValue().getRoot();
    histories[ nd.getIndex() ] = new BranchHistory(num_sites, num_states, nd.getIndex());
    BranchHistory* bh = histories[ nd.getIndex() ];

    simulate(nd, bh, taxa);

    // add the taxon data to the character data
    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
    {
        this->value->addTaxonData( taxa[i] );
        //        this->value->getTaxonData( tau->getValue().getNodes()[i]->getName() );
    }

    TypedDistribution< AbstractHomologousDiscreteCharacterData >::setValue(this->value);
}


/**
 * Swap a parameter of the distribution. We receive this call just before being replaced by a variable,
 * in which case the variable deletes the old parameter. We also receive this call during the cloning of
 * a DAG. Also in that case it is safe to leave the memory management of the tau parameter to others,
 * namely to the destructor of the original distribution owning the old parameter.
 */
template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    // we only have the topology here as the parameter
    if (oldP == tau)
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
        tau->getValue().getTreeChangeEventHandler().addListener( this );
    }

}


template<class charType>
void RevBayesCore::TreeHistoryCtmc<charType>::touchSpecialization( DagNode* affecter, bool touchAll )
{

    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == this->dag_node )
    {
        // do nothing, assume tree events have been fired
        ;
    }
    else if ( affecter != tau )
    {

        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
        {
            (*it) = true;
        }

        // flip the active likelihood pointers
        for (size_t index = 0; index < changedNodes.size(); ++index)
        {
            if ( !changedNodes[index] )
            {
//                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                //activeLikelihood[index] = 0;
                changedNodes[index] = true;
            }

        }

    }

}

#endif
