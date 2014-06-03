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
        TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractCharacterData>* s, TypedDagNode<treeType> *t, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool sm=true, bool sr=true);
        
        // new TreeCharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 10, filepath + "rb.tree_chars.txt", "\t"));
        
        TreeCharacterHistoryNodeMonitor(const TreeCharacterHistoryNodeMonitor& f);
        
        // basic methods
        TreeCharacterHistoryNodeMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::string                         buildExtendedNewick();
        std::string                         buildExtendedNewick(TopologyNode* n);
        std::string                         buildCharacterHistoryString(TopologyNode* n, std::string brEnd="child");
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        StochasticNode<AbstractCharacterData>* variable;
        TypedDagNode<treeType>*             tree;
        //std::vector<StochasticNode<BranchHistory>* > branchHistories;
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                showMetadata;
        bool                                showRates;
        size_t                              numStates;
        
    };
    
}

/* Constructor */
template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractCharacterData>* s, TypedDagNode<treeType>* t, int g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool sm, bool sr) :
Monitor(g,s),
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
showRates(sr),
numStates(0)
{
    nodes.push_back(s);
    nodes.push_back(t);
    
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
showRates(m.showRates),
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
            if (i != 0)
                ss << ",";
            ss << characters[i]->getState();
        }
    }
    else if (infoStr=="parent")
    {
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (i != 0)
                ss << ",";
            ss << characters[i]->getState();
        }
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
            ss << i << ":" << v[i];
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
            ss << i << ":{";
            for (size_t j = 0; j < numStates; j++)
            {
                if (j != 0)
                    ss << ",";
                ss << j << ":" << v[numStates*i + j];
            }
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
        characterStream << "[";
        
        // character history
        characterStream << "&ch={" << buildCharacterHistoryString(n,"child") << "}";
        
        // # events
        characterStream << ",&state_into={" << buildCharacterHistoryString(n,"state_into") << "}";
        characterStream << ",&state_betw={" << buildCharacterHistoryString(n,"state_betw") << "}";
        
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


/** Monitor value at generation gen */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::monitor(long gen) {
    
    // get the printing frequency
    int samplingFrequency = printgen;
    
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
        
        // add a separator before the tree
        outStream << separator;
        
        std::string en = buildExtendedNewick();
        //std::cout << "\n" << en << "\n\n";
        
        outStream << en;
        
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
    outStream << "Iteration";
    
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
    
    // add a separator tree
    outStream << separator << "Tree";
    
    // end line of stream
    outStream << std::endl;
}

template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN) {
    
    bool found = false;
    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode< treeType > *>( newN );
        found = true;
    }
    else if (oldN == variable)
    {
        variable = static_cast<StochasticNode<AbstractCharacterData>* >(newN);
    }

    /*
     if (found == false)
     {
     // error catching
     if ( nodeVariables.find(oldN) == nodeVariables.end() ) {
     throw RbException("Cannot replace DAG node in this monitor because the monitor doesn't hold this DAG node.");
     }
     
     nodeVariables.erase( oldN );
     nodeVariables.insert( newN );
     }
     */
    
    // delegate to base class
    Monitor::swapNode(oldN, newN);
}


#endif /* defined(__rb_mlandis__TreeCharacterHistoryNodeMonitor__) */
