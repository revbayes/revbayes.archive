/**
 * @file
 * This file contains the implementation of DistributionCategorical, the
 * abstract base class for distributions on categorical variables.
 *
 * @brief Partial implementation of DistributionCategorical
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionCategorical
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ContainerNode.h"
#include "DAGNode.h"
#include "DistributionCategorical.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "Natural.h"
#include "RbException.h"
#include "RealPos.h"
#include "RbNames.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionCategorical::DistributionCategorical( const MemberRules& memberRules )
    : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
DAGNode* DistributionCategorical::executeMethod( const std::string& name, ArgumentFrame& args ) {

    if ( name == "probMassVector" ) {

        return new MemberNode( getProbabilityMassVector()->clone() );
    }
    else if ( name == "numStates" ) {

        return new ConstantNode( new Natural( getNumStates() ) );
    }

    return Distribution::executeMethod( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistributionCategorical::getClass( void ) const {

    static VectorString rbClass = VectorString( DistributionCategorical_name ) + Distribution::getClass();
    return rbClass;
}


/** Get method specifications */
const MethodTable& DistributionCategorical::getMethods( void ) const {

    static MethodTable   methods;
    static ArgumentRules probMassVectorArgRules;
    static ArgumentRules numStatesArgRules;
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        methods.addFunction( "probMassVector", new MemberFunction( Simplex_name, ArgumentRules() ) );
        methods.addFunction( "numStates"     , new MemberFunction( Natural_name, ArgumentRules() ) );

        methods.setParentTable( const_cast<MethodTable*>( &Distribution::getMethods() ) );

        methodsSet = true;
    }

    return methods;
}

