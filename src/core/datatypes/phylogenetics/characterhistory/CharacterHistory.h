#ifndef CharacterHistory_H
#define CharacterHistory_H

#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
    
    
    class Tree;
    
    class CharacterHistory : public Cloneable {
        
    public:
        //CharacterHistory(void);
        CharacterHistory(Tree *t, size_t nc, size_t ns, bool rb = false);
        virtual ~CharacterHistory(void);
        
        const BranchHistory&                    operator[](size_t i) const;
//        BranchHistory&                          operator[](size_t i);
        
        CharacterHistory*                       clone(void) const;
        
        void                                    addEvent(CharacterEvent *e, size_t bi);
        size_t                                  getNumberBranches(void) const;
        size_t                                  getNumberEvents(void) const;
        size_t                                  getNumberStates(void) const;
        bool                                    hasRootBranch(void) const;
        CharacterEvent*                         pickRandomEvent(size_t &bi);
        void                                    removeEvent(CharacterEvent *e, size_t bi);
        void                                    setTree(Tree *t);
        
        
    private:
                
        Tree*                                   tree;
        std::vector<BranchHistory>              histories;
        size_t                                  n_branches;
        size_t                                  n_character;
        size_t                                  n_events;
        size_t                                  n_states;
        bool                                    useRootBranch;
        
        
    };
    
    std::ostream& operator<<(std::ostream& o, const CharacterHistory& x);
}

#endif
