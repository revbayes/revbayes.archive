#include "StoppingRule.h"
#include "RlStoppingRule.h"
#include "TypeSpec.h"


using namespace RevLanguage;

StoppingRule::StoppingRule(void) : WorkspaceToCoreWrapperObject<RevBayesCore::StoppingRule>()
{
    
}


/** Get Rev type of object */
const std::string& StoppingRule::getClassType(void)
{
    
    static std::string revType = "StoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& StoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::StoppingRule>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

