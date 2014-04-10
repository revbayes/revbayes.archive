//
//  AdmixtureCPPRateAdmixtureCPPRateScaleMove.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/30/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureCPPRateScaleMove.h"

#include "AdmixtureCPPRateScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/* Constructor */
AdmixtureCPPRateScaleMove::AdmixtureCPPRateScaleMove( StochasticNode<AdmixtureTree>* n, StochasticNode<double> *r, double hp, double l, bool t, double w ) : SimpleMove( r, w, t ), lambda( l ), cpp_prior(hp), variable( n ), rate( r ), storedRate( 0.0 ) {
    // we need to allocate memory for the stored value
    
    nodes.insert(variable);
    nodes.insert(rate);
}

/*
AdmixtureCPPRateScaleMove::AdmixtureCPPRateScaleMove( const AdmixtureCPPRateScaleMove& n ) : SimpleMove( n.variable, n.weight, n.autoTuning ), variable( n.variable ), tau( n.tau ), storedValue( n.storedValue ), lambda( n.lambda ) {
    // we need to allocate memory for the stored value
    
}
 */


/* Clone object */
AdmixtureCPPRateScaleMove* AdmixtureCPPRateScaleMove::clone( void ) const {
    
    return new AdmixtureCPPRateScaleMove( *this );
}



const std::string& AdmixtureCPPRateScaleMove::getMoveName( void ) const {
    static std::string name = "CPP_rate";
    
    return name;
}


/** Perform the move */
double AdmixtureCPPRateScaleMove::performSimpleMove( void ) {
    
    //std::cout << "\n Admix CPP Rate Scale\n";
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    //double &val = variable->getValue();
    double &r = rate->getValue();
    rate->touch();
    
    // copy value
    storedRate = r;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    r *= scalingFactor;

    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    double lnPrior = -cpp_prior * (r - r/scalingFactor); // manual prior, ask Sebastian...
    
    // and the CPP probs
    AdmixtureTree& tau = variable->getValue();
    size_t numEvents = tau.getNumberOfAdmixtureChildren();
    double unitTreeLength = tau.getUnitTreeLength();
    double newR = r * unitTreeLength;
    double oldR = storedRate * unitTreeLength;
    
    //double lnP = (-newR + numEvents * log(newR)) - (-oldR + numEvents * log(oldR));
    double lnP = log( (exp(-newR) * pow(newR,numEvents)) / (exp(-oldR) * pow(oldR,numEvents)) );
    
    //std::cout << "cpp_rate\t" << lnP+lnHastingsratio << "\t=\t" << lnP << "\t+\t" << lnHastingsratio << ";\t" << oldR << " -> " << newR << ";\t" << storedRate << " -> " << r << ";\t" << unitTreeLength << "\t" << numEvents << "\n";
    
    return lnHastingsratio + lnP + lnPrior;
}

void AdmixtureCPPRateScaleMove::acceptSimpleMove(void)
{
    ;//std::cout << "accept CPP_rate\t" << storedRate << "->" << rate->getValue() << "\n";
}

void AdmixtureCPPRateScaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void AdmixtureCPPRateScaleMove::rejectSimpleMove( void ) {
    //std::cout << "reject CPP_rate\t" << storedRate << "!->" << rate->getValue() << "\n";
    // swap current value and stored value
    //rate->setValue( new double(storedRate) );
    double& r = rate->getValue();
    r = storedRate;
}


void AdmixtureCPPRateScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "CPP\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == rate)
    {
        //std::cout << "CPP\trate\n";
        rate = static_cast<StochasticNode<double>* >(newN);
    }
}


void AdmixtureCPPRateScaleMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
