#ifndef CharacterHistoryRateModifier_H
#define CharacterHistoryRateModifier_H

#include <set>
#include <vector>
#include "Assignable.h"
#include "Cloneable.h"

namespace RevBayesCore
{
    class CharacterEvent;
    class CharacterHistoryRateModifier : public Cloneable, public Assignable
    {
    public:
        CharacterHistoryRateModifier(size_t ns, size_t nc);
        CharacterHistoryRateModifier(const CharacterHistoryRateModifier& g);
        
        virtual bool                        operator==(const CharacterHistoryRateModifier &rm) const { return this == &rm; }
        virtual bool                        operator!=(const CharacterHistoryRateModifier &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const CharacterHistoryRateModifier &rm) const { return this < &rm; }
        virtual bool                        operator<=(const CharacterHistoryRateModifier &rm) const { return operator<(rm) || operator==(rm); }
        
        virtual CharacterHistoryRateModifier& assign(const Assignable &m);
        virtual double                      computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0) = 0;
        virtual void                        update(void) = 0;
        CharacterHistoryRateModifier*       clone( void ) const = 0;

    protected:
        size_t                              num_states;
        size_t                              num_characters;

    private:


    };
    std::ostream& operator<<(std::ostream& o, const CharacterHistoryRateModifier& x);                                         //!< Overloaded output operator
    std::ostream& operator<<(std::ostream& o, const std::vector<CharacterHistoryRateModifier*>& x);                                         //!< Overloaded output operator
}

#endif
