#include "DagNode.h"
#include "FileMonitor.h"
#include "PowerPosteriorMcmc.h"
#include "MoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"

#include <cmath>
#include <typeinfo>

using namespace RevBayesCore;

PowerPosteriorMcmc::PowerPosteriorMcmc(const Model& m, const std::vector<Move*> &mvs) : MonteCarloSampler( m, mvs, std::vector<Monitor*>() )
{
    
}


PowerPosteriorMcmc::~PowerPosteriorMcmc(void)
{

}



PowerPosteriorMcmc* PowerPosteriorMcmc::clone( void ) const
{

    return new PowerPosteriorMcmc( *this );
}



const std::vector<double>& PowerPosteriorMcmc::getSamples( void ) const {
    return samples;
}


double PowerPosteriorMcmc::pathSampling( void )
{
    
    std::vector<double> pathValues;
    for (size_t i = 0; i < beta.size(); ++i)
    {
        
        double mean = 0.0;
        for (size_t j = 0; j < samplesPerPath; ++j)
        {
            mean += samples[i*samplesPerPath + j] / samplesPerPath;
        }
        
        pathValues.push_back( mean );
        
    }
    
    double marginal = 0.0;
    for (size_t i = 0; i < pathValues.size(); ++i)
    {
        marginal += (pathValues[i] + pathValues[i+1])*(beta[i]-beta[i+1])/2.0;
    }
    
    return marginal;
}


void PowerPosteriorMcmc::run(size_t gen)
{
    
    std::cout << "Running power posterior ..." << std::endl;
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        (*it)->resetCounters();
    }
    
    samples.clear();
    samplesPerPath = gen / sampleFreq;
    size_t printInterval = size_t( round( fmax(1,gen/20.0) ) );
    size_t digits = size_t( ceil( log10( beta.size() ) ) );
    
    /* Run the chain */    
    RandomMoveSchedule schedule = RandomMoveSchedule(moves);
    for (size_t i = 0; i < beta.size(); ++i) {
        double b = beta[i];
        std::cout << "Step ";
        for (size_t d = size_t( ceil( log10( i+1.1 ) ) ); d < digits; d++ )
        {
            std::cout << " ";
        }
        std::cout << (i+1) << " / " << beta.size();
        std::cout << "\t\t";

        for (size_t k=1; k<=gen; k++) {
            
            if ( k % printInterval == 0 )
            {
                std::cout << "**";
                std::cout.flush();
            }
        
            size_t proposals = size_t( round( schedule.getNumberMovesPerIteration() ) );
            for (size_t j=0; j<proposals; j++) {
                /* Get the move */
                Move* theMove = schedule.nextMove(k);
                
                theMove->perform( b, true );
            
            }
        
            // sample the likelihood
            if ( k % sampleFreq == 0 ) {
                // compute the joint likelihood
                double likelihood = 0.0;
                for (std::vector<DagNode*>::const_iterator n = dagNodes.begin(); n != dagNodes.end(); ++n) {
                    if ( (*n)->isClamped() ) {
                        likelihood += (*n)->getLnProbability();
                    }
                }
                
                samples.push_back( likelihood );
            }
        
        }
        
        std::cout << std::endl;
        
    }
    
    
}


void PowerPosteriorMcmc::setBeta(const std::vector<double> &b)
{
    beta = b;
}


void PowerPosteriorMcmc::setSampleFreq(size_t sf)
{
    sampleFreq = sf;
}

double PowerPosteriorMcmc::steppingStoneSampling( void )
{
    
    std::vector<double> pathValues;
    double marginal = 0.0;
    for (size_t i = 1; i < beta.size(); ++i)
    {
        
        double max = samples[i*samplesPerPath];
        for (size_t j = 1; j < samplesPerPath; ++j)
        {
            if (max < samples[i*samplesPerPath + j])
            {
                max = samples[i*samplesPerPath + j];
            }
        }
        
        // mean( exp(samples-max)^(beta[k-1]-beta[k]) )     or
        // mean( exp( (samples-max)*(beta[k-1]-beta[k]) ) )
        double mean = 0.0;
        for (size_t j = 0; j < samplesPerPath; ++j)
        {
            mean += exp( (samples[i*samplesPerPath + j]-max)*(beta[i-1]-beta[i]) ) / samplesPerPath;
        }
        
        marginal += log(mean) + (beta[i-1]-beta[i])*max;
        
    }
        
    return marginal;
}
