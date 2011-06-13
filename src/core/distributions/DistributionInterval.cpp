/**
 * @file
 * This file contains the partial implementation of DistributionInterval, the
 * abstract base class for distributions on interval variables.
 *
 * @brief Partial implementation of DistributionInterval
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionInterval
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ContainerNode.h"
#include "DAGNode.h"
#include "DistributionInterval.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "RbException.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "RbNames.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cfloat>


/** Constructor passes member rules to base class */
DistributionInterval::DistributionInterval( const MemberRules& memberRules )
    : Distribution( memberRules ) {
}


/** Map direct method calls to internal class methods. */
DAGNode* DistributionInterval::executeOperation( const std::string& name, ArgumentFrame& args ) {

    if ( name == "cdf" ) {

        return new ConstantNode( new RealPos( cdf( args[0].getValue() ) ) );
    }
    else if ( name == "quantile" ) {

        RbObject* quant = quantile( static_cast<const Real*>( args[0].getValue() )->getValue() );

        if ( quant->isType( Container_name ) )
            return new ContainerNode( static_cast<Container*>( quant ) );
        else if ( quant->isType( MemberObject_name ) )
            return new MemberNode( static_cast<MemberObject*>( quant ) );
        else
            return new ConstantNode( quant );
    }

    return Distribution::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistributionInterval::getClass( void ) const {

    static VectorString rbClass = VectorString( DistributionInterval_name ) + Distribution::getClass();
    return rbClass;
}


/** Get max value of distribution */
const RbObject* DistributionInterval::getMax( void ) const {

    throw RbException( "DistributionInterval getMax not implemented" );
}


/** Get min value of distribution */
const RbObject* DistributionInterval::getMin( void ) const {

    throw RbException( "DistributionInterval getMin not implemented" );
}


/** Get method specifications */
const MethodTable& DistributionInterval::getMethods( void ) const {

    static MethodTable   methods;
    static ArgumentRules cdfArgRules;
    static ArgumentRules quantileArgRules;
    static bool          methodsSet = false;

    if ( !methodsSet ) {

        cdfArgRules.push_back     ( new ReferenceRule( "",  MemberObject_name ) );
        cdfArgRules.push_back     ( new ValueRule    ( "q", RealPos_name      ) );

        quantileArgRules.push_back( new ReferenceRule( "",  MemberObject_name ) );
        quantileArgRules.push_back( new ValueRule    ( "p", RealPos_name      ) );

        methods.addFunction( "cdf",      new MemberFunction( Real_name, cdfArgRules      ) );
        methods.addFunction( "quantile", new MemberFunction( Real_name, quantileArgRules ) );

        methods.setParentTable( const_cast<MethodTable*>( &Distribution::getMethods() ) );

        methodsSet = true;
    }

    return methods;
}

