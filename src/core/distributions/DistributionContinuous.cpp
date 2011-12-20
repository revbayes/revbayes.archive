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
DistributionContinuous::DistributionContinuous( RbPtr<const MemberRules> memberRules ) : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> DistributionContinuous::executeOperationSimple( const std::string& name, const RbPtr<Environment>& args ) {

    if ( name == "cdf" ) {

        return RbPtr<RbLanguageObject>( new RealPos( cdf( RbPtr<const RbLanguageObject>( (*args)[1]->getValue() ) ) ) );
    }
    else if ( name == "quantile" ) {

        RbPtr<RbLanguageObject> quant( quantile( static_cast<const Real*>( (const RbLanguageObject*)(*args)[1]->getValue() )->getValue() ) );

        return quant;
    }

    return Distribution::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistributionContinuous::getClass( void ) const {

    static VectorString rbClass = VectorString( DistributionContinuous_name ) + Distribution::getClass();
    return rbClass;
}


/** Get max value of distribution */
RbPtr<const Real> DistributionContinuous::getMax( void ) const {

    return RbPtr<const Real>( new Real(RbConstants::Double::max) );
}


/** Get min value of distribution */
RbPtr<const Real> DistributionContinuous::getMin( void ) const {

    return RbPtr<const Real>( new Real(-RbConstants::Double::max) );
}


/** Get method specifications */
RbPtr<const MethodTable> DistributionContinuous::getMethods( void ) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> cdfArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> quantileArgRules( new ArgumentRules() );
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        cdfArgRules->push_back     ( RbPtr<ArgumentRule>( new ValueRule    ( "q", RealPos_name      ) ) );

        quantileArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule    ( "p", RealPos_name      ) ) );

        methods->addFunction( "cdf",      RbPtr<RbFunction>( new MemberFunction( Real_name, cdfArgRules      ) ) );
        methods->addFunction( "quantile", RbPtr<RbFunction>( new MemberFunction( Real_name, quantileArgRules ) ) );

        methods->setParentTable( RbPtr<const FunctionTable>( Distribution::getMethods() ) );

        methodsSet = true;
    }

    return RbPtr<const MethodTable>( methods );
}

