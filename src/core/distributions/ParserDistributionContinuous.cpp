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

#include "ConstArgumentRule.h"
#include "DistributionContinuous.h"
#include "ParserDistributionContinuous.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Probability.h"
#include "RbException.h"
#include "RealPos.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbUtil.h"

#include <cfloat>


/** Constructor passes member rules to base class */
ParserDistributionContinuous::ParserDistributionContinuous( DistributionContinuous *d, const std::string &n, const MemberRules& mr, const RbPtr<RbLanguageObject> &rv ) : ParserDistribution( n, mr, rv ), distribution( d ), typeSpec( getClassName() + " (" + n + ")", new TypeSpec( ParserDistribution::getClassTypeSpec() )) {

}

ParserDistributionContinuous::ParserDistributionContinuous( const ParserDistributionContinuous &d) : ParserDistribution( d ), distribution( d.distribution->clone() ), typeSpec( d.typeSpec ) {
    
}


ParserDistributionContinuous& ParserDistributionContinuous::operator=(const ParserDistributionContinuous &d) {
    // check for self assignment
    if ( this != &d ) {
        ParserDistribution::operator=( d );
        
        delete distribution;
        
        distribution = d.distribution->clone();
    }
    
    return *this;
}


double ParserDistributionContinuous::cdf(const Real &value) {
    
    return distribution->cdf( value.getValue() );
}


ParserDistributionContinuous* ParserDistributionContinuous::clone(void) const {
    return new ParserDistributionContinuous(*this);
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> ParserDistributionContinuous::executeOperationSimple( const std::string& name, const std::vector<RbPtr<Argument> >& args ) {
    
    if ( name == "cdf" ) {
        
        return RbPtr<RbLanguageObject>( new Probability( cdf( static_cast<const Real&>( args[1]->getVariable()->getValue() ) ) ) );
    }
    else if ( name == "quantile" ) {
        
        return RbPtr<RbLanguageObject>( new Real( quantile( static_cast<const Real&>( args[1]->getVariable()->getValue() ).getValue() ) ) );
    }
    
    return ParserDistribution::executeOperationSimple( name, args );
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


DistributionContinuous* ParserDistributionContinuous::getLeanDistribution( void ) const {
    return distribution;
}


/** Get max value of ParserDistribution */
double ParserDistributionContinuous::getMax( void ) const {
    
    return distribution->getMax();
}


/** Get min value of ParserDistribution */
double ParserDistributionContinuous::getMin( void ) const {
    
    return distribution->getMin();
}

const MemberRules& ParserDistributionContinuous::getMemberRules(void) const {
    return memberRules;
}


/** Get method specifications */
const MethodTable& ParserDistributionContinuous::getMethods( void ) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* cdfArgRules = new ArgumentRules();
    static ArgumentRules* quantileArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( !methodsSet ) {
        
        cdfArgRules->push_back     ( new ConstArgumentRule    ( "q", RealPos::getClassTypeSpec()      ) );
        
        quantileArgRules->push_back( new ConstArgumentRule    ( "p", RealPos::getClassTypeSpec()      ) );
        
        methods.addFunction( "cdf",      new MemberFunction( Probability::getClassTypeSpec(), cdfArgRules      ) );
        methods.addFunction( "quantile", new MemberFunction( Real::getClassTypeSpec(), quantileArgRules ) );
        
        methods.setParentTable( &ParserDistribution::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}


const TypeSpec& ParserDistributionContinuous::getTypeSpec(void) const {
    return typeSpec;
}


double ParserDistributionContinuous::jointLnPdf(const RbLanguageObject &value) const {
    
    std::vector<size_t> lengths;
    value.getLeanValue( lengths );
    
    double *pds = distribution->lnPdf();
    
    if ( lengths.size() > 0 ) {
        double lnPd = 0.0;
        size_t index = 0;
        for ( size_t i = 0; i < lengths.size(); ++i) {
            for ( size_t j = 0; j < lengths[i]; ++j, ++index) {
                lnPd += pds[index];
            }
        }
        return lnPd;
    }
    else {
        return *pds;
    }
}


double ParserDistributionContinuous::lnPdf(const RbLanguageObject &value) const {
    
    return *distribution->lnPdf();
}


double ParserDistributionContinuous::pdf(const RbLanguageObject &value) const {
    
    return *distribution->pdf();
}


double ParserDistributionContinuous::quantile(const double p) {
    
    return distribution->quantile( p );
}


void ParserDistributionContinuous::rv(void) {
    
    distribution->rv();
}

/** We delegate the call to the inference distribution. */
void ParserDistributionContinuous::setParameters(const std::vector<RbValue<void *> > &p) {
    distribution->setParameters(p);
}

/** We delegate the call to the inference distribution. */
void ParserDistributionContinuous::setValue(const RbValue<void *> &v) {
    distribution->setObservedValue(v);
}




