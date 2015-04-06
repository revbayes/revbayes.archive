//
//  LevyJumpSumMove.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "LevyJumpSumMove.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

LevyJumpSumMove::LevyJumpSumMove( std::vector<DagNode*> n, double sf, bool t, double w) : CompoundMove( n, w, t), slideFactor( sf ) {
    
    sliderVal1 = static_cast<ContinuousStochasticNode*>( n[0] );
	sliderVal2 = static_cast<ContinuousStochasticNode*>( n[1] ) ;
    
}



/* Clone object */
LevyJumpSumMove* LevyJumpSumMove::clone( void ) const {
    
    return new LevyJumpSumMove( *this );
}



const std::string& LevyJumpSumMove::getMoveName( void ) const {
    static std::string name = "LevyJumpSumMove";
    
    return name;
}


/** Perform the move */
double LevyJumpSumMove::performCompoundMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double sv1 = sliderVal1->getValue();
    double sv2 = sliderVal2->getValue();
    
    storedSV1 = sv1;
    storedSV2 = sv2;
    
    double min1 = sliderVal1->getMin();
    double min2 = sliderVal2->getMin();
    
    double max1 = sliderVal1->getMax();
    double max2 = sliderVal2->getMax();
    
    double size = max1 - min1;
    
    bool jumpToZero = sv2 != 0.0;
    double delta = 0.0;
    
    // if neither variable may take zero values or if the zero switch is not accepted
    if (jumpToZero)
    {
        sliderVal1->setValue( new double(sv1 + sv2) );
        sliderVal2->setValue( new double(0.0) );
    }
    else
    {
        
        delta  = RbStatistics::Normal::rv(0.0, slideFactor, *GLOBAL_RNG);
        
        if ( fabs(delta) > 2.0*size )
        {
            delta -= floor(delta / (2.0*size)) * (2.0*size);
        }
        double nv1 = sv1 - delta;
        double nv2 = sv1 + delta;
        
        /* reflect the new value */
        do {
            if ( nv1 < min1 )
            {
                nv1 = 2.0 * min1 - nv1;
                nv2 = sv2 - (nv1 - sv1);
            }
            else if ( nv1 > max1 )
            {
                nv1 = 2.0 * max1 - nv1;
                nv2 = sv2 - (nv1 - sv1);
            }
            
        } while ( nv1 < min1 || nv1 > max1 || nv2 < min2 || nv2 > max2);
        
        sliderVal1->setValue( new double(nv1) );
        sliderVal2->setValue( new double(nv2) );
    }
    
    // if the forward move jumps to zero, the reverse move jumps away (where we draw a value from the normal)
    int sign = ( jumpToZero ? 1 : -1 );
	return sign * RbStatistics::Normal::lnPdf(0.0, slideFactor, delta);
}


void LevyJumpSumMove::printParameterSummary(std::ostream &o) const {
    o << "slideFactor = " << slideFactor;
}


void LevyJumpSumMove::rejectCompoundMove( void ) {
	
    // undo the proposal
    sliderVal1->setValue( new double(storedSV1) );
    sliderVal2->setValue( new double(storedSV2) );
	
}


void LevyJumpSumMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
	if(oldN == sliderVal1)
		sliderVal1 = static_cast<ContinuousStochasticNode* >( newN ) ;
    if(oldN == sliderVal2)
		sliderVal2 = static_cast<ContinuousStochasticNode* >( newN ) ;
}


void LevyJumpSumMove::tune( void ){
    
    //	if(0){
    //		double rate = numAccepted / double(numTried);
    //		double target = 0.234;
    //		if ( rate > target ){
    //			slideFactor *= (1.0 + ((rate-target)/(1.0 - target) ));
    //		}
    //		else{
    //			slideFactor /= (2.0 - rate/target );
    //		}
    //	}
}
