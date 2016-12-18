//
//  BranchHistory.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef BranchHistory_H
#define BranchHistory_H

#include "CharacterEvent.h"
#include "CharacterEventCompare.h"
#include "Cloneable.h"
#include "TopologyNode.h"

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

        bool                                                            areEventTimesValid(const TopologyNode &node) const;

        const size_t                                                    getBranchIndex(void) const;
        CharacterEvent*                                                 getEvent(size_t i);
        const size_t                                                    getNumberCharacters(void) const;
        const size_t                                                    getNumberStates(void) const;
        const size_t                                                    getNumberEvents(void) const;

        std::vector<CharacterEvent*>&                                   getParentCharacters(void);
        const std::vector<CharacterEvent*>&                             getParentCharacters(void) const;
        std::vector<CharacterEvent*>&                                   getChildCharacters(void);
        const std::vector<CharacterEvent*>&                             getChildCharacters(void) const;
        std::multiset<CharacterEvent*,CharacterEventCompare>&           getHistory(void);
        const std::multiset<CharacterEvent*,CharacterEventCompare>&     getHistory(void) const;


        void                                                            addEvent(CharacterEvent* evt);
        void                                                            removeEvent(CharacterEvent* evt);
        void                                                            clearEvents(void);
        void                                                            clearEvents(const std::set<size_t>& clearSet);
        void                                                            updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet);
        void                                                            updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<size_t>& indexSet);
        void                                                            updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet);

        void                                                            setParentCharacters(const std::set<CharacterEvent*>& s);
        void                                                            setParentCharacters(const std::vector<CharacterEvent*>& s);
        void                                                            setChildCharacters(const std::set<CharacterEvent*>& s);
        void                                                            setChildCharacters(const std::vector<CharacterEvent*>& s);
        void                                                            setHistory(const std::set<CharacterEvent*,CharacterEventCompare>& s);
        void                                                            setHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& s);
        
        void                                                            print(void) const;

    protected:

        // container/element arguments
        size_t                                                          n_characters;
        size_t                                                          n_states;
        size_t                                                          branch_index;

        // containers
        mutable std::multiset<CharacterEvent*,CharacterEventCompare>    history;
        mutable std::vector<CharacterEvent*>                            parentCharacters;
        mutable std::vector<CharacterEvent*>                            childCharacters;


    };

    std::ostream& operator<<(std::ostream& o, const BranchHistory& x);
}

#endif
