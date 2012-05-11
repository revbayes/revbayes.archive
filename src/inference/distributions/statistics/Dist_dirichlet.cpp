///**
// * @file
// * This file contains the implementation of Dist_dirichlet, which is used to hold
// * parameters and functions related to a Dirichlet distribution.
// *
// * @brief Implementation of Dist_dirichlet
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date$
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-08-27, version 1.0
// *
// * $Id$
// */
//
//#include "Dist_dirichlet.h"
//#include "RandomNumberGenerator.h"
//#include "RandomNumberFactory.h"
//#include "RbException.h"
//#include "RbUtil.h"
//#include "DistributionDirichlet.h"
//
//#include <cmath>
//#include <sstream>
//#include <vector>
//
//
///** Default constructor for parser use */
//Dist_dirichlet::Dist_dirichlet( void ) : Distribution(  ) {
//
//}
//
//
///** Clone this object */
//Dist_dirichlet* Dist_dirichlet::clone( void ) const {
//
//    return new Dist_dirichlet( *this );
//}
//
//
///**
// * This function calculates the natural log of the probability
// * density for a Dirichlet-distributed random variable.
// *
// * @brief Natural log of Dirichlet probability density
// *
// * @param value Observed value
// * @return      Natural log of the probability density
// */
//double Dist_dirichlet::lnPdfSingleValue( std::vector<size_t> &offset ) const {
//    
//    return RbStatistics::Dirichlet::lnPdf(alpha.value[offset[0]], randomVariable.value[offset[1]]);
//}
//
///**
// * This function calculates the probability density
// * for a Dirichlet-distributed random variable.
// *
// * @brief Dirichlet probability density
// *
// * @param value Observed value
// * @return      Probability density
// */
//double Dist_dirichlet::pdfSingleValue( std::vector<size_t> &offset ) const {
//    
//    return RbStatistics::Dirichlet::pdf(alpha.value[offset[0]], randomVariable.value[offset[1]]);
//}
//
//
///**
// * This function generates a Dirichlet-distributed
// * random variable.
// *
// * @brief Random draw from Dirichlet distribution
// *
// * @return      Random draw from Dirichlet distribution
// */
//void Dist_dirichlet::rvSingleValue( std::vector<size_t> &offset ) {
//    
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//    randomVariable.value[offset[1]] = RbStatistics::Dirichlet::rv(alpha.value[offset[0]], *rng);
//    
//}
//
//
///** We catch here the setting of the member variables to store our parameters. */
//void Dist_dirichlet::setInternalParameters(const std::vector<RbValue<void *> > &p) {
//    
//    alpha.value             = static_cast<std::vector<double> *>( p[0].value );
//    alpha.lengths           = p[0].lengths;
//    
//    randomVariable.value    = static_cast<std::vector<double> *>( p[1].value );
//    randomVariable.lengths  = p[1].lengths;
//    
//}
//
//
