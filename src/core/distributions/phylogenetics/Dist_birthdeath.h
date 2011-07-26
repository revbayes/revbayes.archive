/**
 * @file
 * This file contains the declaration of Dist_birthdeath, which holds
 * the parameters and functions related to a birthdeath distribution.
 *
 * @brief Declaration of Dist_birthdeath
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-07-26, version 1.0
 *
 * $Id$
 */

#ifndef Dist_birthdeath_H
#define Dist_birthdeath_H

#include "Distribution.h"
#include "Topology.h"
#include "VectorRealPos.h"

#include <ostream>
#include <string>

class VectorString;

class Dist_birthdeath: public Distribution{
    
public:
    Dist_birthdeath(void);                                                                          //!< Parser constructor
    Dist_birthdeath(double lambda, double mu, double rho=1.0);                                //!< Constructor for internal use
    
    // Basic utility functions
    Dist_birthdeath*            clone(void) const;                                                  //!< Clone object
    const VectorString&         getClass(void) const;                                               //!< Get class vector
    
    // Member variable setup
    const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
    
    // Discrete distribution functions
    Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
    const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (Simplex)
    double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
    double                      pdf(const RbObject* value);                                         //!< Probability density
    VectorRealPos*              rv(void);                                                           //!< Generate random variable
    
private:
    
};

#endif

