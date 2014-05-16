//
//  WangLandauMcmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <cmath>
#include "RbConstants.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include "WangLandauMcmc.h"

using namespace RevBayesCore;

WangLandauMcmc::WangLandauMcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, bool ca, double ch, int ci, size_t ns, double se, double sc, double ub, double lb, size_t np) : Mcmc(m,moves,mons,"random",ca,ch,ci), numSteps(ns), stepEpsilon(se), stepConstant(sc), numPartitions(np), upperBoundEnergy(ub), lowerBoundEnergy(lb)
{
    
    // initialize vectors
    initializeSteps();
    initializePartitionLowerBound();
    proportions.resize(numPartitions, 0.0);
    bias.resize(numPartitions, 1.0);
    
}

void WangLandauMcmc::initializeSteps(void)
{
    bool isGeqEpsilon = true;

    // piecewise function for step sizes
    for (size_t n = 0; n < numSteps; n++)
    {
        double v = ( isGeqEpsilon ? pow(stepConstant,-n) : 1.0 / n );
        if (isGeqEpsilon && v < stepEpsilon)
            isGeqEpsilon = false;
        steps.push_back(v);
    }
}

void WangLandauMcmc::initializePartitionLowerBound(void)
{
    
    double db = (upperBoundEnergy - lowerBoundEnergy) / (numPartitions - 1);
    
    double v = upperBoundEnergy;
    for (size_t i = 0; i < (numPartitions - 1); i++)
    {
        v -= db;
        partitionLowerBound.push_back(v);
    }
    partitionLowerBound.push_back(RbConstants::Double::neginf);
}

void WangLandauMcmc::updateProportions(double x, int t)
{
    // which partition is x in?
    partitionIndex = 0;
    for (size_t i = 0; i < numPartitions; i++)
    {
        if (x > partitionLowerBound[i])
        {
            partitionIndex = i;
            break;
        }
    }
    
    // update partitions
    for (size_t i = 0; i < numPartitions; i++)
        proportions[i] = ((t - 1) * proportions[i]) / t;
    
    // further update found partition
    proportions[partitionIndex] += 1.0 / t;
    
    // "flat histogram" resets proportions as needed
    resetProportions();
}

void WangLandauMcmc::resetProportions(void)
{
    double maxValue = 0.0;
    
    for (size_t i = 0; i < numPartitions; i++)
    {
        if (proportions[i] > maxValue)
        {
            maxValue = proportions[i];
        }
    }
    
    double lhs = fabs(maxValue - (1.0/numPartitions));
    double rhs = proportionConstant/numPartitions;
    if (lhs < rhs)
    {
        stepIndex++;
        proportions.resize(numPartitions,0.0);
    }
}

void WangLandauMcmc::updateBias(void)
{
    
    // update bias given current state's partition
    bias[partitionIndex] += steps[stepIndex] * (1.0 - (1.0/numPartitions) );
    
    // renormalize vector
    double sum = 0.0;
    for (size_t i = 0; i < numPartitions; i++)
        sum += bias[i];
    for (size_t i = 0; i < numPartitions; i++)
        bias[i] /= sum;
}


unsigned long WangLandauMcmc::nextCycle(bool advanceCycle) {
    
    size_t proposals = round( schedule->getNumberMovesPerIteration() );
    for (size_t i=0; i<proposals; i++)
    {
        // Get the move
        Move* theMove = schedule->nextMove( generation );
        
        if ( theMove->isGibbs() )
        {
            // do Gibbs proposal
            theMove->performGibbs();
            // theMove->accept(); // Not necessary, because Gibbs samplers are automatically accepted.
        }
        else
        {
            // do a Metropolois-Hastings proposal
            
            // Propose a new value
            double lnProbabilityRatio;
            double lnHastingsRatio = theMove->perform(lnProbabilityRatio);
            
            // Calculate acceptance ratio
            double lnR = chainHeat * (lnProbabilityRatio) + lnHastingsRatio;
            
            //std::cout << "\n**********\n";
            //std::cout << theMove->getMoveName() << "\n";
            //std::cout << lnHastingsRatio << "  " << lnProbabilityRatio << ";  " << lnProbability << " -> " << lnProbability + lnProbabilityRatio << "\n";
            //std::cout << "pre-mcmc     " << lnProbability << "\n";
            
            if (lnR >= 0.0)
            {
                theMove->accept();
                lnProbability += lnProbabilityRatio;
                //  if (lnProbability > 0.0)
                {
                    
                }
            }
            else if (lnR < -300.0)
            {
                theMove->reject();
            }
            else
            {
                double r = exp(lnR);
                // Accept or reject the move
                double u = GLOBAL_RNG->uniform01();
                if (u < r)
                {
                    theMove->accept();
                    lnProbability += lnProbabilityRatio;
                }
                else
                {
                    theMove->reject();
                }
            }
            //std::cout << "post-mcmc    " << lnProbability << "\n";
            //std::cout << "model        " << getModelLnProbability() << "\n";
            //std::cout << "**********\n\n";
        }
        
#ifdef DEBUG_MCMC
        // Assert that the probability calculation shortcuts work
        double curLnProb = 0.0;
        std::vector<double> lnRatio;
        for (std::vector<DagNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
        {
            (*i)->touch();
            double lnProb = (*i)->getLnProbability();
            curLnProb += lnProb;
        }
        if (fabs(lnProbability - curLnProb) > 1E-8)
            throw RbException("Error in ln probability calculation shortcuts");
        else
            lnProbability = curLnProb;              // otherwise rounding errors accumulate
#endif
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if (advanceCycle)
        generation++;
    
    // gen number used for p(MC)^3
    return generation;
}
