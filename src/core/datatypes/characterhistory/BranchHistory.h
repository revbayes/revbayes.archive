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
#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
    
    class CharacterEvent;

    class BranchHistory : public Cloneable {
        
    public:
        //BranchHistory(void);
        BranchHistory(size_t nc, size_t ns, size_t idx);
        BranchHistory(size_t nc, size_t ns, size_t idx, std::set<int> sc);
        BranchHistory(const BranchHistory& m);
        ~BranchHistory(void);
        BranchHistory& operator=(const BranchHistory& bh);
        BranchHistory* clone(void) const;
        //bool operator()(const BranchHistory&) const;
        bool operator<(const BranchHistory&) const;
        
        const size_t getNumCharacters(void) const;
        const size_t getNumStates(void) const;
        const size_t getNumEvents(void) const;
        const size_t getIndex(void) const;
        
        std::vector<CharacterEvent*>& getParentCharacters(void);
        const std::vector<CharacterEvent*>& getParentCharacters(void) const;
        std::vector<CharacterEvent*>& getChildCharacters(void);
        const std::vector<CharacterEvent*>& getChildCharacters(void) const;
        std::multiset<CharacterEvent*,CharacterEventCompare>& getHistory(void);
        const std::multiset<CharacterEvent*,CharacterEventCompare>& getHistory(void) const;

        
        void addEvent(CharacterEvent* evt);
        void removeEvent(CharacterEvent* evt);
        void clearEvents(void);
        void clearEvents(const std::set<size_t>& clearSet);
        void updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet);
        void updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<size_t>& indexSet);
        
        // used to 
        //std::multiset<CharacterEvent*,CharacterEventCompare> swapHistoryEvents(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        //std::set<CharacterEvent*> swapParentCharacters(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        //std::set<CharacterEvent*> swapChildCharacters(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);
        void setParentCharacters(const std::set<CharacterEvent*>& s);
        void setParentCharacters(const std::vector<CharacterEvent*>& s);
        void setChildCharacters(const std::set<CharacterEvent*>& s);
        void setChildCharacters(const std::vector<CharacterEvent*>& s);
        void setHistory(const std::set<CharacterEvent*,CharacterEventCompare>& s);
        void setHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& s);

        
        void print(void) const;
        const std::set<size_t>& getDirtyCharacters(void);
        void setDirtyCharacters(const std::set<size_t>& s);
        
        bool getRedrawParentCharacters(void);
        bool getRedrawChildCharacters(void);
        bool getRedrawHistory(void);
        void setRedrawParentCharacters(bool tf);
        void setRedrawChildCharacters(bool tf);
        void setRedrawHistory(bool tf);
        
        bool isClampedChildCharacters(void);
        void setClampChildCharacters(bool tf);

        const std::set<int>& getSampleChildCharacters(void);
        void setSampleChildCharacters(std::set<int> sc);

        
    protected:
        
        // container/element arguments
        size_t numCharacters;
        size_t numStates;
        size_t index;
        
        // containers
        mutable std::multiset<CharacterEvent*,CharacterEventCompare> history;
        mutable std::vector<CharacterEvent*> parentCharacters;
        mutable std::vector<CharacterEvent*> childCharacters;
        
        // add per-state count
        //std::vector<std::vector<unsigned int> > parentCharacterCounts;
        //std::vector<std::vector<unsigned int> > childCharacterCounts;
        
        //
        
        bool redrawChildCharacters;
        bool redrawParentCharacters;
        bool redrawHistory;
        bool clampChildCharacters;
        
        std::set<int>               sampleChildCharacters;
        std::set<size_t>             dirtyCharacters;
        
    private:
        
        
    };
    
    std::ostream& operator<<(std::ostream& o, const BranchHistory& x);
}

#endif /* defined(__rb_mlandis__BranchHistory__) */
