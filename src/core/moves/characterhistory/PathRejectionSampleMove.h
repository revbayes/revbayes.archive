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
#include "Proposal.h"
#include "RateMap.h"
#include "MoveOld.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class PathRejectionSampleMove : public MoveOld {
        
    public:
        PathRejectionSampleMove( StochasticNode<AbstractDiscreteCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, Proposal* p, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        PathRejectionSampleMove* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
        void                            acceptSimpleMove(void);
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        
        // ctmcs
        StochasticNode<AbstractDiscreteCharacterData>*  ctmc;
        StochasticNode<treeType>*               tau;
        DeterministicNode<RateMap>*             qmap;
        
        // parameters
        double                          lambda;
        size_t                          numNodes;
        size_t                          numCharacters;
        size_t                          numStates;
        
        // proposal
        Proposal*                       proposal;
        
        bool changed;
    };
    
}

#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

template<class charType, class treeType>
RevBayesCore::PathRejectionSampleMove<charType, treeType>::PathRejectionSampleMove( StochasticNode<AbstractDiscreteCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, Proposal* p, double l, bool tuning, double w) :
    MoveOld(t, w, tuning),
    ctmc(n),
    tau(t),
    qmap(q),
    lambda(l),
    proposal(p)
{
    numNodes = t->getValue().getNumberOfNodes();
    numCharacters = n->getValue().getNumberOfCharacters();
    numStates = static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates();
    
    nodes.insert(ctmc);
    nodes.insert(qmap);
    nodes.insert(tau);

    changed = false;

    ;
}

// Basic utility functions
template<class charType, class treeType>
RevBayesCore::PathRejectionSampleMove<charType, treeType>* RevBayesCore::PathRejectionSampleMove<charType, treeType>::clone(void) const
{
    PathRejectionSampleMove<charType, treeType>* p = new PathRejectionSampleMove( *this );
    Proposal *prop = p->proposal;
    
    return p;
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == ctmc)
    {
        ctmc = static_cast<StochasticNode<AbstractDiscreteCharacterData>* >( newN );
    }
    else if (oldN == tau)
    {
        tau = static_cast<StochasticNode<treeType>* >( newN );
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
double RevBayesCore::PathRejectionSampleMove<charType, treeType>::performMove( double &probRatio ) {
    
    if (changed)
    {
        throw RbException("Trying to execute a simple move twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf )
    {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    ctmc->touch();
//    tau->touch();
    
    // calculate the probability ratio for the node we just changed
    // don't need this probability quite yet...
    probRatio = 0.0; // tau->getLnProbabilityRatio();
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
    {
        
        std::set<DagNode* > affectedNodes;
        tau->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i)
        {
            DagNode* theAffectedNode = *i;
//            std::cout << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() << " " << theAffectedNode->getLnProbability() << "\n";
            probRatio += theAffectedNode->getLnProbabilityRatio();
        }
    }
    
//    std::cout << "hr " << hr << "\n";
//    std::cout << "probRatio " << probRatio << "\n";
//    std::cout << "move hr + probRatio " << hr + probRatio << "\n";
//    if (fabs(hr + probRatio) > 0.1)
//    {
//        std::cout << "move hr + probRatio " << hr + probRatio << " " << hr << " " << probRatio << "\n";
//        std::cout << "!!!\n";
//    }
//    
//    std::cout << "\n";
    
    return hr;
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
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::rejectMove( void ) {
    
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    tau->touch();
    ctmc->touch();
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::rejectSimpleMove(void)
{
    proposal->undoProposal();
}

template<class charType, class treeType>
void RevBayesCore::PathRejectionSampleMove<charType, treeType>::acceptMove( void )
{
    changed = false;   
    acceptSimpleMove();
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
