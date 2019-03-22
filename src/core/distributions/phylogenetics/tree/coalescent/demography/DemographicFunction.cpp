#include "DemographicFunction.h"

using namespace RevBayesCore;

DemographicFunction::DemographicFunction(void) : Cloneable()
{
    
}


DemographicFunction::DemographicFunction(const DemographicFunction &f)
{
    
}


DemographicFunction::~DemographicFunction(void)
{
    
}


DemographicFunction& DemographicFunction::operator=(const DemographicFunction &f)
{
    return *this;
}


std::ostream& operator<<(std::ostream& o, const DemographicFunction& x)
{
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const DemographicFunction& x)
{
    o << "DemographicFunction";
    
    return o;
}
