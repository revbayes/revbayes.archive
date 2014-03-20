#include "Real.h"
#include "RlString.h"
#include "RlUtils.h"

using namespace RevLanguage;


RbLanguageObject* RlUtils::RlTypeConverter::toReal(double x) 
{
    return new Real( x );
}

RbLanguageObject* RlUtils::RlTypeConverter::toString(const std::string &x) 
{
    return new RlString( x );
}
