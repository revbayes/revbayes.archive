#include "Argument.h"
#include "ArgumentRule.h"
#include "DagNode.h"
#include "Func_structure.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RlBoolean.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Default constructor */
Func_structure::Func_structure( void ) : Procedure()
{
    
}


/** Clone object */
Func_structure* Func_structure::clone( void ) const
{
    
    return new Func_structure( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_structure::execute( void )
{
    
    std::ostringstream o;
    
    bool verbose = static_cast< const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();

    // get a reference to the variable from which we want to know the struct
    RevPtr<RevVariable> &the_var = args[0].getVariable();
    
    o << std::endl;
    if ( verbose == true )
    {
        
        // print the name and the memory address
        if ( the_var->getName() == "" )
        {
            o << "_variable     = <unnamed> <" << the_var << ">" << std::endl;
        }
        else
        {
            o << "_variable     = " << the_var->getName() << " <" << the_var << ">" << std::endl;
        }
        
    }
    else
    {
        
        // print only the name
        if ( the_var->getName() == "" )
        {
            o << "_variable     = <unnamed>" << std::endl;
        }
        else
        {
            o << "_variable     = " << the_var->getName() << std::endl;
        }
        
    }
    
    if ( the_var->isWorkspaceVariable() && verbose == true )
    {
        o << "_varType      = workspace (control)" << std::endl;
    }
    else if ( the_var->isReferenceVariable() && verbose == true  )
    {
        o << "_varType      = reference" << std::endl;
        o << "_refVar       = " << the_var->getName() << " <" << the_var << ">" << std::endl;
    }
    
    if ( the_var->getRevObject().isModelObject() == true )
    {
        the_var->getRevObject().getDagNode()->printStructureInfo( o, verbose );
    }
    
    o << std::endl;

    RBOUT( o.str() );

    // no return value
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_structure::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x"      , RevObject::getClassTypeSpec(), "The variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "verbose", RlBoolean::getClassTypeSpec(), "Do you want all the information?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_structure::getClassType(void)
{
    
    static std::string revType = "Func_type";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_structure::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_structure::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_structure::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

