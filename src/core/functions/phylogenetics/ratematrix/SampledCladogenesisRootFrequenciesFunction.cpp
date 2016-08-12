//
//  SampledCladogenesisRootFrequenciesFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 8/11/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "AbstractCladogenicStateFunction.h"
#include "AbstractCharacterHistoryBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "SampledCladogenesisRootFrequenciesFunction.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"
#include <cmath>

using namespace RevBayesCore;

SampledCladogenesisRootFrequenciesFunction::SampledCladogenesisRootFrequenciesFunction(const TypedDagNode<RateGenerator> *rm,
                                                                   const TypedDagNode<MatrixReal> *cp,
                                                                   const TypedDagNode<Tree> *t,
                                                                   const TypedDagNode<double> *r)
: TypedFunction<RbVector<double> >(new RbVector<double>(rm->getValue().getNumberOfStates(), (1.0/rm->getValue().getNumberOfStates()))),
    rateMatrix(rm),
    cladogenesisProbabilities(cp),
    tree(t),
    clock(r),
    numStates(rm->getValue().getNumberOfStates()),
    expectedLengthForStationarity(30.0), // MJL: This number might be learned heuristically
    absorptionTolerance(1.0 - 1e-12),
    stationarityTolerance(1e-6)
{
    
    // add the rate and frequency parameters as parents
    addParameter( rateMatrix );
    addParameter( cladogenesisProbabilities );
    addParameter( tree );
    addParameter( clock );
    
    update();
}


SampledCladogenesisRootFrequenciesFunction::~SampledCladogenesisRootFrequenciesFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SampledCladogenesisRootFrequenciesFunction* SampledCladogenesisRootFrequenciesFunction::clone( void ) const {
    return new SampledCladogenesisRootFrequenciesFunction( *this );
}


void SampledCladogenesisRootFrequenciesFunction::update( void ) {
    
    // get anagenesis clock
    double rate = clock->getValue();
    
    // get anagenetic rate matrix
    const RateGenerator& rm = rateMatrix->getValue();
    
    // get cladogenetic probabilities
    cladogenesisProbabilities->getValue();
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesisProbabilities );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMap();

    // get tree
    const AbstractCharacterHistoryBirthDeathProcess* dist = dynamic_cast<const AbstractCharacterHistoryBirthDeathProcess* >( &tree->getDistribution() );
    const TopologyNode& root = tree->getValue().getRoot();
    
    // get root history information
    CharacterHistory tree_history = dist->getCharacterHistory();
    BranchHistory branch_history = tree_history[root.getIndex()];
    const std::multiset<CharacterEvent*,CharacterEventCompare>& events = branch_history.getHistory();

    // compute root freqs
    TransitionProbabilityMatrix tp(numStates);
    TransitionProbabilityMatrix tp_next(numStates);
    
    // this many anagenetic events are expected to occur BEFORE cladogenetic events are applied
    rm.calculateTransitionProbabilities( expectedLengthForStationarity, 0, 1.0, tp );
    
    
//    std::cout << tp << "\n";
    // tune this amount if needed
    bool tune = true;
    size_t tuningAttempts = 0;
    while (tune) {
        
        tune = false;
        
        // if the expected length is too long, we hit an absorbing state
        if (tp[1][0] > absorptionTolerance) {
            expectedLengthForStationarity *= 0.66;
//            std::cout << "Absorption error term: " << tp[1][1] - tp[numStates-1][1] << "\n";
//            std::cout << "expectedLengthForStationarity: " << expectedLengthForStationarity << "\n";
            rm.calculateTransitionProbabilities( expectedLengthForStationarity, 0, 1.0, tp );
            tune = true;
        }
        
        // if the expected length is too short, then we have not reached stationarity
        else if ( fabs(tp[1][1] - tp[numStates-1][1]) > stationarityTolerance ) {
            expectedLengthForStationarity *= 1.50;
//            std::cout << "Stationarity error term: " << tp[1][1] - tp[numStates-1][1] << "\n";
//            std::cout << "expectedLengthForStationarity: " << expectedLengthForStationarity << "\n";
            rm.calculateTransitionProbabilities( expectedLengthForStationarity, 0, 1.0, tp );
            tune = true;
        }
        
        if (tuningAttempts > 100)
             RbException("SampledCladogenesisRootFrequenciesFunction could not satisfy absorption/stationarity tolerances");
        
        tuningAttempts++;
    }
    
    // for each interval between events, go from present to past
    double brlen = root.getAge();
    double startAge = root.getAge() + brlen;
    double endAge = root.getAge();
    
    double t = 0.0;
    double dt = 0.0;
    double event_age = 0.0;
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it;
    for (it = events.begin(); it != events.end(); it++)
    {
        
        dt = (*it)->getTime() - t;
        event_age = startAge - t;
        
        // anagenetic changes occurring between (event_age, event_age-dt)
        
        rm.calculateTransitionProbabilities(event_age, event_age-dt, rate, tp_next );
        tp *= tp_next;
        
        // first compute clado probs at younger end of branch
        TransitionProbabilityMatrix cp(numStates);
        std::map<std::vector<unsigned>, double>::iterator it;
        for ( it = eventMapProbs.begin(); it != eventMapProbs.end(); ++it)
        {
            //                    std::cout << "\n";
            // sparse elements from map
            const std::vector<unsigned>& idx = it->first;
            const size_t j = idx[0];
            const size_t k = idx[1];
            const size_t l = idx[2];
            double p_clado = 0.5 * it->second;
            
            cp[j][k] += p_clado;
            cp[j][l] += p_clado;
            
            //                    for (size_t i = 0; i < this->numChars; ++i)
            //                    {
            //                        double p_ikl = tp[i][j] * p_clado;
            //
            //                        std::cout << i << " -> " << j << " -> " << k << "," << l << "\n";
            //                        std::cout << "A[" << i << "][" << j << "]=" << tp[i][j] << "\n";
            //                        std::cout << "C[" << j << "][" << k << "]=" << p_clado << "\n";
            //
            ////                        std::cout << p_ik << " " << p_il << "\n";
            ////                        cp[i][k] += p_ikl;
            ////                        cp[i][l] += p_ikl;
            ////                        cp[i][l] += p_il;
            //                    }
        }
        //                std::cout << "CLADOGENETIC CHANGE\n" << event_age << " " << event_age - dt << "\n" << cp << "\n\n";
        //                std::cout << cp << "\n";
        //                std::cout << "---\n\n";
        
        //                std::cout << num_event++ << "\n";
        //                std::cout << tp << "\n";
        //                std::cout << dt << " " << t+dt << " " << brlen << "\n\n";
        t += dt;
        tp *= cp;
    }
    
    // last interval
    dt = brlen - t;
   
    rm.calculateTransitionProbabilities( endAge+dt, endAge, rate, tp_next );
    tp *= tp_next;

    // update value
    double sum = 0.0;
    for (size_t i = 0; i < value->size(); i++)
    {
        (*value)[i] = tp[1][i];
        sum += tp[1][i];
    }
    
    if ( fabs(1.0 - sum) > 1e-4 )
    {
        RbException("SampledCladogenesisRootFrequenciesFunction simplex does not sum to one");
    }
    
    for (size_t i = 0; i < value->size(); i++)
    {
        (*value)[i] = (*value)[i] / sum;
    }
    

}



void SampledCladogenesisRootFrequenciesFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == rateMatrix) {
        rateMatrix = static_cast<const TypedDagNode< RateGenerator >* >( newP );
    }
    else if (oldP == cladogenesisProbabilities) {
        cladogenesisProbabilities = static_cast<const TypedDagNode< MatrixReal >* >( newP );
    }
    else if (oldP == tree) {
        tree = static_cast<const TypedDagNode<Tree>* >( newP );
    }
    else if (oldP == clock) {
        clock = static_cast<const TypedDagNode<double>* >( newP );
    }
}
