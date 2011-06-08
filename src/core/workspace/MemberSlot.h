/**
 * @file
 * This file contains the declaration of MemberSlot, which is
 * used to manage variables in member object frames.
 *
 * @brief Declaration of MemberSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef MemberSlot_H 
#define MemberSlot_H

#include "DAGNode.h"
#include "TypeSpec.h"

#include <string>

class DAGNode;
class Frame;
class RbObject;

class MemberSlot : public VariableSlot {
    
    public:
                                MemberSlot(ArgumentRule* argRule);                              //!< Constructor from argument rule

        // Regular functions
        MemberSlot*             clone(void) const { return new MemberSlot(*this); }             //!< Clone slot
        bool                    isValidVariable(DAGNode* newVariable) const;                    //!< Is newVariable valid for the slot?

    private:
        // Member variables
        const ArgumentRule*     memberRule;                                                     //!< The rule for the member
};

#endif
