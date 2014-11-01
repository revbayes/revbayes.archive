//
//  AbstractCharacterHistoryRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AbstractCharacterHistoryRateModifier__
#define __rb_mlandis__AbstractCharacterHistoryRateModifier__

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

#endif /* defined(__rb_mlandis__AbstractCharacterHistoryRateModifier__) */
