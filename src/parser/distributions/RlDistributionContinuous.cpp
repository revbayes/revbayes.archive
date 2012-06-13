/**
 * @file
 * This file contains the partial implementation of RlDistributionContinuous, the
 * abstract base class for RlDistributions on continuous variables.
 *
 * @brief Partial implementation of RlDistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface RlDistributionContinuous
 *
 * $Id: RlDistributionContinuous.cpp 1378 2012-04-17 00:27:08Z hoehna $
 */

#include "ArgumentRule.h"
#include "DistributionContinuous.h"
#include "RlDistributionContinuous.h"
#include "MethodTable.h"
#include "Probability.h"
#include "RbException.h"
#include "RealPos.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbUtil.h"
#include "SimpleMemberFunction.h"

#include <cfloat>


/** Constructor passes member rules to base class */
RlDistributionContinuous::RlDistributionContinuous( DistributionContinuous *d, const std::string &n, const MemberRules& mr, const RbPtr<RbLanguageObject> &rv ) : RlDistribution( d, n, mr, rv ) {

}

RlDistributionContinuous::RlDistributionContinuous( const RlDistributionContinuous &d) : RlDistribution( d ) {
    
}


double RlDistributionContinuous::cdf(const Real &value) {
    
    return static_cast<DistributionContinuous *>( distribution )->cdf( value.getValue() );
}


RlDistributionContinuous* RlDistributionContinuous::clone(void) const {
    return new RlDistributionContinuous(*this);
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> RlDistributionContinuous::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if ( name == "cdf" ) {
        
        return RbPtr<RbLanguageObject>( new Probability( cdf( static_cast<const Real&>( *args[1] ) ) ) );
    }
    else if ( name == "quantile" ) {
        
        return RbPtr<RbLanguageObject>( new Real( quantile( static_cast<const Real&>( *args[1] ).getValue() ) ) );
    }
    
    return RlDistribution::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& RlDistributionContinuous::getClassName(void) { 
    
    static std::string rbClassName = "Parser Distribution on continuous random variable";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlDistributionContinuous::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlDistribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get max value of RlDistribution */
double RlDistributionContinuous::getMax( void ) const {
    
    return static_cast<DistributionContinuous *>( distribution )->getMax();
}


/** Get min value of RlDistribution */
double RlDistributionContinuous::getMin( void ) const {
    
    return static_cast<DistributionContinuous *>( distribution )->getMin();
}


/** Get method specifications */
const MethodTable& RlDistributionContinuous::getMethods( void ) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* cdfArgRules = new ArgumentRules();
    static ArgumentRules* quantileArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( !methodsSet ) {
        
        cdfArgRules->push_back     ( new ArgumentRule    ( "q", true, RealPos::getClassTypeSpec()      ) );
        
        quantileArgRules->push_back( new ArgumentRule    ( "p", true, RealPos::getClassTypeSpec()      ) );
        
        methods.addFunction( "cdf",      new SimpleMemberFunction( Probability::getClassTypeSpec(), cdfArgRules      ) );
        methods.addFunction( "quantile", new SimpleMemberFunction( Real::getClassTypeSpec(), quantileArgRules ) );
        
        methods.setParentTable( &RlDistribution::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}


double RlDistributionContinuous::quantile(const double p) {
    
    return static_cast<DistributionContinuous *>( distribution )->quantile( p );
}




