#ifndef CharacterHistoryRateModifier_H
#define CharacterHistoryRateModifier_H

#include <set>
#include <vector>
#include "Cloneable.h"

namespace RevBayesCore
{
    class CharacterEvent;
    class CharacterHistoryRateModifier : public Cloneable
    {
    public:
        CharacterHistoryRateModifier(size_t ns, size_t nc);
        CharacterHistoryRateModifier(const CharacterHistoryRateModifier& g);
        virtual double computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState) = 0;
        virtual void update(void) = 0;
        CharacterHistoryRateModifier* clone( void ) const = 0;

    protected:
        size_t num_states;
        size_t num_characters;

    private:


    };
    std::ostream& operator<<(std::ostream& o, const CharacterHistoryRateModifier& x);                                         //!< Overloaded output operator
    std::ostream& operator<<(std::ostream& o, const std::vector<CharacterHistoryRateModifier*>& x);                                         //!< Overloaded output operator
}

#endif
