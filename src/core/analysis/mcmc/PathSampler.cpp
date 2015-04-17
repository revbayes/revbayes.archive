#include "PathSampler.h"

#include <vector>


using namespace RevBayesCore;



PathSampler::PathSampler(const std::string &fn, const std::string &pn, const std::string &ln,  const std::string &del) : MarginalLikelihoodEstimator(fn, pn, ln, del)
{
    
}



PathSampler::~PathSampler()
{
    
}



PathSampler* PathSampler::clone( void ) const
{
    return new PathSampler(*this);
}


double PathSampler::marginalLikelihood( void ) const
{
    // create a vector for the mean log-likelihood values per power posterior
    std::vector<double> pathValues;
    
    // iterate over all powers
    for (size_t i = 0; i < powers.size(); ++i)
    {
        // compute the mean for this power
        double mean = 0.0;
        
        // get the number of samples for this power posterior analysis
        size_t samplesPerPath = likelihoodSamples[i].size();
        for (size_t j = 0; j < samplesPerPath; ++j)
        {
            mean += likelihoodSamples[i][j];
        }
        
        // store the mean
        pathValues.push_back( mean  / samplesPerPath );
        
    }
    
    // now we can compute the marginal likelihood
    // the method uses the trapezoidal rule for numerical integration
    double marginal = 0.0;
    for (size_t i = 0; i < pathValues.size()-1; ++i)
    {
        marginal += (pathValues[i] + pathValues[i+1])*(powers[i]-powers[i+1])/2.0;
    }
    
    return marginal;
}

