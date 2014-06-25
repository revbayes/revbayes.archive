//
//  PathSampler.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 6/24/14.
//  Copyright (c) 2014 hoehna. All rights reserved.
//

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
    
    std::vector<double> pathValues;
    for (size_t i = 0; i < powers.size(); ++i)
    {
        
        double mean = 0.0;
        size_t samplesPerPath = likelihoodSamples[i].size();
        for (size_t j = 0; j < samplesPerPath; ++j)
        {
            mean += likelihoodSamples[i][j] / samplesPerPath;
        }
        
        pathValues.push_back( mean );
        
    }
    
    double marginal = 0.0;
    for (size_t i = 0; i < pathValues.size(); ++i)
    {
        marginal += (pathValues[i] + pathValues[i+1])*(powers[i]-powers[i+1])/2.0;
    }
    
    return marginal;
}

