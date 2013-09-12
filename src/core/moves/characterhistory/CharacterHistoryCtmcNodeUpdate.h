//
//  CharacterHistoryCtmcNodeUpdate.h
//  rb_mlandis
//
//  Created by Michael Landis on 9/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterHistoryCtmcNodeUpdate__
#define __rb_mlandis__CharacterHistoryCtmcNodeUpdate__


#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "AbstractCharacterHistoryCtmc.h"
#include "BranchHistory.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class CharacterHistoryCtmcNodeUpdate : public SimpleMove {
        
    public:
        CharacterHistoryCtmcNodeUpdate( StochasticNode<BranchHistory> *n, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        CharacterHistoryCtmcNodeUpdate* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        // parameters
        double                          lambda;
        
        StochasticNode<BranchHistory>*  variable;
        double                          storedValue;
        
        std::set<size_t>                storedIndexes;
        std::multiset<CharacterEvent*>  storedCharacterEvents;
        
    };
    
}

#endif /* defined(__rb_mlandis__CharacterHistoryCtmcNodeUpdate__) */
