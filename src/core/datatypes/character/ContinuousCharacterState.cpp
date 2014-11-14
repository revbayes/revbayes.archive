///**
// * @file
// * This file contains the implementation of ContinuousCharacterState, which is
// * the class for the Amino Acid character data type in RevBayes.
// *
// * @brief Implementation of ContinuousCharacterState
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// *
// * $Id: ContinuousCharacterState.cpp 1568 2012-05-24 07:58:04Z hoehna $
// */
//
//#include "ContinuousCharacterState.h"
//#include "RbConstants.h"
//
//#include <cmath>
//#include <limits>
//#include <sstream>
//
//using namespace RevBayesCore;
//
///** Default constructor */
//ContinuousCharacterState::ContinuousCharacterState(void) : CharacterState(), mean( 0.0 ), variance( 0.0 ) {
//    
//}
//
//
///** Copy constructor */
//ContinuousCharacterState::ContinuousCharacterState(const ContinuousCharacterState& s) : CharacterState(), mean( s.mean ), variance( s.variance ) {
//    
//}
//
//
///** Constructor that sets the observation */
//ContinuousCharacterState::ContinuousCharacterState( double x ) : CharacterState(), mean( x ), variance( 0.0 ) {
//    
//}
//
//
///** Constructor that sets the observation */
//ContinuousCharacterState::ContinuousCharacterState( double x, double v ) : CharacterState(), mean( x ), variance( v ) {
//    
//}
//
//
///** Equals comparison */
//bool ContinuousCharacterState::operator==(const CharacterState& x) const {
//    
//    const ContinuousCharacterState* derivedX = static_cast<const ContinuousCharacterState*>(&x);
//    if ( derivedX != NULL ) {
//        return mean == derivedX->mean && variance == derivedX->variance;
//    }    
//    
//    return false;
//}
//
//
///** Not equals comparison */
//bool ContinuousCharacterState::operator!=(const CharacterState& x) const {
//    
//    return !operator==(x);
//}
//
//
//
//bool ContinuousCharacterState::operator<(const CharacterState &x) const {
//    
//    const ContinuousCharacterState* derivedX = static_cast<const ContinuousCharacterState*>(&x);
//    if ( derivedX != NULL ) {
//        
//        return mean < derivedX->mean;
//    }    
//    
//    return false;
//}
//
//
///* Clone object */
//ContinuousCharacterState* ContinuousCharacterState::clone(void) const {
//    
//	return  new ContinuousCharacterState( *this );
//}
//
//
//std::string ContinuousCharacterState::getDatatype( void ) const {
//    return "Continuous";
//}
//
//
//double ContinuousCharacterState::getMean( void ) const {
//    return mean;
//}
//
//
//size_t ContinuousCharacterState::getNumberOfStates( void ) const {
//    return RbConstants::Size_t::nan;
//}
//
//
//
//std::string ContinuousCharacterState::getStringValue(void) const  {
//    
//    std::stringstream o;
//    
//    if ( fabs(variance - 0.0) < 0.00000001 )
//        o << mean;
//    else
//        o << mean << " (" << variance << ")";
//    
//    return o.str();
//}
//
//
//
//double ContinuousCharacterState::getVariance( void ) const {
//    return variance;
//}
//
//
//bool ContinuousCharacterState::isAmbiguous( void ) const {
//    return false;
//}
//
//
//
//bool ContinuousCharacterState::isGapState( void ) const {
//    // gaps are represented by NaN's
//    return mean != mean; 
//}
//
//
//
//void ContinuousCharacterState::setGapState(bool tf) {
//    if (tf) {
//        mean = std::numeric_limits<double>::quiet_NaN();
//    }
//    else {
//        mean = 0.0;
//    }
//}
//
//
//
//void ContinuousCharacterState::setMean(double x) {
//    
//    mean = x;
//}
//
//
//
//void ContinuousCharacterState::setVariance(double x) {
//    
//    variance = x;
//}
