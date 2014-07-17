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
    
    /*
    if (name == "nnodes") {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Natural( n );
    }
    else if (name == "height") {
        const RevBayesCore::TopologyNode& r = this->dagNode->getValue().getTipNode( 0 );
        return new RealPos( r.getBranchLength() );
    } 
    else if (name == "names") {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Vector<RlString>( n );
    } 
    */
    
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
        
        /*
        ArgumentRules* nnodesArgRules = new ArgumentRules();
        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
        
        ArgumentRules* heightArgRules = new ArgumentRules();
        methods.addFunction("height", new MemberFunction(Natural::getClassTypeSpec(),       heightArgRules              ) );
        
        ArgumentRules* namesArgRules = new ArgumentRules();
        methods.addFunction("names", new MemberFunction(Vector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        */
        
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

