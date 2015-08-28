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


