//
//  PathRejectionSampleMove.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/8/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__PathRejectionSampleMove__
#define __rb_mlandis__PathRejectionSampleMove__


#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "AbstractTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "DeterministicNode.h"
//#include "PathRejectionSampleProposal.h"
#include "Proposal.h"
#include "RateMap.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class PathRejectionSampleMove : public SimpleMove {
        
    public:
        PathRejectionSampleMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, Proposal* p, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        PathRejectionSampleMove* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                            acceptSimpleMove(void);
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        
        // variables
        StochasticNode<AbstractCharacterData>*  variable;
        StochasticNode<treeType>*               tau;
        DeterministicNode<RateMap>*             qmap;
        
        // parameters
        double                          lambda;
        size_t                          numNodes;
        size_t                          numCharacters;
        size_t                          numStates;
        
        // proposal
        Proposal*                       proposal;
    };
    
}

#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

template<class charType, class treeType>
RevBayesCore::PathRejectionSampleMove<charType, treeType>::PathRejectionSampleMove( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, Proposal* p, double l, bool tuning, double w) :
    SimpleMove(n, w, tuning),
    variable(n),
    tau(t),
    qmap(q),
    lambda(l),
    proposal(p)
{
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates();
    
    nodes.insert(tau);
    nodes.insert(qmap);
}

// Basic utility functions
template<class charType, class treeType>
RevBayesCore::PathRejectionSampleMove<charType, treeType>* RevBayesCore::PathRejectionSampleMove<charType, treeType>::clone(void) const
{
    return new PathRejectionSampleMove( *this );
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    SimpleMove::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        variable = static_cast<StochasticNode<AbstractCharacterData>* >( newN );
    }
    else if (oldN == tau)
    {
        tau = static_cast<StochasticNode<TimeTree>* >( newN );
    }
    else if (oldN == qmap)
    {
        qmap = static_cast<DeterministicNode<RateMap>* >(newN);
    }
    
    proposal->swapNode(oldN, newN);
}

template<class charType, class treeType>
const std::string& RevBayesCore::PathRejectionSampleMove<charType, treeType>::getMoveName(void) const
{
    static std::string name = "PathRejectionSampleMove";
    return name;
}

template<class charType, class treeType>
double RevBayesCore::PathRejectionSampleMove<charType, treeType>::PathRejectionSampleMove::performSimpleMove(void)
{
    proposal->prepareProposal();
    double lnProposalRatio = proposal->doProposal();
    
    
    return lnProposalRatio;
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::rejectSimpleMove(void)
{
    proposal->undoProposal();
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::acceptSimpleMove(void)
{
    proposal->cleanProposal();
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::tune(void)
{
    ; // do nothing
}


#endif /* defined(__rb_mlandis__PathRejectionSampleMove__) */
