/* 
 * File:   RlMultivariatePhyloProcess.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:54
 */

#include "RlMultivariatePhyloProcess.h"

#include "Natural.h"
#include "RbUtil.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "RlAbstractCharacterData.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"

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
RevLanguage::RevPtr<Variable> MultivariatePhyloProcess::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "rootVal") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double rootval = this->dagNode->getValue().getRootVal(k->getValue());
        return new Variable( new Real( rootval ) );
    }
    else if (name == "mean") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double mean = this->dagNode->getValue().getMean(k->getValue());
        return new Variable( new Real( mean ) );
    }
    else if (name == "stdev") {        
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getDagNode();
        double stdev = this->dagNode->getValue().getStdev(k->getValue());
        return new Variable( new Real( stdev ) );
    }    
    else if ( name == "clampAt" )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData >* data = static_cast<const AbstractCharacterData &>( args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::TypedDagNode< int >* l = static_cast<const Integer &>( args[2].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::AbstractCharacterData* d = & data->getValue();
        RevBayesCore::ContinuousCharacterData* c = static_cast<RevBayesCore::ContinuousCharacterData*>(d);
        
        this->dagNode->getValue().clampAt(c, k->getValue(), l->getValue());   
        return new Variable( new Real( 0 ) );
    }

    return ModelObject<RevBayesCore::MultivariatePhyloProcess>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& MultivariatePhyloProcess::getClassType(void) { 
    
    static std::string revClassType = "MultivariatePhyloProcess";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& MultivariatePhyloProcess::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/* Get method specifications */
const RevLanguage::MethodTable& MultivariatePhyloProcess::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false )
    {
        
        ArgumentRules* meanArgRules = new ArgumentRules();
        meanArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("mean", new MemberFunction<MultivariatePhyloProcess,Real>( this, meanArgRules ) );
        
        ArgumentRules* stdevArgRules = new ArgumentRules();
        stdevArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("stdev", new MemberFunction<MultivariatePhyloProcess,RealPos>(  this, stdevArgRules ) );
        
        ArgumentRules* rootArgRules = new ArgumentRules();
        rootArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("rootVal", new MemberProcedure(Real::getClassTypeSpec(), rootArgRules ) );
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back(new ArgumentRule("data", false, AbstractCharacterData::getClassTypeSpec()));
        clampArgRules->push_back(new ArgumentRule("processIndex", false, Natural::getClassTypeSpec()));
        clampArgRules->push_back(new ArgumentRule("dataIndex", false, Natural::getClassTypeSpec()));
        methods.addFunction("clampAt", new MemberProcedure(MultivariatePhyloProcess::getClassTypeSpec(), clampArgRules ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::MultivariatePhyloProcess>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& MultivariatePhyloProcess::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void MultivariatePhyloProcess::printValue(std::ostream &o) const {

    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );
    
    dagNode->printValue( o, "" );
    
    o.setf( previousFlags );
    o.precision( previousPrecision );

}


