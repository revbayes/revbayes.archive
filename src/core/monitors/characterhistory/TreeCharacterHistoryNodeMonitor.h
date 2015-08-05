//
//  TreeCharacterHistoryNodeMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/5/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TreeCharacterHistoryNodeMonitor__
#define __rb_mlandis__TreeCharacterHistoryNodeMonitor__


#include "AbstractCharacterData.h"
#include "AbstractTreeHistoryCtmc.h"
#include "BiogeographicCladoEvent.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "BranchHistory.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class TreeCharacterHistoryNodeMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractDiscreteCharacterData>* s, TypedDagNode<treeType> *t, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool sm=true, bool sne=false, bool ste=true);
        
        // new TreeCharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 10, filepath + "rb.tree_chars.txt", "\t"));
        
        TreeCharacterHistoryNodeMonitor(const TreeCharacterHistoryNodeMonitor& f);
        
        // basic methods
        TreeCharacterHistoryNodeMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::string                         buildExtendedNewick(void);
        std::string                         buildExtendedNewick(TopologyNode* n);
        std::string                         buildNumEventsStr(TopologyNode* n);
        std::string                         buildCladoStr(TopologyNode* n);
        std::string                         buildNumEventsStr(TopologyNode* n, unsigned state);
        std::string                         buildCladoForTreeStr(unsigned state);
        std::string                         buildNumEventsForTreeStr(unsigned state);
        std::string                         buildCharacterHistoryString(TopologyNode* n, std::string brEnd="child");
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        StochasticNode<AbstractDiscreteCharacterData>* variable;
        TypedDagNode<treeType>*             tree;
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                showMetadata;
        bool                                showNumEvents;
        bool                                showTreeEvents;
        size_t                              numStates;
        
    };
    
}

/* Constructor */
template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractDiscreteCharacterData>* s, TypedDagNode<treeType>* t, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool sm, bool sne, bool ste) :
Monitor(g,t),
outStream(),
variable(s),
tree( t ),
filename( fname ),
separator( del ),
posterior( pp ),
prior( pr ),
likelihood( l ),
append(ap),
showMetadata(sm),
showNumEvents(sne),
showTreeEvents(ste),
numStates(0)
{
    nodes.push_back(s);
//    nodes.push_back(t);
    s->incrementReferenceCount();
    
    numStates = static_cast<const DiscreteCharacterState&>(s->getValue().getCharacter(0,0)).getNumberOfStates();
}

template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::TreeCharacterHistoryNodeMonitor(const TreeCharacterHistoryNodeMonitor &m) :
Monitor( m ),
outStream( ),
variable(m.variable),
tree( m.tree ),
nodeVariables( m.nodeVariables ),
showMetadata(m.showMetadata),
showNumEvents(m.showNumEvents),
showTreeEvents(m.showTreeEvents),
numStates(m.numStates)
{    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
}


/* Clone the object */
template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType,treeType>* RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::clone(void) const {
    
    return new TreeCharacterHistoryNodeMonitor<charType, treeType>(*this);
}

template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::closeStream() {
    outStream.close();
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildExtendedNewick( void ) {
    //tree->getValue().getRoot().setNewickNeedsRefreshing(true);
    std::string newick = buildExtendedNewick( &tree->getValue().getRoot() );
    return newick;
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildCharacterHistoryString(TopologyNode* n, std::string infoStr)
{

    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    const BranchHistory& bh = p->getHistory(*n);
    std::stringstream ss;
    
    if (infoStr=="child")
    {
        std::vector<CharacterEvent*> characters = bh.getChildCharacters();
        
        for (size_t i = 0; i < characters.size(); i++)
        {
//            if (i != 0) ss << ",";
            ss << characters[i]->getState();
        }
    }
    else if (infoStr=="parent")
    {
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        for (size_t i = 0; i < characters.size(); i++)
        {
//            if (i != 0) ss << ",";
            ss << characters[i]->getState();
        }
    }
    else if (infoStr=="clado_state")
    {
        BiogeographicTreeHistoryCtmc<charType, treeType>* q = static_cast<BiogeographicTreeHistoryCtmc<charType, treeType>* >(p);
        int cladoState = q->getCladogenicState(*n);
        
        if (cladoState == BiogeographicCladoEvent::SYMPATRY_NARROW)
            ss << "n";
        else if (cladoState == BiogeographicCladoEvent::SYMPATRY_WIDESPREAD)
            ss << "w";
        else if (cladoState == BiogeographicCladoEvent::SYMPATRY_SUBSET)
            ss << "s";
        else if (cladoState == BiogeographicCladoEvent::ALLOPATRY)
            ss << "a";
        else
            ss << "NA";
        
    }
    else if (infoStr=="bud_state")
    {
        BiogeographicTreeHistoryCtmc<charType, treeType>* q = static_cast<BiogeographicTreeHistoryCtmc<charType, treeType>* >(p);
        int budState = (q)->getBuddingState(*n);
        
        ss << ( budState == 1 ? n->getIndex() : n->getParent().getChild(1).getIndex() );
    }
    else if (infoStr=="state_into")
    {
        // loop over events
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh.getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        
        std::vector<unsigned> v(numStates,0);
        for (it = evts.begin(); it != evts.end(); it++)
        {
            unsigned s = (*it)->getState();
            v[s] += 1;
        }

        // loop over states
        for (size_t i = 0; i < v.size(); i++)
        {
            if (i != 0)
                ss << ",";
//            ss << i << ":" << v[i];
            ss << v[i];
        }
    }
    else if (infoStr=="state_betw")
    {
        // loop over events
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh.getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        
        std::vector<unsigned> v(numStates*numStates,0);
        for (it = evts.begin(); it != evts.end(); it++)
        {
            size_t idx = (*it)->getIndex();
            unsigned from = characters[idx]->getState();
            unsigned to = (*it)->getState();
            v[ numStates*from + to ] += 1;
            characters[idx] = (*it);
        }
        
        // loop over states
        for (size_t i = 0; i < numStates; i++)
        {
            if (i != 0)
                ss << ",";
//            ss << i << ":{";
            ss << "{";
            for (size_t j = 0; j < numStates; j++)
            {
                if (j != 0)
                    ss << ",";
//                ss << j << ":" << v[numStates*i + j];
                ss << v[numStates*i + j];
            }
            ss << "}";
        }
    
    }
    else if (infoStr=="events")
    {
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh.getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        
        std::vector<unsigned> v(numStates*numStates,0);
        double ndAge;
        if (n->isRoot())
            ndAge = n->getAge() * 5;
        else
            ndAge = n->getParent().getAge();
        double brLen = n->getBranchLength();
        
        for (it = evts.begin(); it != evts.end(); it++)
        {
            if (it != evts.begin())
                ss << ",";
            
            ss << "{";
            ss << "t:" << (*it)->getTime() << ",";
            ss << "a:" << ndAge - brLen * (*it)->getTime() << ",";
            ss << "s:" << (*it)->getState() << ",";
            ss << "i:" << (*it)->getIndex() << "";
            ss << "}";

        }
    }
    
    return ss.str();

}

/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildExtendedNewick( TopologyNode* n ) {
    // create the newick string
    std::stringstream o;
    
    // extended data is only found on admixture nodes
    std::string additionalInfo = "";
    
    // loop over admixture nodes per branch
    std::stringstream characterStream;
    
    double br = 1.0;
    
    if (showMetadata)
    {
        characterStream << "[&";
        characterStream << "index=" << n->getIndex();
        
        // character history
        characterStream << ";nd=" << buildCharacterHistoryString(n,"child") << "";
        characterStream << ";pa=" << buildCharacterHistoryString(n,"parent") << "";
        if (!n->isTip())
        {
            characterStream << ";ch0=" << buildCharacterHistoryString(&n->getChild(0),"parent") << "";
            characterStream << ";ch1=" << buildCharacterHistoryString(&n->getChild(1),"parent") << "";
            
            characterStream << ";cs=" << buildCharacterHistoryString(&n->getChild(0),"clado_state");
            characterStream << ";bn=" << buildCharacterHistoryString(&n->getChild(0),"bud_state");
        }
        
        // # events
//        characterStream << ",si={" << buildCharacterHistoryString(n,"state_into") << "}";
//        characterStream << ",sb={" << buildCharacterHistoryString(n,"state_betw") << "}";
        
        // event history
        characterStream << ";ev={" << buildCharacterHistoryString(n,"events") << "}";
        
        // ... whatever else
        characterStream << "]";
        
        additionalInfo = characterStream.str();
    }
    
    // test whether this is a internal or external node
    if (n->isTip()) {
        // this is a tip so we just return the name of the node
        o << n->getName() << additionalInfo << ":" << n->getBranchLength();
    }
    
    else {
        o << "(";
        for (size_t i=0; i<(n->getNumberOfChildren()-1); i++) {
            o << buildExtendedNewick( &n->getChild(i) ) << ",";
        }
        o << buildExtendedNewick( &n->getChild(n->getNumberOfChildren()-1) ) << ")" << additionalInfo << ":" << n->getBranchLength() * br;
    }
    
    return o.str();
}

/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsStr(TopologyNode* nd)
{
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BranchHistory* bh = &p->getHistory(*nd);
    
    std::stringstream ss;
    double eventRatio = (double)bh->getNumEvents() / 1.0; //nd->getBranchLength();
    
    ss << eventRatio;
    return ss.str();
    
}



/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsStr(TopologyNode* nd, unsigned state)
{
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BranchHistory* bh = &p->getHistory(*nd);
    
    const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
    
    std::stringstream ss;
    
    unsigned v = 0;
    for (it = evts.begin(); it != evts.end(); it++)
    {
        if ( (*it)->getState() == state )
            v++;
    }
    double eventRatio = (double)v / 1.0;//nd->getBranchLength();

    ss << eventRatio;
    return ss.str();
    
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsForTreeStr(unsigned state)
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    
    const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
    unsigned v = 0;
    std::stringstream ss;
    
    for (size_t i = 0; i < nds.size(); i++)
    {
        BranchHistory* bh = &p->getHistory(*nds[i]);
        
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh->getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        
        
        for (it = evts.begin(); it != evts.end(); it++)
        {
            if ( (*it)->getState() == state )
                v++;
        }
        
    }
    double eventRatio = (double)v / 1.0;//nd->getBranchLength();
    ss << eventRatio;
    return ss.str();
}


template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildCladoForTreeStr(unsigned state)
{
    std::stringstream ss;
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BiogeographicTreeHistoryCtmc<charType, treeType>* q = static_cast<BiogeographicTreeHistoryCtmc<charType, treeType>* >(p);
    
    const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
    
    int cladoCount = 0;
    for (size_t i = 0; i < nds.size(); i++)
    {
        if (!nds[i]->isTip() && state == q->getCladogenicState(nds[i]->getChild(0)))
            cladoCount++;
    }
    ss << cladoCount;
    return ss.str();
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildCladoStr(TopologyNode* nd)
{
    std::stringstream ss;
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BiogeographicTreeHistoryCtmc<charType, treeType>* q = static_cast<BiogeographicTreeHistoryCtmc<charType, treeType>* >(p);
    int cladoState = q->getCladogenicState(*nd);
    ss << cladoState;
    
//    std::string cladoStr = "";
//    if (cladoState == 0)
//        cladoStr = "\"s\"";
//    else if (cladoState == 1)
//        cladoStr = "\"p\"";
//    else if (cladoState == 2)
//        cladoStr = "\"a\"";
//      ss << cladoStr;

    return ss.str();
}

/** Monitor value at generation gen */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::monitor(unsigned long gen) {
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        if ( posterior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                pp += (*it)->getLnProbability();
            }
            outStream << pp;
        }
        
        if ( likelihood ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( (*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if ( prior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( !(*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if (showNumEvents)
        {
            for (unsigned s = 0; s < numStates; s++)
            {
                outStream << separator << buildNumEventsForTreeStr(s);
            }
            for (unsigned s = 0; s < 4; s++)
            {
                outStream << separator << buildCladoForTreeStr(s);
            }
            for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
            {
                TopologyNode* nd = &tree->getValue().getNode(i);
                for (unsigned s = 0; s < numStates; s++)
                {
                    if (!nd->isRoot())
                        outStream << separator << buildNumEventsStr(nd, s);
                }
                if (!nd->isRoot())
                    outStream << separator << buildCladoStr(nd);
            }
        }
        
        if (showTreeEvents)
        {
            // add a separator before the tree
            outStream << separator;
            outStream << buildExtendedNewick();
            
        }
        
        outStream << std::endl;
        
    }
}


/** open the file stream for printing */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::openStream(void) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
}

/** Print header for monitored values */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::printHeader() {
    
    // print one column for the iteration number
    outStream << "Iter";
    
    if ( posterior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Posterior";
    }
    
    if ( likelihood ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    if ( prior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Prior";
    }
    
    if (showNumEvents)
    {
        for (size_t s = 0; s < numStates; s++)
        {
            outStream << separator << "t_s" << s;
        }
        for (size_t s = 0; s < 4; s++)
        {
            outStream << separator << "t_c" << s;
        }
        for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
        {
            TopologyNode* nd = &tree->getValue().getNode(i);
            for (size_t s = 0; s < numStates; s++)
            {
                if (!nd->isRoot())
                    outStream << separator << "b" << nd->getIndex() << "_s" << s;
            }
            if (!nd->isRoot())
                outStream << separator << "b" << nd->getIndex() << "_c";

        }
    }
    
    if (showTreeEvents)
    {
        // add a separator tree
        outStream << separator << "Tree";
    }
    
    // end line of stream
    outStream << std::endl;
}

template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN) {

    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode< treeType > *>( newN );
    }
    else if (oldN == variable)
    {
        variable = static_cast<StochasticNode<AbstractDiscreteCharacterData>* >(newN);
    }

    // delegate to base class
    Monitor::swapNode(oldN, newN);
}


#endif /* defined(__rb_mlandis__TreeCharacterHistoryNodeMonitor__) */
