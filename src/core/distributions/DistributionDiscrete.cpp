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
#include "RbUtil.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionDiscrete::DistributionDiscrete( const MemberRules& memberRules ) : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
const RbLanguageObject& DistributionDiscrete::executeOperationSimple( const std::string& name, const std::vector<Argument>& args ) {

    if ( name == "probMassVector" ) {

        return getProbabilityMassVector();
    }
    else if ( name == "numStates" ) {
        
        numStates.setValue( getNumberOfStates() );
        return numStates;
    }

    return Distribution::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& DistributionDiscrete::getClassName(void) { 
    
    static std::string rbClassName = "Distribution on discrete random variable";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionDiscrete::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get method specifications */
const MethodTable& DistributionDiscrete::getMethods( void ) const {

    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;

    if ( !methodsSet ) {
        static ArgumentRules* probMassVectorArgRules = new ArgumentRules();
        static ArgumentRules* numStatesArgRules = new ArgumentRules();

        methods.addFunction( "probMassVector", new MemberFunction( Simplex::getClassTypeSpec(), probMassVectorArgRules ) );
        methods.addFunction( "numStates",      new MemberFunction( Natural::getClassTypeSpec(), numStatesArgRules ) );

        methods.setParentTable( &Distribution::getMethods() );

        methodsSet = true;
    }

    return methods;
}

