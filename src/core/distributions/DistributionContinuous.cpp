/**
 * @file
 * This file contains the partial implementation of DistributionContinuous, the
 * abstract base class for distributions on continuous variables.
 *
 * @brief Partial implementation of DistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionContinuous
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DistributionContinuous.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RealPos.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbUtil.h"
#include "ValueRule.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionContinuous::DistributionContinuous( const MemberRules& memberRules ) : Distribution( memberRules ), max( RbConstants::Double::max ), min( -RbConstants::Double::max ){
}


/** Map direct method calls to internal class methods. */
const RbLanguageObject& DistributionContinuous::executeOperationSimple( const std::string& name, const std::vector<Argument>& args ) {

    if ( name == "cdf" ) {

        cd.setValue( cdf( args[1].getVariable().getValue() ) );
        return cd;
    }
    else if ( name == "quantile" ) {

        return quantile( static_cast<const Real&>( args[1].getVariable().getValue() ).getValue() );
    }

    return Distribution::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& DistributionContinuous::getClassName(void) { 
    
    static std::string rbClassName = "Distribution on continuous random variable";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionContinuous::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get max value of distribution */
const Real& DistributionContinuous::getMax( void ) const {

    return max;
}


/** Get min value of distribution */
const Real& DistributionContinuous::getMin( void ) const {

    return min;
}


/** Get method specifications */
const MethodTable& DistributionContinuous::getMethods( void ) const {

    static MethodTable methods = MethodTable();
    static ArgumentRules* cdfArgRules = new ArgumentRules();
    static ArgumentRules* quantileArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        cdfArgRules->push_back     ( new ValueRule    ( "q", RealPos::getClassTypeSpec()      ) );

        quantileArgRules->push_back( new ValueRule    ( "p", RealPos::getClassTypeSpec()      ) );

        methods.addFunction( "cdf",      new MemberFunction( Real::getClassTypeSpec(), cdfArgRules      ) );
        methods.addFunction( "quantile", new MemberFunction( Real::getClassTypeSpec(), quantileArgRules ) );

        methods.setParentTable( &Distribution::getMethods() );

        methodsSet = true;
    }

    return methods;
}

