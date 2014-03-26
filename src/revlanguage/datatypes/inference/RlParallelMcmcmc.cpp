//
//  RlParallelMcmcmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/25/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "OptionRule.h"
#include "ParallelMcmcmc.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlParallelMcmcmc.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "VectorRbPointer.h"


using namespace RevLanguage;

ParallelMcmcmc::ParallelMcmcmc() : RlControlVariableWrapper<RevBayesCore::ParallelMcmcmc>() {
    
}


ParallelMcmcmc::ParallelMcmcmc(const ParallelMcmcmc &m) : RlControlVariableWrapper<RevBayesCore::ParallelMcmcmc>( m ), model( m.model ), moves( m.moves ), monitors( m.monitors ) {
    
}


/** Clone object */
ParallelMcmcmc* ParallelMcmcmc::clone(void) const {
    
	return new ParallelMcmcmc(*this);
}


void ParallelMcmcmc::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new MCMC object
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getValue() ).getValue();
    const std::vector<RevBayesCore::Move *>&    mvs     = static_cast<const VectorRbPointer<Move> &>( moves->getValue() ).getValue();
    const std::vector<RevBayesCore::Monitor *>& mntr    = static_cast<const VectorRbPointer<Monitor> &>( monitors->getValue() ).getValue();
    const std::string &                         sched   = static_cast<const RlString &>( moveSchedule->getValue() ).getValue();
    const int                                   nc      = static_cast<const Natural&>( numChains->getValue() ).getValue();
    const int                                   np      = static_cast<const Natural&>( numProcessors->getValue() ).getValue();
    const int                                   si      = static_cast<const Natural&>( swapInterval->getValue() ).getValue();
    const double                                sh      = static_cast<const Real&>( startHeat->getValue() ).getValue();
    const double                                delth   = static_cast<const Real&>( deltaHeat->getValue() ).getValue();
    const double                                sigh    = static_cast<const Real&>( sigmaHeat->getValue() ).getValue();
    
    value = new RevBayesCore::ParallelMcmcmc(mdl, mvs, mntr, sched, nc, np, si, delth, sigh, sh);
    
}


/* Map calls to member methods */
RbLanguageObject* ParallelMcmcmc::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "run")
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getValue() ).getValue();
        value->run( gen );
        
        return NULL;
    }
    else if (name == "burnin")
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getValue() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getValue() ).getValue();
        value->burnin( gen, tuningInterval );
        
        return NULL;
    }
    else if ( name == "operatorSummary")
    {
        value->printOperatorSummary();
        
        return NULL;
    }
    
    return RbLanguageObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& ParallelMcmcmc::getClassName(void) {
    
    static std::string rbClassName = "ParallelMcmcmc";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& ParallelMcmcmc::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::Mcmc>::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& ParallelMcmcmc::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("monitors", true, VectorRbPointer<Monitor>::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("moves", true, VectorRbPointer<Move>::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("numChains", true, Natural::getClassTypeSpec(), new Natural(4) ) );
        memberRules.push_back( new ArgumentRule("numProcessors", true, Natural::getClassTypeSpec(), new Natural(4) ) );
        memberRules.push_back( new ArgumentRule("swapInterval", true, Natural::getClassTypeSpec(), new Natural(100)) );
        memberRules.push_back( new ArgumentRule("deltaHeat", true, Real::getClassTypeSpec(), new Real(0.2) ) );
        memberRules.push_back( new ArgumentRule("sigmaHeat", true, Real::getClassTypeSpec(), new Real(1.0) ) );
        memberRules.push_back( new ArgumentRule("startHeat", true, Real::getClassTypeSpec(), new Real(1.0)) );
        
        Vector<RlString> options;
        options.push_back( RlString("sequential") );
        options.push_back( RlString("random") );
        options.push_back( RlString("single") );
        
        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& ParallelMcmcmc::getMethods(void) const {
    
    static MethodTable   methods    = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* runArgRules = new ArgumentRules();
        runArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        methods.addFunction("run", new MemberFunction( RlUtils::Void, runArgRules) );
        
        ArgumentRules* burninArgRules = new ArgumentRules();
        burninArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        burninArgRules->push_back( new ArgumentRule("tuningInterval", true, Natural::getClassTypeSpec()) );
        methods.addFunction("burnin", new MemberFunction( RlUtils::Void, burninArgRules) );
        
        ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
        methods.addFunction("operatorSummary", new MemberFunction( RlUtils::Void, operatorSummaryArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& ParallelMcmcmc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ParallelMcmcmc::printValue(std::ostream &o) const {
    
    o << "ParallelMcmcmc";
}


/** Set a member variable */
void ParallelMcmcmc::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "model") {
        model = var;
    }
    else if ( name == "moves") {
        moves = var;
    }
    else if ( name == "monitors") {
        monitors = var;
    }
    else if ( name == "moveschedule") {
        moveSchedule = var;
    }
    else if ( name == "numChains" ) {
        numChains = var;
    }
    else if ( name == "numProcessors" ) {
        numProcessors = var;
    }
    else if ( name == "deltaHeat" )
    {
        deltaHeat = var;
    }
    else if ( name == "sigmaHeat" )
    {
        sigmaHeat = var;
    }
    else if ( name == "startHeat" )
    {
        startHeat = var;
    }
    else if ( name == "swapInterval" )
    {
        swapInterval = var;
    }
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
