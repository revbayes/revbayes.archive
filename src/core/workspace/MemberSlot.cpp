/**
 * @file
 * This file contains the implementation of MemberSlot, which
 * is used to manage variables in member object frames.
 *
 * @brief Implementation of MemberSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "MemberSlot.h"

#include <cassert>


/** Constructor of slot from argument (member) rule */
MemberSlot::MemberSlot( ArgumentRule* theRule )
    : VariableSlot( theRule->getArgTypeSpec() ), memberRule( theRule )  {
    
    if ( theRule->hasDefault() ) {

        if ( theRule->isReference() )
            setVariable( theRule->getDefaultReference() );
        else
            setValue( theRule->getDefaultValue() );
    }
}


/** Check against member rule if new variable is valid */
bool MemberSlot::isValidVariable( DAGNode* newVariable ) const {

    bool needsConversion;

    if ( newVariable->isImmutable() )
        return memberRule->isArgValid( newVariable, needsConversion, true );
    else
        return memberRule->isArgValid( newVariable, needsConversion, false );
}

