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
ParserDistribution::ParserDistribution( const std::string &n, const MemberRules& mr, const RbPtr<RbLanguageObject> &rv ) : MemberObject( mr ), randomValue( rv ), memberRules( mr ), name( n ) {
}


/** Constructor with inheritance for member rules */
ParserDistribution::ParserDistribution( const ParserDistribution &p ) : MemberObject( p ), randomValue( p.randomValue->clone() ), memberRules( p.memberRules ), name( p.name ), params( p.params ) {
}



void ParserDistribution::clear( void ) {
    params.clear();
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


const TypeSpec& ParserDistribution::getVariableType( void ) const {
    return randomValue->getTypeSpec();
}


void ParserDistribution::setMember(std::string const &name, const RbPtr<const Variable> &var) {
    params.push_back( new ConstArgument(var, name) );
}

