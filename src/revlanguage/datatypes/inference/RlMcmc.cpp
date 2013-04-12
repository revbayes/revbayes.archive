
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "TypeSpec.h"
#include "VectorPointer.h"


using namespace RevLanguage;

Mcmc::Mcmc() : RlControlVariableWrapper<RevBayesCore::Mcmc>() {
    
}


Mcmc::Mcmc(const Mcmc &m) : RlControlVariableWrapper<RevBayesCore::Mcmc>( m ), model( m.model ), moves( m.moves ), monitors( m.monitors ) {
    
}


/** Clone object */
Mcmc* Mcmc::clone(void) const {
    
	return new Mcmc(*this);
}


void Mcmc::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getValue() ).getValue();
    const std::vector<RevBayesCore::Move *>&    mvs     = static_cast<const VectorPointer<Move> &>( moves->getValue() ).getValue();
    const std::vector<RevBayesCore::Monitor *>& mntr    = static_cast<const VectorPointer<Monitor> &>( monitors->getValue() ).getValue();
    value = new RevBayesCore::Mcmc(mdl, mvs, mntr);
}


/* Map calls to member methods */
RbLanguageObject* Mcmc::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "run") {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getValue() ).getValue();
        value->run( gen );
        
        return NULL;
    } 
    else if (name == "burnin") {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getValue() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getValue() ).getValue();
        value->burnin( gen, tuningInterval );
        
        return NULL;
    } 
    else if ( name == "operatorSummary") {
        value->printOperatorSummary();
        
        return NULL;
    }
    
    return RbLanguageObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& Mcmc::getClassName(void) { 
    
    static std::string rbClassName = "MCMC";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mcmc::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::Mcmc>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Mcmc::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("monitors", true, VectorPointer<Monitor>::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("moves", true, VectorPointer<Move>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */
const MethodTable& Mcmc::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* runArgRules = new ArgumentRules();
        runArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        methods.addFunction("run", new MemberFunction( RbVoid_name, runArgRules) );
        
        ArgumentRules* burninArgRules = new ArgumentRules();
        burninArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        burninArgRules->push_back( new ArgumentRule("tuningInterval", true, Natural::getClassTypeSpec()) );
        methods.addFunction("burnin", new MemberFunction( RbVoid_name, burninArgRules) );
        
        ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
        methods.addFunction("operatorSummary", new MemberFunction( RbVoid_name, operatorSummaryArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& Mcmc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mcmc::printValue(std::ostream &o) const {
    
    o << "Mcmc";
}


/** Set a member variable */
void Mcmc::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "model") {
        model = var;
    } 
    else if ( name == "moves") {
        moves = var;
    }
    else if ( name == "monitors") {
        monitors = var;
    }
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
