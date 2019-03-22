#include "ConstantDemographicFunction.h"


using namespace RevBayesCore;


ConstantDemographicFunction::ConstantDemographicFunction(const TypedDagNode<double>* t) : DemographicFunction(),
    theta( t )
{
    
}


ConstantDemographicFunction::ConstantDemographicFunction(const ConstantDemographicFunction &f) : DemographicFunction(f),
    theta( f.theta )
{
    
}


ConstantDemographicFunction::~ConstantDemographicFunction( void )
{
    
}


ConstantDemographicFunction& ConstantDemographicFunction::operator=(const ConstantDemographicFunction &f)
{
    DemographicFunction::operator=( f );
    
    if ( this != &f )
    {
        theta = f.theta;
    }
    
    return *this;
}


ConstantDemographicFunction* ConstantDemographicFunction::clone( void ) const
{
    
    return new ConstantDemographicFunction(*this);
}


double ConstantDemographicFunction::getDemographic(double t) const
{
    
    return theta->getValue();
}


double ConstantDemographicFunction::getIntegral(double start, double finish) const
{
    double delta = finish - start;
    return theta->getValue() * delta;
}
