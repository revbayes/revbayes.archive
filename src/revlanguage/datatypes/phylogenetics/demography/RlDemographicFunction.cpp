
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "DemographicFunction.h"
#include "OptionRule.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlDemographicFunction.h"
#include "TypeSpec.h"


using namespace RevLanguage;

DemographicFunction::DemographicFunction(void) : WorkspaceToCoreWrapperObject<RevBayesCore::DemographicFunction>()
{
    
//    // add method for call "addVariable" as a function
//    ArgumentRules* addArgRules = new ArgumentRules();
//    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to DemographicFunction.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
//    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}


DemographicFunction::DemographicFunction(RevBayesCore::DemographicFunction *m) : WorkspaceToCoreWrapperObject<RevBayesCore::DemographicFunction>( m )
{
    
//    // add method for call "addVariable" as a function
//    ArgumentRules* addArgRules = new ArgumentRules();
//    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to DemographicFunction.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
//    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}



RevPtr<RevVariable> DemographicFunction::executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found)
{
    
//    if ( name == "addVariable" )
//    {
//        found = true;
//
//        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
//
//        RevBayesCore::DemographicFunction *f = static_cast<RevBayesCore::DemographicFunction*>(this->value);
//        f->addVariable( node );
//
//        return NULL;
//    }
//    else if ( name == "removeVariable" )
//    {
//        found = true;
//
//        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
//
//        RevBayesCore::DemographicFunction *f = static_cast<RevBayesCore::DemographicFunction*>(this->value);
//        f->removeVariable( node );
//
//        return NULL;
//    }
    
    return WorkspaceToCoreWrapperObject<RevBayesCore::DemographicFunction>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& DemographicFunction::getClassType(void)
{
    
    static std::string rev_type = "DemographicFunction";
    
	return rev_type; 
}

/** Get class type spec describing type of object */
const TypeSpec& DemographicFunction::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::DemographicFunction>::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the aliases of the Rev name for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> DemographicFunction::getConstructorFunctionAliases( void ) const
{
    // create a constructor function name alias variable that is the same for all instance of this class
    std::vector<std::string> aliases;
    
    std::vector<std::string> DemographicFunction_aliases = getDemographicFunctionAliases();
    for (size_t i=0; i < DemographicFunction_aliases.size(); ++i)
    {
        std::string tmp = DemographicFunction_aliases[i];
        std::string c_name = "df" + StringUtilities::firstCharToUpper( tmp );
        
        aliases.push_back( c_name );
    }
    
    return aliases;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string DemographicFunction::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string tmp = getDemographicFunctionName();
    std::string c_name = "df" + StringUtilities::firstCharToUpper( tmp );
    
    return c_name;
}


/** Get type spec */
void DemographicFunction::printValue(std::ostream &o, bool user) const
{
    
    printValue(o);
}

/** Get type spec */
void DemographicFunction::printValue(std::ostream &o) const
{
    
    o << getDemographicFunctionName();
}




