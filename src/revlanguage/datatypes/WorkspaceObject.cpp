#include "WorkspaceObject.h"

#include "TypeSpec.h"
#include "RlFunction.h"

using namespace RevLanguage;

WorkspaceObject::WorkspaceObject() : RevObject()
{
    
}



WorkspaceObject::~WorkspaceObject()
{
    
}


const std::string& WorkspaceObject::getClassType(void) {
    
    static std::string revType = "WorkspaceObject";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& WorkspaceObject::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


bool WorkspaceObject::isWorkspaceObject( void ) const
{
    return true;
}



/** Print structure info for user */
void WorkspaceObject::printStructure( std::ostream &o, bool verbose ) const
{
    o << "_RevType      = " << getType() << std::endl;
    o << "_RevTypeSpec  = [ " << getTypeSpec() << " ]" << std::endl;
    o << "_value        = ";
    
    std::ostringstream o1;
    printValue( o1 );
    o << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;
    
    const MethodTable& methods = getMethods();
    for ( MethodTable::const_iterator it = methods.begin(); it != methods.end(); ++it )
    {
        o << "." << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }
    
}


