#include "SteppingStoneSampler.h"

#include <cmath>

using namespace RevBayesCore;


SteppingStoneSampler::SteppingStoneSampler(const std::string &fn, const std::string &pn, const std::string &ln,  const std::string &del) : MarginalLikelihoodEstimator(fn, pn, ln, del)
{
    
}



SteppingStoneSampler::~SteppingStoneSampler()
{
    
}



SteppingStoneSampler* SteppingStoneSampler::clone( void ) const
{
    return new SteppingStoneSampler(*this);
}


double SteppingStoneSampler::marginalLikelihood( void ) const
{
    
    std::vector<double> pathValues;
    double marginal = 0.0;
    for (size_t i = 1; i < powers.size(); ++i)
    {
        
        size_t samplesPerPath = likelihoodSamples[i].size();
        double max = likelihoodSamples[i][0];
        for (size_t j = 1; j < samplesPerPath; ++j)
        {
            if (max < likelihoodSamples[i][j])
            {
                max = likelihoodSamples[i][j];
            }
        }
        
        // mean( exp(samples-max)^(beta[k-1]-beta[k]) )     or
        // mean( exp( (samples-max)*(beta[k-1]-beta[k]) ) )
        double mean = 0.0;
        for (size_t j = 0; j < samplesPerPath; ++j)
        {
            mean += exp( (likelihoodSamples[i][j]-max)*(powers[i-1]-powers[i]) ) / samplesPerPath;
        }
        
        marginal += log(mean) + (powers[i-1]-powers[i])*max;
        
    }
    
    return marginal;
}

