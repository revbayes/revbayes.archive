//
//  BranchHistory.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BranchHistory__
#define __rb_mlandis__BranchHistory__

#include "DiscreteCharacterState.h"
#include "CharacterEvent.h"
#include "CharacterEventCompare.h"
#include <vector>
#include <set>

namespace RevBayesCore {
    
    class CharacterEvent;

    class BranchHistory {
        
    public:
        
        BranchHistory(DiscreteCharacterState* d, size_t nc, size_t bi=-1, bool it=false, bool ir=false);
        
        const std::vector<CharacterEvent*>& getParentCharacters(void);
        const std::vector<CharacterEvent*>& getChildCharacters(void);
        const std::multiset<CharacterEvent*,CharacterEventCompare>& getHistory(void);
        
        void addEvent(CharacterEvent* evt);
        void removeEvent(CharacterEvent* evt);
        void clearEvents(void);
        void clearEvents(const std::set<size_t>& clearSet);
        void updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet);
        
        void print(void);
        
    protected:
        
        
    private:
        
        // container/element arguments
        size_t branchIndex;
        size_t numCharacters;
        size_t numStates;
        std::string labels;
        bool isTip;
        bool isRoot;
        
        // containers
        std::multiset<CharacterEvent*,CharacterEventCompare> history;
        std::vector<CharacterEvent*> parentCharacters;
        std::vector<CharacterEvent*> childCharacters;
       
    };
}

#endif /* defined(__rb_mlandis__BranchHistory__) */
