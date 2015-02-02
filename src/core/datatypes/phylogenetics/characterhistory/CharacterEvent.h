//
//  CharacterEvent.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterEvent__
#define __rb_mlandis__CharacterEvent__

#include <stdlib.h>
#include <vector>
#include "Cloneable.h"
//#include "DiscreteCharacterState.h"

namespace RevBayesCore {

    class CharacterEvent : public Cloneable
    {

    public:
        
        CharacterEvent(void);
        CharacterEvent(size_t i, unsigned int s, double t);
        CharacterEvent(const CharacterEvent& c);
        ~CharacterEvent(void);
        
        CharacterEvent*                     clone(void) const;
        double                              getTime(void);
        size_t                              getIndex(void);
        unsigned int                        getState(void);
        std::string                         getStateStr(void);
        void                                setState(unsigned int s);
        void                                setTime(double t);
        void                                print(void);
        
        bool                                operator<(const CharacterEvent& lhs) const;
        
    protected:
        
        
    private:
        size_t index;   // from 0 to N "sites"
        unsigned int state;  // from statespace -- possibly make a set of state/positions...
        double time; // from 0 to 1
      

    };
}


#endif /* defined(__rb_mlandis__CharacterEvent__) */
