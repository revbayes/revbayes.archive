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

#include "InferenceDistributionContinuous.h"
#include "ParserDistributionContinuous.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RealPos.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbUtil.h"
#include "ValueRule.h"

#include <cfloat>


/** Constructor passes member rules to base class */
ParserDistributionContinuous::ParserDistributionContinuous( InferenceDistributionContinuous *d, const MemberRules& mr, RbLanguageObject *rv ) : ParserDistribution( memberRules ), distribution( d ), max( d->getMax() ), min( d->getMin() ), memberRules( mr ), randomValue( rv ) {

}

ParserDistributionContinuous::ParserDistributionContinuous( const ParserDistributionContinuous &d) : ParserDistribution( d ), distribution( d.distribution->clone() ), max( d.getMax() ), min( d.getMin() ), memberRules( d.memberRules ), randomValue( d.randomValue->clone() ) {
    
}


double ParserDistributionContinuous::cdf(const Real &value) {
    
    std::vector<RbValue<void*> > newArgs;
    for (std::map<std::string, const Variable*>::iterator it = members.begin(); it != members.end(); ++it) {
        RbValue<void*> arg;
        arg.value = it->second->getValue().getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbLanguageObject* retVal = randomValue->clone();
    RbValue<void*> arg;
    arg.value = retVal->getValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    distribution->setParameters( newArgs );
    
    return distribution->cdf( value.getValue() );
}


ParserDistributionContinuous* ParserDistributionContinuous::clone(void) const {
    return new ParserDistributionContinuous(*this);
}


/** Map direct method calls to internal class methods. */
const RbLanguageObject& ParserDistributionContinuous::executeOperationSimple( const std::string& name, const std::vector<Argument>& args ) {
    
    if ( name == "cdf" ) {
        
        cd.setValue( cdf( static_cast<const Real&>( args[1].getVariable().getValue() ) ) );
        return cd;
    }
    else if ( name == "quantile" ) {
        
        quant.setValue( quantile( static_cast<const Real&>( args[1].getVariable().getValue() ).getValue() ) );
        return quant;
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


/** Get max value of ParserDistribution */
const Real& ParserDistributionContinuous::getMax( void ) const {
    
    return max;
}


/** Get min value of ParserDistribution */
const Real& ParserDistributionContinuous::getMin( void ) const {
    
    return min;
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
        
        cdfArgRules->push_back     ( new ValueRule    ( "q", RealPos::getClassTypeSpec()      ) );
        
        quantileArgRules->push_back( new ValueRule    ( "p", RealPos::getClassTypeSpec()      ) );
        
        methods.addFunction( "cdf",      new MemberFunction( Real::getClassTypeSpec(), cdfArgRules      ) );
        methods.addFunction( "quantile", new MemberFunction( Real::getClassTypeSpec(), quantileArgRules ) );
        
        methods.setParentTable( &ParserDistribution::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}


const TypeSpec& ParserDistributionContinuous::getTypeSpec(void) const {
    return getClassTypeSpec();
}


const TypeSpec& ParserDistributionContinuous::getVariableType( void ) const {
    return randomValue->getTypeSpec();
}


double ParserDistributionContinuous::lnPdf(const RbLanguageObject &value) const {
    
    std::vector<RbValue<void*> > newArgs;
    for (std::map<std::string, const Variable*>::const_iterator it = members.begin(); it != members.end(); ++it) {
        RbValue<void*> arg;
        arg.value = it->second->getValue().getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbValue<void*> arg;
    arg.value = value.getValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    distribution->setParameters( newArgs );
    
    return distribution->lnPdf(  );
}


double ParserDistributionContinuous::pdf(const RbLanguageObject &value) const {
    
    std::vector<RbValue<void*> > newArgs;
    for (std::map<std::string, const Variable*>::const_iterator it = members.begin(); it != members.end(); ++it) {
        RbValue<void*> arg;
        arg.value = it->second->getValue().getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbValue<void*> arg;
    arg.value = value.getValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    distribution->setParameters( newArgs );
    
    return distribution->pdf();
}


double ParserDistributionContinuous::quantile(const double p) {
    
    std::vector<RbValue<void*> > newArgs;
    for (std::map<std::string, const Variable*>::iterator it = members.begin(); it != members.end(); ++it) {
        RbValue<void*> arg;
        arg.value = it->second->getValue().getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbLanguageObject* retVal = randomValue->clone();
    RbValue<void*> arg;
    arg.value = retVal->getValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    distribution->setParameters( newArgs );
    
    return distribution->quantile( p );
}


const RbLanguageObject& ParserDistributionContinuous::rv(void) {
    
    std::vector<RbValue<void*> > newArgs;
    for (std::map<std::string, const Variable*>::iterator it = members.begin(); it != members.end(); ++it) {
        RbValue<void*> arg;
        arg.value = it->second->getValue().getValue(arg.lengths);
        newArgs.push_back( arg );
    }
    
    // add te return value
    RbLanguageObject* retVal = randomValue->clone();
    RbValue<void*> arg;
    arg.value = retVal->getValue(arg.lengths);
    newArgs.push_back( arg );
    
    // set the current parameter values of the function
    distribution->setParameters( newArgs );
    
    distribution->rv();
    
    return *retVal;
}




