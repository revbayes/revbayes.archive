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
#include "Natural.h"
#include "OptionRule.h"
#include "ParallelMcmcmc.h"
#include "Real.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlParallelMcmcmc.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

ParallelMcmcmc::ParallelMcmcmc() : WorkspaceObject<RevBayesCore::ParallelMcmcmc>() {
    
}


ParallelMcmcmc::ParallelMcmcmc(const ParallelMcmcmc &m) : WorkspaceObject<RevBayesCore::ParallelMcmcmc>( m ), model( m.model ), moves( m.moves ), monitors( m.monitors ) {
    
}


/** Clone object */
ParallelMcmcmc* ParallelMcmcmc::clone(void) const {
    
	return new ParallelMcmcmc(*this);
}


void ParallelMcmcmc::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
#ifndef USE_LIB_OPENMP2
    throw RbException("You must #define USE_LIB_OPENMP to use ParallelMcmcmc");
    return;
#endif
    
    // now allocate a new MCMC object
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const RevBayesCore::RbVector<RevBayesCore::Move>&    mvs     = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() ).getVectorRbPointer();
    const RevBayesCore::RbVector<RevBayesCore::Monitor>& mntr    = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() ).getVectorRbPointer();
    const std::string &                         sched   = static_cast<const RlString &>( moveSchedule->getRevObject() ).getValue();
    const int                                   nc      = static_cast<const Natural&>( numChains->getRevObject() ).getValue();
    const int                                   np      = static_cast<const Natural&>( numProcessors->getRevObject() ).getValue();
    const int                                   si      = static_cast<const Natural&>( swapInterval->getRevObject() ).getValue();
    const double                                sh      = static_cast<const Real&>( startHeat->getRevObject() ).getValue();
    const double                                delth   = static_cast<const Real&>( deltaHeat->getRevObject() ).getValue();
    const double                                sigh    = static_cast<const Real&>( sigmaHeat->getRevObject() ).getValue();
    
    value = new RevBayesCore::ParallelMcmcmc(mdl, mvs, mntr, sched, nc, np, si, delth, sigh, sh);
    
}


/* Map calls to member methods */
RevPtr<Variable> ParallelMcmcmc::executeMethod(std::string const &name, const std::vector<Argument> &args) {

    
    if (name == "run")
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->run( gen );
        
        return NULL;
    }
    else if (name == "burnin")
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( gen, tuningInterval );
        
        return NULL;
    }
    else if ( name == "operatorSummary")
    {
        value->printOperatorSummary();
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& ParallelMcmcmc::getClassType(void) {
    
    static std::string revType = "ParallelMcmcmc";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& ParallelMcmcmc::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::Mcmc>::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& ParallelMcmcmc::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("model"        , Model::getClassTypeSpec()                   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        memberRules.push_back( new ArgumentRule("monitors"     , WorkspaceVector<Monitor>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("moves"        , WorkspaceVector<Move>::getClassTypeSpec()   , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("numChains"    , Natural::getClassTypeSpec()                 , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(4) ) );
        memberRules.push_back( new ArgumentRule("numProcessors", Natural::getClassTypeSpec()                 , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(4) ) );
        memberRules.push_back( new ArgumentRule("swapInterval" , Natural::getClassTypeSpec()                 , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100)) );
        memberRules.push_back( new ArgumentRule("deltaHeat"    , Real::getClassTypeSpec()                    , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(0.2) ) );
        memberRules.push_back( new ArgumentRule("sigmaHeat"    , Real::getClassTypeSpec()                    , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        memberRules.push_back( new ArgumentRule("startHeat"    , Real::getClassTypeSpec()                    , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0)) );
        
        std::vector<std::string> options;
        options.push_back( "sequential" );
        options.push_back( "random" );
        options.push_back( "single" );
        
        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
void ParallelMcmcmc::initializeMethods(void) const
{
    
    // necessary call for proper inheritance
    RevObject::initializeMethods();
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
        
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );
        
    ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
    methods.addFunction("operatorSummary", new MemberProcedure( RlUtils::Void, operatorSummaryArgRules) );
    
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
void ParallelMcmcmc::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        RevObject::setConstParameter(name, var);
    }
}
