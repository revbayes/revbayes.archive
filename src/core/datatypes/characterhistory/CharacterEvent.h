//
//  CharacterEvent.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterEvent__
#define __rb_mlandis__CharacterEvent__

#include <vector>
#include "DiscreteCharacterState.h"

namespace RevBayesCore {

    class CharacterEvent
    {

    public:
        
        CharacterEvent(void);
        CharacterEvent(DiscreteCharacterState* s, size_t i, double t);
        CharacterEvent(const CharacterEvent& c);
        ~CharacterEvent(void);
        
        double getTime(void);
        size_t getIndex(void);
        DiscreteCharacterState* getState(void);
        void setState(DiscreteCharacterState* s);
        void print(void);
        
        bool operator<(const CharacterEvent& lhs) const;
        
    protected:
        
        
    private:
        DiscreteCharacterState* state;  // from statespace -- possibly make a set of state/positions...
        double time;    // from 0 to 1
        size_t index;   // from 0 to N "sites"

    };
}


#endif /* defined(__rb_mlandis__CharacterEvent__) */
