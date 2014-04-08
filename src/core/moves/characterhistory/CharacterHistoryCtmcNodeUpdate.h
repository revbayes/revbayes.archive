//
//  CharacterHistoryCtmcNodeUpdate.h
//  rb_mlandis
//
//  Created by Michael Landis on 9/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterHistoryCtmcNodeUpdate__
#define __rb_mlandis__CharacterHistoryCtmcNodeUpdate__


//#include <ostream>
#include <set>
#include <string>
#include <vector>
#include <map>

#include "AbstractCharacterHistoryCtmc.h"
#include "BranchHistory.h"
#include "Move.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {


class CharacterHistoryCtmcNodeUpdate : public Move {
        
    public:
        CharacterHistoryCtmcNodeUpdate( StochasticNode<BranchHistory>* n, std::vector<StochasticNode<BranchHistory>* > bhv, StochasticNode<TimeTree>* t, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        CharacterHistoryCtmcNodeUpdate* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        
        void                                    acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                                  performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                                    rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
        
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        void                            tune(void);
        
        bool                                    changed;
        DagNode*                                theNode;

        
    private:
        // parameters
               
        StochasticNode<BranchHistory>*  variable;
        StochasticNode<TimeTree>*       tree;
        std::vector< StochasticNode<BranchHistory>* > bh_vector;
        
        std::set<size_t>                storedIndexes;
        std::map<size_t,BranchHistory*>  storedValue;
        std::set<BranchHistory>         storedSet;
    
        double                          lambda;
        size_t                          numCharacters;
        size_t                          numStates;
        size_t                          index;

    
    };

}

#endif /* defined(__rb_mlandis__CharacterHistoryCtmcNodeUpdate__) */
