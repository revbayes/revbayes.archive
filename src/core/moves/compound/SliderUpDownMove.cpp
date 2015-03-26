//
//  SliderUpDownMove.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/1/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "SliderUpDownMove.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

SliderUpDownMove::SliderUpDownMove( std::vector<DagNode*> n, double sf, bool t, double w) : CompoundMove( n, w, t), slideFactor( sf ) {
    
    sliderVal1 = static_cast<ContinuousStochasticNode*>( n[0] );
	sliderVal2 = static_cast<ContinuousStochasticNode*>( n[1] ) ;
    
}



/* Clone object */
SliderUpDownMove* SliderUpDownMove::clone( void ) const {
    
    return new SliderUpDownMove( *this );
}



const std::string& SliderUpDownMove::getMoveName( void ) const {
    static std::string name = "SliderUpDownMove";
    
    return name;
}


/** Perform the move */
double SliderUpDownMove::performCompoundMove( void ) {
    
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
    
    double u      = rng->uniform01();
    double delta  = ( slideFactor * ( u - 0.5 ) );
    
    if ( fabs(delta) > 2.0*size )
    {
        delta -= floor(delta / (2.0*size)) * (2.0*size);
    }
    double nv1 = sv1 + delta;
    double nv2 = sv2 - delta;
    
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
	
	return 0.0;
}


void SliderUpDownMove::printParameterSummary(std::ostream &o) const {
    o << "slideFactor = " << slideFactor;
}


void SliderUpDownMove::rejectCompoundMove( void ) {
	
    // undo the proposal
    sliderVal1->setValue( new double(storedSV1) );
    sliderVal2->setValue( new double(storedSV2) );
	
}


void SliderUpDownMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
	if(oldN == sliderVal1)
		sliderVal1 = static_cast<ContinuousStochasticNode* >( newN ) ;
    if(oldN == sliderVal2)
		sliderVal2 = static_cast<ContinuousStochasticNode* >( newN ) ;
}


void SliderUpDownMove::tune( void ){
    
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


