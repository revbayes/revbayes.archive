#ifndef CharacterEventContinuous_H
#define CharacterEventContinuous_H

#include <stdlib.h>
#include <vector>

#include "CharacterEvent.h"
#include "CharacterEventType.h"

namespace RevBayesCore {
    
    class CharacterEventContinuous : public CharacterEvent
    {
        
    public:
        CharacterEventContinuous(void);
        CharacterEventContinuous(size_t ch_idx, double val, double a, size_t t=CharacterEventType::UNDEFINED);
        CharacterEventContinuous(const CharacterEventContinuous& c);
        ~CharacterEventContinuous(void);
        
        CharacterEventContinuous*           clone(void) const;
        double                              getState(void) const;
        std::string                         getStateStr(void) const;
        void                                setState(double s);
        
        
    private:

        double                              value;                                             // the value/state of this event
        
        
    };
}


#endif

