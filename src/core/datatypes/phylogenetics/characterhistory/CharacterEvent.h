#ifndef CharacterEvent_H
#define CharacterEvent_H

#include <stdlib.h>
#include <vector>
#include "Cloneable.h"
#include "CharacterEventType.h"
//#include "DiscreteCharacterState.h"

namespace RevBayesCore {
    
    class CharacterEvent : public Cloneable
    {

    public:
        CharacterEvent(void);
        CharacterEvent(size_t i, size_t s, double a, size_t t=CharacterEventType::UNDEFINED);
        CharacterEvent(const CharacterEvent& c);
        ~CharacterEvent(void);

        CharacterEvent*                     clone(void) const;
        double                              getAge(void) const;
        size_t                              getEventType(void) const;
        size_t                              getSiteIndex(void) const;
        size_t                              getState(void) const;
        std::string                         getStateStr(void) const;
        void                                setAge(double a);
        void                                setEventType(size_t t);
        void                                setSiteIndex(size_t i);
        void                                setState(size_t s);
        void                                print(void) const ;

        bool                                operator<(const CharacterEvent& lhs) const;
        bool                                operator>(const CharacterEvent& lhs) const;

    protected:


    private:
        size_t                              site_index;                                        // from 0 to N character ("sites")
        size_t                              state;                                             // from statespace -- possibly make a set of state/positions...
        double                              age;                                               // from 0 to 1
        size_t                              event_type;


    };
}


#endif
