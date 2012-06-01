/**
 * @file
 * This file contains the partial implementation of ParserDistributionContinuous, the
 * abstract base class for ParserDistributions on continuous variables.
 *
 * @brief Partial implementation of ParserDistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface ParserDistributionContinuous
 *
 * $Id: ParserDistributionContinuous.cpp 1378 2012-04-17 00:27:08Z hoehna $
 */

#include "ArgumentRule.h"
#include "DistributionContinuous.h"
#include "ParserDistributionContinuous.h"
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
ParserDistributionContinuous::ParserDistributionContinuous( DistributionContinuous *d, const std::string &n, const MemberRules& mr, const RbPtr<RbLanguageObject> &rv ) : ParserDistribution( d, n, mr, rv ) {

}

ParserDistributionContinuous::ParserDistributionContinuous( const ParserDistributionContinuous &d) : ParserDistribution( d ) {
    
}


double ParserDistributionContinuous::cdf(const Real &value) {
    
    return static_cast<DistributionContinuous *>( distribution )->cdf( value.getValue() );
}


ParserDistributionContinuous* ParserDistributionContinuous::clone(void) const {
    return new ParserDistributionContinuous(*this);
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> ParserDistributionContinuous::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if ( name == "cdf" ) {
        
        return RbPtr<RbLanguageObject>( new Probability( cdf( static_cast<const Real&>( *args[1] ) ) ) );
    }
    else if ( name == "quantile" ) {
        
        return RbPtr<RbLanguageObject>( new Real( quantile( static_cast<const Real&>( *args[1] ).getValue() ) ) );
    }
    
    return ParserDistribution::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& ParserDistributionContinuous::getClassName(void) { 
    
    static std::string rbClassName = "Parser Distribution on continuous random variable";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserDistributionContinuous::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ParserDistribution::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get max value of ParserDistribution */
double ParserDistributionContinuous::getMax( void ) const {
    
    return static_cast<DistributionContinuous *>( distribution )->getMax();
}


/** Get min value of ParserDistribution */
double ParserDistributionContinuous::getMin( void ) const {
    
    return static_cast<DistributionContinuous *>( distribution )->getMin();
}


/** Get method specifications */
const MethodTable& ParserDistributionContinuous::getMethods( void ) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* cdfArgRules = new ArgumentRules();
    static ArgumentRules* quantileArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( !methodsSet ) {
        
        cdfArgRules->push_back     ( new ArgumentRule    ( "q", true, RealPos::getClassTypeSpec()      ) );
        
        quantileArgRules->push_back( new ArgumentRule    ( "p", true, RealPos::getClassTypeSpec()      ) );
        
        methods.addFunction( "cdf",      new SimpleMemberFunction( Probability::getClassTypeSpec(), cdfArgRules      ) );
        methods.addFunction( "quantile", new SimpleMemberFunction( Real::getClassTypeSpec(), quantileArgRules ) );
        
        methods.setParentTable( &ParserDistribution::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}


double ParserDistributionContinuous::quantile(const double p) {
    
    return static_cast<DistributionContinuous *>( distribution )->quantile( p );
}




