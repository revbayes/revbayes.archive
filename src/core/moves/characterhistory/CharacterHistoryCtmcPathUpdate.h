//
//  CharacterHistoryCtmcPathUpdate.h
//  rb_mlandis
//
//  Created by Michael Landis on 10/1/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterHistoryCtmcPathUpdate__
#define __rb_mlandis__CharacterHistoryCtmcPathUpdate__


#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "AbstractCharacterHistoryCtmc.h"
#include "BranchHistory.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class CharacterHistoryCtmcPathUpdate : public SimpleMove {
        
    public:
        CharacterHistoryCtmcPathUpdate( StochasticNode<BranchHistory> *n, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        CharacterHistoryCtmcPathUpdate* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                            acceptSimpleMove(void);
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        
        StochasticNode<BranchHistory>*  variable;
        BranchHistory                   storedValue;
        
        std::set<size_t>                storedIndexes;
        
        // parameters
        double                          lambda;
        size_t                          numCharacters;
        size_t                          numStates;

        
    };
    
}

#endif /* defined(__rb_mlandis__CharacterHistoryCtmcPathUpdate__) */
