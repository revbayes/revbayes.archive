//
//  BranchHistory.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BranchHistory__
#define __rb_mlandis__BranchHistory__

#include "CharacterEvent.h"
#include "CharacterEventCompare.h"
#include <vector>
#include <set>

namespace RevBayesCore {
    
    class CharacterEvent;

    class BranchHistory {
        
    public:
        BranchHistory(void);
        BranchHistory(size_t ns, size_t nc, size_t bi=-1, bool it=false, bool ir=false);
        BranchHistory& operator=(const BranchHistory& bh);
        
        BranchHistory* clone(void) const;
        
        const std::vector<CharacterEvent*>& getParentCharacters(void);
        const std::vector<CharacterEvent*>& getChildCharacters(void);
        const std::multiset<CharacterEvent*,CharacterEventCompare>& getHistory(void);
        
        void addEvent(CharacterEvent* evt);
        void removeEvent(CharacterEvent* evt);
        void clearEvents(void);
        void clearEvents(const std::set<size_t>& clearSet);
        void updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet);
        
        // used to 
        std::multiset<CharacterEvent*,CharacterEventCompare> swapHistoryEvents(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        std::set<CharacterEvent*> swapParentCharacters(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        std::set<CharacterEvent*> swapChildCharacters(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        
        void print(void);
        const std::set<size_t>& getDirtyCharacters(void);
        void setDirtyCharacters(const std::set<size_t>& s);
        
    protected:
        
        
    private:
        
        // container/element arguments
        size_t branchIndex;
        size_t numCharacters;
        size_t numStates;
        bool isTip;
        bool isRoot;
        
        // containers
        std::multiset<CharacterEvent*,CharacterEventCompare> history;
        std::vector<CharacterEvent*> parentCharacters;
        std::vector<CharacterEvent*> childCharacters;
        std::set<size_t>             dirtyCharacters;
    };
    
    std::ostream& operator<<(std::ostream& o, const BranchHistory& x);
}

#endif /* defined(__rb_mlandis__BranchHistory__) */
