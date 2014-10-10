
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "Move.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlMove.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move::Move(void) : WorkspaceObject<RevBayesCore::Move>() {
    
}


Move::Move(const Move &m) : WorkspaceObject<RevBayesCore::Move>( m ) {
    
}


/** Get Rev type of object */
const std::string& Move::getClassType(void) { 
    
    static std::string revType = "Move";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::Move>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules */
const MemberRules& Move::getMemberRules(void) const {
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "weight", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos( 1.0 ) ) );
        
        /* Inherit variables from WorkspaceObject, put them last */
        const MemberRules& inheritedRules = WorkspaceObject<RevBayesCore::Move>::getMemberRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}


/** Set a member variable */
void Move::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "weight" ) 
    {
        weight = var;
    }
    else 
    {
        RevObject::setConstMemberVariable(name, var);
    }
}

