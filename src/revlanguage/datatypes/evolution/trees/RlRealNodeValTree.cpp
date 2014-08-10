/* 
 * File:   RlRealNodeValTree.cpp
 * Author: nl
 * 
 * Created on 26 juillet 2014, 13:03
 */

#include "RlRealNodeValTree.h"

#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RbUtil.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "RlAbstractCharacterData.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RealNodeValTree::RealNodeValTree(void) : ModelObject<RevBayesCore::RealNodeContainer>() {
    
}

/** Construct from bool */
RealNodeValTree::RealNodeValTree(RevBayesCore::RealNodeContainer *t) : ModelObject<RevBayesCore::RealNodeContainer>( t ) {
    
}

/** Construct from bool */
RealNodeValTree::RealNodeValTree(const RevBayesCore::RealNodeContainer &t) : ModelObject<RevBayesCore::RealNodeContainer>( new RevBayesCore::RealNodeContainer( t ) ) {
    
}

/** Construct from bool */
RealNodeValTree::RealNodeValTree(RevBayesCore::TypedDagNode<RevBayesCore::RealNodeContainer> *n) : ModelObject<RevBayesCore::RealNodeContainer>( n ) {
    
}



/** Construct from bool */
RealNodeValTree::RealNodeValTree(const RealNodeValTree &t) : ModelObject<RevBayesCore::RealNodeContainer>( t ) {
    
}


/** Clone object */
RealNodeValTree* RealNodeValTree::clone(void) const {
    
	return new RealNodeValTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<Variable> RealNodeValTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if ( name == "clampAt" )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::AbstractCharacterData >* data = static_cast<const AbstractCharacterData &>( args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::TypedDagNode< int >* k = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::AbstractCharacterData* d = & data->getValue();
        RevBayesCore::ContinuousCharacterData* c = static_cast<RevBayesCore::ContinuousCharacterData*>(d);
        
        this->dagNode->getValue().clampAt(c, k->getValue());   
        return new Variable( new Real( 0 ) );
    }

    return ModelObject<RevBayesCore::RealNodeContainer>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& RealNodeValTree::getClassType(void) { 
    
    static std::string revClassType = "RealNodeValTree";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& RealNodeValTree::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/* Get method specifications */
const RevLanguage::MethodTable& RealNodeValTree::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false )
    {
        
        ArgumentRules* meanArgRules = new ArgumentRules();
        methods.addFunction("mean", new MemberFunction<RealNodeValTree,Real>( this, meanArgRules ) );
        
        ArgumentRules* tipmeanArgRules = new ArgumentRules();
        methods.addFunction("tipMean", new MemberFunction<RealNodeValTree,Real>( this, tipmeanArgRules ) );
        
        ArgumentRules* stdevArgRules = new ArgumentRules();
        methods.addFunction("stdev", new MemberFunction<RealNodeValTree,RealPos>(  this, stdevArgRules ) );
        
        ArgumentRules* rootArgRules = new ArgumentRules();
        methods.addFunction("rootVal", new MemberProcedure(Real::getClassTypeSpec(), rootArgRules ) );
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back(new ArgumentRule("data", false, AbstractCharacterData::getClassTypeSpec()));
        clampArgRules->push_back(new ArgumentRule("dataIndex", false, Natural::getClassTypeSpec()));
        methods.addFunction("clampAt", new MemberProcedure(RealNodeValTree::getClassTypeSpec(), clampArgRules ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::RealNodeContainer>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& RealNodeValTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void RealNodeValTree::printValue(std::ostream &o) const {

    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );
    o << dagNode->getValue();
    
    o.setf( previousFlags );
    o.precision( previousPrecision );

}


