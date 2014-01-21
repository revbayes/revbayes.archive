#include "DistributionExponential.h"
#include "DiversityDependentPureBirthProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"


using namespace RevBayesCore;


/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    o      Time of the origin/present/length of the process.
 * \param[in]    s      Initial speciation rate (lambda_0).
 * \param[in]    k      Carrying capacity.
 * \param[in]    r      Sampling probability (rho).
 * \param[in]    ss     Sampling strategy (uniform/diversified).
 * \param[in]    cdt    Condition of the process (none/survival/#Taxa).
 * \param[in]    nTaxa  Number of observed taxa.
 * \param[in]    tn     Taxon names.
 * \param[in]    c      Clades conditioned to be present.
 */
DiversityDependentPureBirthProcess::DiversityDependentPureBirthProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *s, const TypedDagNode<int> *k,
                                                             const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, unsigned int nTaxa, 
                                                             const std::vector<std::string> &tn, const std::vector<Clade> &c) : BirthDeathProcess( o, r, ss, cdt, nTaxa, tn, c ), 
        initialSpeciation( s ), 
        capacity( k ) 
{
    
    addParameter( initialSpeciation );
    addParameter( capacity );
    
    simulateTree();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
DiversityDependentPureBirthProcess* DiversityDependentPureBirthProcess::clone( void ) const 
{
    
    return new DiversityDependentPureBirthProcess( *this );
}


/**
 * Compute the log-transformed speciation rate at time t.
 * The speciation rate is diversity-dependent and computed by the function:
 * lambda = (1.0 - n/K)*lambda_0
 *
 * \param[in]    t      Time.
 *
 * \return Speciation rate at time t. 
 */
double DiversityDependentPureBirthProcess::lnSpeciationRate(double t) const 
{
    
    int n = diversity( t );
    double b = initialSpeciation->getValue();
    int k = capacity->getValue();
    
    return (1.0 - double(n)/k) * b ;
}

/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 * At the present time each species has the survival probability r.
 * The general equation for the homogeneous birth-death process is
 *    P(N(T)>0|N(t)=1) = (1+\int{mu*exp[rateIntegral(t,s)]ds} - (r-1)/r exp[rateIntegral(t,T)])^{-1}
 * Because this process has no extinction, the survival probability is simplified to
 *    P(N(T)>0|N(t)=1) = (1 - (r-1)/r exp[rateIntegral(t,T)])^{-1}
 * 
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 * \param[in]    r          Sampling probability.
 *
 * \return Speciation rate at time t. 
 */
double DiversityDependentPureBirthProcess::pSurvival(double start, double end, double r) const 
{
    
    // compute the rate
    double rate = rateIntegral(start, end);    
    double den = 1.0 - ( (r-1.0)/r ) * exp(rate);
    
    
    return (1.0 / den);
}


/**
 * Compute the speciation rate integral.
 * The integral is defined as
 *    rate = \int( mu(s) - lambda(s) ds )
 * Because this process is a pure-birth process with diversity-dependent speciation rate,
 * the integral simplifies to
 *    rate = \int( -lambda(s) ds )
 *
 * \param[in]    t_low       Lower boundary of the interval.
 * \param[in]    t_high      Upper boundary of the interval.
 *
 * \return Diversification rate integrated from t_low to t_high.
 **/
double DiversityDependentPureBirthProcess::rateIntegral(double t_low, double t_high) const 
{
    
    // get the divergence times (speciation events)
    std::vector<double> times = divergenceTimes();
    
    // get the current parameter values
    double lambda = initialSpeciation->getValue();
    double k = capacity->getValue();
    
    // compute the rate integral by piecewise computation of the linear parts of the speciation rate
    double rate = 0.0;
    for (size_t i = 1; i < numTaxa; ++i) 
    {
        // get the interval boundaries:
        // either these are the speciation events t[i-1] and t[i]
        // or adjusted by the actual time we want to compute
        double min = (times[i-1] < t_low)  ? t_low  : times[i-1];
        double max = (times[i]   > t_high) ? t_high : times[i];
        double diff = (min > max) ? 0.0 : (max-min);
        
        // compute the speciation rate for this interval
        double b = (1.0 - i/k) * lambda;
        
        // add the integral for this interval
        rate -= b*diff;
    }
     
    // return the rate integral
    return rate;
}


/**
 * Simulate new speciation times.
 */
std::vector<double> DiversityDependentPureBirthProcess::simSpeciations(size_t n, double origin, double r) const
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
	double u = rng->uniform01();
    
    // get the parameters
    double lambda = initialSpeciation->getValue();
    double k = capacity->getValue();
    
    // \todo
    // draw the final event
    // this is not until actually an event happened but a uniform time before the next species would have been sampled.
    
    double lastEvent = 0.0;
    
    // draw the number of speciation events (sampled + not-sampled)
    size_t m = size_t( ceil( n/r ) );
    // 
    std::vector<double> times = std::vector<double>(m,0.0);
    for (size_t i = 0; i < m; i++ )
    {
        double rate = ( 1.0 - ((m-i+2)/k) ) * lambda;
        double t = lastEvent + RbStatistics::Exponential::rv(rate, *rng);
        lastEvent = t;
        times[m-i] = t;
    }
	
    return times;
}



/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void DiversityDependentPureBirthProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == initialSpeciation) 
    {
        initialSpeciation = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == capacity) 
    {
        capacity = static_cast<const TypedDagNode<int>* >( newP );
    }
    else 
    {
        // delegate the super-class
        BirthDeathProcess::swapParameter(oldP, newP);
    }
    
}
