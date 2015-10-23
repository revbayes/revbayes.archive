#include "Procedure.h"

using namespace RevLanguage;

Procedure::Procedure() : Function()
{
    
}


Procedure::~Procedure()
{
    
}


/* Get Rev type of object */
const std::string& Procedure::getClassType(void)
{
    
    static std::string revType = "Procedure";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Procedure::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


