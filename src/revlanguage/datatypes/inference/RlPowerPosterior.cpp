
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "DistributionBeta.h"
#include "PowerPosteriorMcmc.h"
#include "Model.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "Real.h"
#include "RlPowerPosterior.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "TypeSpec.h"
#include "VectorRbPointer.h"
#include "Vector.h"


using namespace RevLanguage;

PowerPosterior::PowerPosterior() : RlControlVariableWrapper<RevBayesCore::PowerPosteriorMcmc>() {
    
}


PowerPosterior::PowerPosterior(const PowerPosterior &m) : RlControlVariableWrapper<RevBayesCore::PowerPosteriorMcmc>( m ), model( m.model ), moves( m.moves ) {
    
}


/** Clone object */
PowerPosterior* PowerPosterior::clone(void) const {
    
	return new PowerPosterior(*this);
}


void PowerPosterior::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getValue() ).getValue();
    const std::vector<RevBayesCore::Move *>&    mvs     = static_cast<const VectorRbPointer<Move> &>( moves->getValue() ).getValue();
    int k     = static_cast<const Natural &>( cats->getValue() ).getValue();
    
    value = new RevBayesCore::PowerPosteriorMcmc(mdl, mvs);
    
    std::vector<double> beta;
    for (int i = k; i >= 0; --i) {
        double b = RevBayesCore::RbStatistics::Beta::quantile(0.3,1.0,i / double(k));
        beta.push_back( b );
    }
    value->setBeta( beta );
    value->setSampleFreq( 10 );
}


/* Map calls to member methods */
RbLanguageObject* PowerPosterior::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
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
    else if (name == "pathSampling") {
        double ml = value->pathSampling();
        
        return new Real( ml );
    } 
    else if (name == "steppingStoneSampling") {
        double ml = value->steppingStoneSampling();
        
        return new Real( ml );
    } 
    else if (name == "all") {
        const std::vector<double>& vals = value->getSamples();
        
        return new Vector<Real>( vals );
    }
    
    return RbLanguageObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& PowerPosterior::getClassName(void) { 
    
    static std::string rbClassName = "PowerPosterior";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& PowerPosterior::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::PowerPosteriorMcmc>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& PowerPosterior::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("moves", true, VectorRbPointer<Move>::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("cats", true, Natural::getClassTypeSpec(), new Natural(20) ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */
const MethodTable& PowerPosterior::getMethods(void) const {
    
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
        
        ArgumentRules* pathSamplingArgRules = new ArgumentRules();
        methods.addFunction("pathSampling", new MemberFunction( Real::getClassTypeSpec(), pathSamplingArgRules) );
        
        ArgumentRules* steppingStoneSamplingArgRules = new ArgumentRules();
        methods.addFunction("steppingStoneSampling", new MemberFunction( Real::getClassTypeSpec(), steppingStoneSamplingArgRules) );
        
        ArgumentRules* allArgRules = new ArgumentRules();
        methods.addFunction("all", new MemberFunction( Vector<Real>::getClassTypeSpec(), allArgRules) );
                
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& PowerPosterior::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PowerPosterior::printValue(std::ostream &o) const {
    
    o << "PowerPosterior";
}


/** Set a member variable */
void PowerPosterior::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "model") {
        model = var;
    } 
    else if ( name == "moves") {
        moves = var;
    }
    else if ( name == "cats") {
        cats = var;
    }
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
