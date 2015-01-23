#include "AbstractOldMove.h"
#include "DagNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathLogic.h"

#include <cmath>
#include <stdio.h>
#include <iostream>

using namespace RevBayesCore;


AbstractOldMove::AbstractOldMove()
{
    
    
}


AbstractOldMove::~AbstractOldMove()
{
    
}


void AbstractOldMove::perform( double lHeat, double pHeat )
{
    
    if ( isGibbs() )
    {
        if ( lHeat < 1.0 || pHeat < 1.0 )
        {
            throw RbException("Cannot perform Gibbs move because the likelihood is perturbed for this Monte Carlo sampler.");
        }
        // do Gibbs proposal
        performGibbs();
        // theMove->accept(); // Not necessary, because Gibbs samplers are automatically accepted.
    }
    else
    {
        // do a Metropolois-Hastings proposal
        
        // Propose a new value
        double lnProbabilityRatio = 0;
        double lnHastingsRatio = performOld(lnProbabilityRatio);
        
        double lnPriorRatio = 0.0;
        double lnLikelihoodRatio = lnProbabilityRatio;

        lnLikelihoodRatio = 0.0;
        const std::set<DagNode*> &nodes = getDagNodes();
        std::set<DagNode*> affectedNodes;
        for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            (*it)->getAffectedNodes( affectedNodes );
        }
        // compute the probability of the current value for each node
        for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            lnPriorRatio += (*it)->getLnProbabilityRatio();
        }
            
        // then we recompute the probability for all the affected nodes
        for (std::set<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it)
        {
            if ( (*it)->isClamped() )
            {
                    
                lnLikelihoodRatio += (*it)->getLnProbabilityRatio();
                    
            }
            else
            {
                    
                lnPriorRatio += (*it)->getLnProbabilityRatio();
                
            }
        }
        
        
        // Calculate acceptance ratio
        double lnR = pHeat * (lHeat * lnLikelihoodRatio + lnPriorRatio) + lnHastingsRatio;
	
		if ( !RbMath::isAComputableNumber(lnR) )
        {
		
            reject();
			
		}
        else
        {
            if (lnR >= 0.0)
            {
#ifdef DEBUG_MCMC_DETAILS
                std::cerr << "Accepting move" << std::endl;
#endif            

                accept();
            }
            else if (lnR < -300.0)
            {
#ifdef DEBUG_MCMC_DETAILS
                std::cerr << "Rejecting move" << std::endl;
#endif

                reject();
            }
            else
            {
                double r = exp(lnR);
                // Accept or reject the move
                double u = GLOBAL_RNG->uniform01();
                if (u < r)
                {
#ifdef DEBUG_MCMC_DETAILS
                    std::cerr << "Accepting move" << std::endl;
#endif
                
                    accept();
                }
                else
                {
#ifdef DEBUG_MCMC_DETAILS
                    std::cerr << "Rejecting move" << std::endl;
#endif
                
                    reject();
                }
            }
        }
    
    }

}
