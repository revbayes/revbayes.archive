/**
 * @file
 * This file contains the declaration of Dist_dirichlet, which is used to hold
 * parameters and functions related to a Dirichlet distribution.
 *
 * @brief Declaration of Dist_dirichlet
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_dirichlet_H
#define Dist_dirichlet_H

#include "DistributionContinuous.h"
#include "Simplex.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class Simplex;


class Dist_dirichlet: public DistributionContinuous {

    
public:
    Dist_dirichlet(void);                                                                           //!< constructor
    
    // Basic utility functions
    Dist_dirichlet*             clone(void) const;                                                  //!< Clone object
    
private:
    double                      lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
    double                      pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
    void                        rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
    void                        setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    
    // parameters
    RbValue<double*>            alpha;

};

#endif

