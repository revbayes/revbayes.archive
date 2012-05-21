/**
 * @file
 * This file contains the partial implementation of ParserDistribution, the abstract
 * base class for ParserDistributions in RevBayes.
 *
 * @brief Partial implementation of ParserDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface ParserDistribution
 *
 * $Id: ParserDistribution.cpp 1353 2012-03-19 08:25:05Z hoehna $
 */

#include "ConstantNode.h"
#include "ConstArgument.h"
#include "ConstArgumentRule.h"
#include "ParserDistribution.h"
#include "Environment.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "SimpleMemberFunction.h"
#include "Workspace.h"


/** Constructor with inheritance for member rules */
ParserDistribution::ParserDistribution( Distribution *d, const std::string &n, const MemberRules& mr, const RbPtr<RbLanguageObject> &rv ) : MemberObject( mr ), distribution( d ), randomValue( rv ), memberRules( mr ), name( n ), typeSpec( getClassName() + " (" + n + ")", new TypeSpec( MemberObject::getClassTypeSpec() )) {
}


/** Constructor with inheritance for member rules */
ParserDistribution::ParserDistribution( const ParserDistribution &p ) : MemberObject( p ), distribution( p.distribution->clone() ), randomValue( p.randomValue->clone() ), memberRules( p.memberRules ), name( p.name ), params( p.params ), typeSpec( p.typeSpec ) {
}


ParserDistribution& ParserDistribution::operator=(const ParserDistribution &d) {
    // check for self assignment
    if ( this != &d ) {
        MemberObject::operator=( d );
        
        delete distribution;
        
        distribution    = d.distribution->clone();
        name            = d.name;
        memberRules     = d.memberRules;
        params          = d.params;
        randomValue     = d.randomValue->clone();
        typeSpec        = d.typeSpec;
    }
    
    return *this;
}


void ParserDistribution::clear( void ) {
    params.clear();
}


ParserDistribution* ParserDistribution::clone(void) const {
    return new ParserDistribution(*this);
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> ParserDistribution::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if ( name == "lnPdf" ) {
        
        return RbPtr<RbLanguageObject>( new Real( lnPdf( static_cast<const RbLanguageObject &>( *args[1] ) ) ) );
    }
    else if ( name == "pdf" ) {
        
        return RbPtr<RbLanguageObject>( new RealPos( pdf( static_cast<const RbLanguageObject &>( *args[1] ) ) ) );
    }
    else if ( name == "rv" ) {
        
        rv();
        return randomValue;
    }
    else
        return MemberObject::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& ParserDistribution::getClassName(void) { 
    
    static std::string rbClassName = "ParserDistribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}




Distribution* ParserDistribution::getLeanDistribution( void ) const {
    return distribution;
}


const MemberRules& ParserDistribution::getMemberRules(void) const {
    return memberRules;
}


/** Get methods */
const MethodTable& ParserDistribution::getMethods( void ) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules*    lnPdfArgRules = new ArgumentRules();
    static ArgumentRules*    pdfArgRules = new ArgumentRules();
    static ArgumentRules*    rvArgRules = new ArgumentRules();
    static bool             methodsSet = false;
    
    if ( !methodsSet ) {
        
        lnPdfArgRules->push_back( new ConstArgumentRule( "x", RbObject::getClassTypeSpec() ) );
        
        pdfArgRules->push_back( new ConstArgumentRule( "x", RbObject::getClassTypeSpec() ) );
        
        methods.addFunction( "lnPdf", new SimpleMemberFunction( Real::getClassTypeSpec()    , lnPdfArgRules ) );
        methods.addFunction( "pdf",   new SimpleMemberFunction( Real::getClassTypeSpec()    , pdfArgRules   ) );
        methods.addFunction( "rv",    new SimpleMemberFunction( RbObject::getClassTypeSpec(), rvArgRules    ) );
        
        methods.setParentTable( &MemberObject::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}    

const std::vector<RbPtr<Argument> >& ParserDistribution::getParameters( void ) const {
    return params;
}


const RbLanguageObject& ParserDistribution::getTemplateRandomVariable( void ) const {
    return *randomValue;
}   


const TypeSpec& ParserDistribution::getTypeSpec(void) const {
    return typeSpec;
}


const TypeSpec& ParserDistribution::getVariableType( void ) const {
    return randomValue->getTypeSpec();
}


double ParserDistribution::jointLnPdf(const RlValue<RbLanguageObject> &value) const {
    
    std::vector<size_t> lengths = value.lengths;
    
    // compute all the probability densities (pd's)
    double *pds = distribution->lnPdf();
    
    // sum all pd's together
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


double ParserDistribution::lnPdf(const RbLanguageObject &value) const {
    
    return *distribution->lnPdf();
}


double ParserDistribution::pdf(const RbLanguageObject &value) const {
    
    return *distribution->pdf();
}


void ParserDistribution::rv(void) {
    
    distribution->rv();
}


void ParserDistribution::setConstMember(std::string const &name, const RbPtr<const Variable> &var) {
    params.push_back( new ConstArgument(var, name) );
}

/** We delegate the call to the inference distribution. */
void ParserDistribution::setParameters(const std::vector<RbValue<void *> > &p) {
    distribution->setParameters(p);
}


void ParserDistribution::setSimpleMemberValue(std::string const &name, const RbPtr<const RbLanguageObject> &var) {
    // we do nothing, we catch just the call because it leads to an exception otherwise.
}


/** We delegate the call to the inference distribution. */
void ParserDistribution::setValue(const RbValue<void *> &v) {
    distribution->setObservedValue(v);
}

