
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "Monitor.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlMonitor.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Monitor::Monitor(void) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>()
{
    
    // add method for call "addVariable" as a function
    ArgumentRules* addArgRules = new ArgumentRules();
    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL ) );
    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}


Monitor::Monitor(RevBayesCore::Monitor *m) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>( m )
{
    
    // add method for call "addVariable" as a function
    ArgumentRules* addArgRules = new ArgumentRules();
    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL ) );
    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}



RevPtr<RevVariable> Monitor::executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found)
{
    
    if ( name == "addVariable" )
    {
        found = true;
        
        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
        
        RevBayesCore::Monitor *m = static_cast<RevBayesCore::Monitor*>(this->value);
        m->addVariable( node );
        
        return NULL;
    }
    else if ( name == "removeVariable" )
    {
        found = true;
        
        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
        
        RevBayesCore::Monitor *m = static_cast<RevBayesCore::Monitor*>(this->value);
        m->removeVariable( node );
        
        return NULL;
    }
    
    return WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Monitor::getClassType(void)
{
    
    static std::string revType = "Monitor";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

