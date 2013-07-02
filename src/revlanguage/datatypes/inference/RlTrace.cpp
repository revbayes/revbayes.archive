//
//  RlTrace.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/27/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlTrace.h"

#include "ArgumentRules.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"

using namespace RevLanguage;

Trace::Trace() : RlControlVariableWrapper<RevBayesCore::Trace>() {
    
}


Trace::Trace(const RevBayesCore::Trace &t) : RlControlVariableWrapper<RevBayesCore::Trace>( new RevBayesCore::Trace( t ) ) {
    
}


Trace::Trace(const Trace &t) : RlControlVariableWrapper<RevBayesCore::Trace>( t ) {
    
}


/** Clone object */
Trace* Trace::clone(void) const {
    
	return new Trace(*this);
}


void Trace::computeStatistics( void ) {
    value->computeStatistics();
}


void Trace::constructInternalObject( void ) {
    throw RbException("We do not support a constructor function for Trace.");
}


/* Map calls to member methods */
RbLanguageObject* Trace::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "summarize") {
        
//        int b = static_cast<const Natural &>( args[0].getVariable()->getValue() ).getValue();
//        RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( *this->value );
//        summary.summarize( b );
//        summary.printTreeSummary(std::cerr);
        
        return NULL;
    } 
    
    return RbLanguageObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& Trace::getClassName(void) { 
    
    static std::string rbClassName = "Trace";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Trace::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::Trace>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Trace::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        //        modelMemberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        //        modelMemberRules.push_back( new ArgumentRule("monitors", true, VectorRbPointer<Monitor>::getClassTypeSpec() ) );
        //        modelMemberRules.push_back( new ArgumentRule("moves", true, VectorRbPointer<Move>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */
const MethodTable& Trace::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* summarizeArgRules = new ArgumentRules();
        summarizeArgRules->push_back( new ArgumentRule("burnin", true, Natural::getClassTypeSpec(), new Natural(0)) );
        methods.addFunction("summarize", new MemberFunction( RbVoid_name, summarizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& Trace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Trace::printValue(std::ostream &o) const {
    
    o << "Filename:                  " << value->getFileName() << std::endl;
    o << "Parameter:                 " << value->getParameterName() << std::endl;
    o << "Samples:                   " << value->getSamples() << std::endl;
    o << "Stepsize:                  " << value->getStepSize() << std::endl;
    o << "ESS:                       " << value->getEss() << std::endl;
    o << "Burnin:                    " << value->getBurnin() << std::endl;
    o << "Mean:                      " << value->getMean() << std::endl;
    o << "SEM:                       " << value->getSem() << std::endl;
    o << "Geweke-test:               " << (value->hasPassedGewekeTest() ? "TRUE" : "FALSE") << std::endl;
    o << "stationarity-test:         " << (value->hasPassedStationarityTest() ? "TRUE" : "FALSE") << std::endl;
//    o << "Heidelberger-Welch-test:   " << (value->hasPassedHeidelbergerWelchTest() ? "TRUE" : "FALSE") << std::endl;
}


/** Set a member variable */
void Trace::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
