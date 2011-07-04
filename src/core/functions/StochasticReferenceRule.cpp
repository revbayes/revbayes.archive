/**
 * @file
 * This file contains the declaration of StochasticReferenceRule,
 * which is used to describe reference rules that require the
 * variable to be a stochastic node.
 *
 * @brief Implementation of StochasticReferenceRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StochasticReferenceRule.h"
#include "VectorString.h"

#include <sstream>


/** Construct rule without default value; use "" for no label. */
StochasticReferenceRule::StochasticReferenceRule( const std::string& argName, TypeSpec typeSp )
    : ReferenceRule( argName, typeSp ) {
}


/** Clone object */
StochasticReferenceRule* StochasticReferenceRule::clone( void ) const {

    return new StochasticReferenceRule( *this );
}


/** Get class vector describing type of object */
const VectorString& StochasticReferenceRule::getClass( void ) const { 

    static VectorString rbClass = VectorString( StochasticReferenceRule_name ) + ReferenceRule::getClass();
	return rbClass; 
}


/** Test if argument is valid */
bool StochasticReferenceRule::isArgValid( DAGNode* var, bool& needsConversion, bool once ) const {

    if ( !var->isDAGType( StochasticNode_name ) )
        return false;
    
    return ReferenceRule::isArgValid( var, needsConversion, once );
}


/** Print value for user */
void StochasticReferenceRule::printValue(std::ostream& o) const {

    o << "<Stochastic> ";
    
    ReferenceRule::printValue(o);
}


/** Provide complete information about object */
std::string StochasticReferenceRule::richInfo(void) const {

    std::ostringstream o;

    o << "StochasticReferenceRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;

    return o.str();
}

