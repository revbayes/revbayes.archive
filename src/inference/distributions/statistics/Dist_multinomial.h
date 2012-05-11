///**
// * @file
// * This file contains the declaration of Dist_multinomial, which is used to hold
// * parameters and functions related to a multinomial distribution.
// *
// * @brief Declaration of Dist_multinomial
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
//#ifndef Dist_multinomial_H
//#define Dist_multinomial_H
//
//#include "DistributionDiscrete.h"
//#include "RbVector.h"
//
//#include <ostream>
//#include <string>
//
//
//class Dist_multinomial: public DistributionDiscrete {
//
//    
//public:
//    Dist_multinomial(void);                                                                         //!< constructor
//    
//    // Basic utility functions
//    Dist_multinomial*           clone(void) const;                                                  //!< Clone object
//                    
//private:
//    double                      lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
//    double                      pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
//    void                        rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
//    void                        setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
//    
//    // parameters
//    RbValue<double*>            probabilities;
//
//};
//
//#endif
//
