#include "ExponentialDemographicFunction.h"

#include <cmath>

using namespace RevBayesCore;


ExponentialDemographicFunction::ExponentialDemographicFunction(const TypedDagNode<double>* N0, const TypedDagNode<double>* N1, const TypedDagNode<double>* t0, const TypedDagNode<double>* t1) : DemographicFunction(),
    theta_ancient( N1 ),
    theta_recent( N0 ),
    time_ancient( t1 ),
    time_recent( t0 )
{
    
}


ExponentialDemographicFunction::ExponentialDemographicFunction(const ExponentialDemographicFunction &f) : DemographicFunction(f),
    theta_ancient( f.theta_ancient ),
    theta_recent( f.theta_recent ),
    time_ancient( f.time_ancient ),
    time_recent( f.time_recent )
{
    
}


ExponentialDemographicFunction::~ExponentialDemographicFunction( void )
{
    
}


ExponentialDemographicFunction& ExponentialDemographicFunction::operator=(const ExponentialDemographicFunction &f)
{
    DemographicFunction::operator=( f );
    
    if ( this != &f )
    {
        theta_ancient   = f.theta_ancient;
        theta_recent    = f.theta_recent;
        time_ancient    = f.time_ancient;
        time_recent     = f.time_recent;
    }
    
    return *this;
}


ExponentialDemographicFunction* ExponentialDemographicFunction::clone( void ) const
{
    
    return new ExponentialDemographicFunction(*this);
}


double ExponentialDemographicFunction::getDemographic(double t) const
{
    double N0 = theta_recent->getValue();
    double N1 = theta_ancient->getValue();
    double t0 = time_recent->getValue();
    double t1 = time_ancient->getValue();
    
    if ( t1 < t0 || t0 < 0 || N1 < 0 || N1 < 0 )
    {
        throw RbException("Impossible parameter values in exponential growth/decline demographic functions.");
    }
    
    if ( N0 == N1 )
    {
        return N0;
    }
    else
    {
        double alpha = log( N1/N0 ) / (t0 - t1);
        return N0 * exp( (t0-t) * alpha);
    }
    
}


double ExponentialDemographicFunction::getIntegral(double start, double finish) const
{
    double N0 = theta_recent->getValue();
    double N1 = theta_ancient->getValue();
    double t0 = time_recent->getValue();
    double t1 = time_ancient->getValue();
    
    if ( t1 < t0 || t0 < 0 || N1 < 0 || N1 < 0 )
    {
        throw RbException("Impossible parameter values in exponential growth/decline demographic functions.");
    }
    
    
    if ( N0 == N1 )
    {
        double delta = finish - start;
        return delta / N0;
    }
    else
    {
        double alpha = log( N1/N0 ) / (t0 - t1);
        return (exp( (finish-t0)*alpha ) - exp((start-t0)*alpha)) / N0 / alpha;
    }
    
}




std::ostream& operator<<(std::ostream& o, const ExponentialDemographicFunction& x)
{
    return o;
}
