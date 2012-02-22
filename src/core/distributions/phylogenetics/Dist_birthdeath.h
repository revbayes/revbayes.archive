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
#include "RealPos.h"

#include <ostream>
#include <string>

class VectorString;

class Dist_birthdeath: public Distribution {
    
public:
    Dist_birthdeath(void);                                                                          //!< Default constructor
    
    // Basic utility functions
    Dist_birthdeath*            clone(void) const;                                                  //!< Clone object
    static const std::string&   getClassName(void);                                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
    
    // Member variable setup
    const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
    
    // Discrete distribution functions
    const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (Simplex)
    double                      lnPdf( const RbLanguageObject& value) const;                        //!< Ln probability density
    double                      pdf( const RbLanguageObject& value) const;                          //!< Probability density
    const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
private:
    double                      nj(double t, double t_prime, double T, double lambda, double mu, double rho) const;
    double                      pBirth(double t, double T, double lambda, double mu, double rho) const;
    double                      pSurvival(double start, double end, double lambda, double mu, double rho) const;
    double                      pWaiting(double start, double observed, double end, double lambda, double mu, double rho) const;
    double                      rate(double start, double end, double lambda, double mu, double rho) const;
    double                      ut(double t, double t_prime, double lambda, double mu, double rho) const;
        
    // parameters
    RbVariablePtr               origin;
    RbVariablePtr               presentTime;
    RbVariablePtr               lambda;
    RbVariablePtr               mu;
    RbVariablePtr               rho;
    RbVariablePtr               speciationEvent;
    
    // memberfunction return value
    RealPos                     randomVariable;
};

#endif

