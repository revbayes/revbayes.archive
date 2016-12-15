#ifndef AbstractCharacterHistoryRateModifier_H
#define AbstractCharacterHistoryRateModifier_H

#include <set>
#include <vector>
#include "Cloneable.h"

namespace RevBayesCore
{
    class CharacterEvent;
    class AbstractCharacterHistoryRateModifier : public Cloneable
    {
    public:
        AbstractCharacterHistoryRateModifier(void);
        virtual double computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState) = 0;
        virtual void update(void) = 0;
        AbstractCharacterHistoryRateModifier* clone( void ) const = 0;

    protected:


    private:


    };
    std::ostream& operator<<(std::ostream& o, const AbstractCharacterHistoryRateModifier& x);                                         //!< Overloaded output operator
    std::ostream& operator<<(std::ostream& o, const std::vector<AbstractCharacterHistoryRateModifier*>& x);                                         //!< Overloaded output operator
}

#endif
