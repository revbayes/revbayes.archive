/* 
 * File:   RlMultivariatePhyloProcess.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:54
 */

#include "RlMultivariatePhyloProcess.h"


#include "Natural.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
MultivariatePhyloProcess::MultivariatePhyloProcess(void) : ModelObject<RevBayesCore::MultivariatePhyloProcess>() {
    
}

/** Construct from bool */
MultivariatePhyloProcess::MultivariatePhyloProcess(RevBayesCore::MultivariatePhyloProcess *t) : ModelObject<RevBayesCore::MultivariatePhyloProcess>( t ) {
    
}

/** Construct from bool */
MultivariatePhyloProcess::MultivariatePhyloProcess(const RevBayesCore::MultivariatePhyloProcess &t) : ModelObject<RevBayesCore::MultivariatePhyloProcess>( new RevBayesCore::MultivariatePhyloProcess( t ) ) {
    
}

/** Construct from bool */
MultivariatePhyloProcess::MultivariatePhyloProcess(RevBayesCore::TypedDagNode<RevBayesCore::MultivariatePhyloProcess> *n) : ModelObject<RevBayesCore::MultivariatePhyloProcess>( n ) {
    
}



/** Construct from bool */
MultivariatePhyloProcess::MultivariatePhyloProcess(const MultivariatePhyloProcess &t) : ModelObject<RevBayesCore::MultivariatePhyloProcess>( t ) {
    
}


/** Clone object */
MultivariatePhyloProcess* MultivariatePhyloProcess::clone(void) const {
    
	return new MultivariatePhyloProcess(*this);
}


/* Map calls to member methods */
RevLanguage::RevObject* MultivariatePhyloProcess::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "mean") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double mean = this->dagNode->getValue().getMean(k->getValue());
        return new Real( mean );
    }
    
    if (name == "stdev") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double mean = this->dagNode->getValue().getStdev(k->getValue());
        return new Real( mean );
    }
    
    if (name == "rootVal") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double mean = this->dagNode->getValue().getRootVal(k->getValue());
        return new Real( mean );
    }
    
    return ModelObject<RevBayesCore::MultivariatePhyloProcess>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& MultivariatePhyloProcess::getClassName(void) { 
    
    static std::string rbClassName = "MultivariatePhyloProcess";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MultivariatePhyloProcess::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& MultivariatePhyloProcess::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* meanArgRules = new ArgumentRules();
        meanArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("mean", new MemberFunction(Real::getClassTypeSpec(),       meanArgRules              ) );
        
        ArgumentRules* stdevArgRules = new ArgumentRules();
        stdevArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("stdev", new MemberFunction(Real::getClassTypeSpec(),       stdevArgRules              ) );
        
        ArgumentRules* rootArgRules = new ArgumentRules();
        rootArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("rootVal", new MemberFunction(Real::getClassTypeSpec(),       rootArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& MultivariatePhyloProcess::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

