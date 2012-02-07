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
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionContinuous::DistributionContinuous( const MemberRules* memberRules ) : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
RbLanguageObject* DistributionContinuous::executeOperationSimple( const std::string& name, Environment* args ) {

    if ( name == "cdf" ) {

        return new RealPos( cdf( (*args)[1]->getValue() ) );
    }
    else if ( name == "quantile" ) {

        return quantile( static_cast<const Real*>( (*args)[1]->getValue() )->getValue() );
    }

    return Distribution::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistributionContinuous::getClass( void ) const {

    static VectorString rbClass = VectorString( DistributionContinuous_name ) + Distribution::getClass();
    return rbClass;
}


/** Get max value of distribution */
const Real* DistributionContinuous::getMax( void ) const {

    return new Real(RbConstants::Double::max);
}


/** Get min value of distribution */
const Real* DistributionContinuous::getMin( void ) const {

    return new Real(-RbConstants::Double::max);
}


/** Get method specifications */
const MethodTable* DistributionContinuous::getMethods( void ) const {

    static MethodTable* methods = new MethodTable();
    static ArgumentRules* cdfArgRules = new ArgumentRules();
    static ArgumentRules* quantileArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        cdfArgRules->push_back     ( new ValueRule    ( "q", RealPos_name      ) );

        quantileArgRules->push_back( new ValueRule    ( "p", RealPos_name      ) );

        methods->addFunction( "cdf",      new MemberFunction( Real_name, cdfArgRules      ) );
        methods->addFunction( "quantile", new MemberFunction( Real_name, quantileArgRules ) );

        methods->setParentTable( Distribution::getMethods() );

        methodsSet = true;
    }

    return methods;
}

