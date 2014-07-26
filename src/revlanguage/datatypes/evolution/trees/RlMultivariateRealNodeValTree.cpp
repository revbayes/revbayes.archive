/* 
 * File:   RlMultivariateRealNodeValTree.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:54
 */

#include "RlMultivariateRealNodeValTree.h"

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
MultivariateRealNodeValTree::MultivariateRealNodeValTree(void) : ModelObject<RevBayesCore::MultivariateRealNodeContainer>() {
    
}

/** Construct from bool */
MultivariateRealNodeValTree::MultivariateRealNodeValTree(RevBayesCore::MultivariateRealNodeContainer *t) : ModelObject<RevBayesCore::MultivariateRealNodeContainer>( t ) {
    
}

/** Construct from bool */
MultivariateRealNodeValTree::MultivariateRealNodeValTree(const RevBayesCore::MultivariateRealNodeContainer &t) : ModelObject<RevBayesCore::MultivariateRealNodeContainer>( new RevBayesCore::MultivariateRealNodeContainer( t ) ) {
    
}

/** Construct from bool */
MultivariateRealNodeValTree::MultivariateRealNodeValTree(RevBayesCore::TypedDagNode<RevBayesCore::MultivariateRealNodeContainer> *n) : ModelObject<RevBayesCore::MultivariateRealNodeContainer>( n ) {
    
}



/** Construct from bool */
MultivariateRealNodeValTree::MultivariateRealNodeValTree(const MultivariateRealNodeValTree &t) : ModelObject<RevBayesCore::MultivariateRealNodeContainer>( t ) {
    
}


/** Clone object */
MultivariateRealNodeValTree* MultivariateRealNodeValTree::clone(void) const {
    
	return new MultivariateRealNodeValTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<Variable> MultivariateRealNodeValTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    /*
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
    */
    if ( name == "clampAt" )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData >* data = static_cast<const AbstractCharacterData &>( args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::TypedDagNode< int >* l = static_cast<const Integer &>( args[2].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::AbstractCharacterData* d = & data->getValue();
        RevBayesCore::ContinuousCharacterData* c = static_cast<RevBayesCore::ContinuousCharacterData*>(d);
        
        this->dagNode->getValue().clampAt(c, k->getValue(), l->getValue());   
        return new Variable( new Real( 0 ) );
    }

    return ModelObject<RevBayesCore::MultivariateRealNodeContainer>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& MultivariateRealNodeValTree::getClassType(void) { 
    
    static std::string revClassType = "MultivariateRealNodeValTree";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& MultivariateRealNodeValTree::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/* Get method specifications */
const RevLanguage::MethodTable& MultivariateRealNodeValTree::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false )
    {
        
        ArgumentRules* meanArgRules = new ArgumentRules();
        meanArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("mean", new MemberFunction<MultivariateRealNodeValTree,Real>( this, meanArgRules ) );
        
        ArgumentRules* tipmeanArgRules = new ArgumentRules();
        tipmeanArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("tipMean", new MemberFunction<MultivariateRealNodeValTree,Real>( this, tipmeanArgRules ) );
        
        ArgumentRules* stdevArgRules = new ArgumentRules();
        stdevArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("stdev", new MemberFunction<MultivariateRealNodeValTree,RealPos>(  this, stdevArgRules ) );
        
        ArgumentRules* rootArgRules = new ArgumentRules();
        rootArgRules->push_back(new ArgumentRule("index", false, Natural::getClassTypeSpec()));
        methods.addFunction("rootVal", new MemberProcedure(Real::getClassTypeSpec(), rootArgRules ) );
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back(new ArgumentRule("data", false, AbstractCharacterData::getClassTypeSpec()));
        clampArgRules->push_back(new ArgumentRule("processIndex", false, Natural::getClassTypeSpec()));
        clampArgRules->push_back(new ArgumentRule("dataIndex", false, Natural::getClassTypeSpec()));
        methods.addFunction("clampAt", new MemberProcedure(MultivariateRealNodeValTree::getClassTypeSpec(), clampArgRules ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::MultivariateRealNodeContainer>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& MultivariateRealNodeValTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void MultivariateRealNodeValTree::printValue(std::ostream &o) const {

    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );
    o << dagNode->getValue();
    
    o.setf( previousFlags );
    o.precision( previousPrecision );

}


