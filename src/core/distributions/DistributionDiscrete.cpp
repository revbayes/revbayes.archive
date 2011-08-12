/**
 * @file
 * This file contains the implementation of DistributionDiscrete, the
 * abstract base class for distributions on discrete variables.
 *
 * @brief Partial implementation of DistributionDiscrete
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionDiscrete
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DistributionDiscrete.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RealPos.h"
#include "RbNames.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionDiscrete::DistributionDiscrete( const MemberRules& memberRules )
    : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
RbLanguageObject* DistributionDiscrete::executeOperation( const std::string& name, Environment& args ) {

    if ( name == "probMassVector" ) {

        return getProbabilityMassVector()->clone();
    }
    else if ( name == "numStates" ) {

        return new Natural( int( getNumberOfStates() ) );
    }

    return Distribution::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistributionDiscrete::getClass( void ) const {

    static VectorString rbClass = VectorString( DistributionDiscrete_name ) + Distribution::getClass();
    return rbClass;
}


/** Get method specifications */
const MethodTable& DistributionDiscrete::getMethods( void ) const {

    static MethodTable   methods;
    static ArgumentRules probMassVectorArgRules;
    static ArgumentRules numStatesArgRules;
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        probMassVectorArgRules.push_back( new ValueRule( "", MemberObject_name ) );

        numStatesArgRules.push_back     ( new ValueRule( "", MemberObject_name ) );

        methods.addFunction( "probMassVector", new MemberFunction( Simplex_name, probMassVectorArgRules ) );
        methods.addFunction( "numStates",      new MemberFunction( Natural_name, numStatesArgRules ) );

        methods.setParentTable( const_cast<MethodTable*>( &Distribution::getMethods() ) );

        methodsSet = true;
    }

    return methods;
}

