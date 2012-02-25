/**
 * @file
 * This file contains the declaration of Dist_lnorm, which is used to hold
 * parameters and functions related to a lognormal distribution.
 *
 * @brief Declaration of Dist_lnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-17, version 1.0
 *
 * $Id$
 */

#ifndef Dist_lnorm_H
#define Dist_lnorm_H

#include "DistributionContinuous.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class VectorString;

class Dist_lnorm: public DistributionContinuous {
	
public:
								Dist_lnorm(void);                                                   //!< Parser constructor
	
	// Basic utility functions
	Dist_lnorm*                 clone(void) const;                                                  //!< Clone object
    static const std::string&   getClassName(void);                                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
	
	// Member variable setup
    const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
    void                        setMemberVariable(const std::string& name, Variable* var);          //!< Catching the setting of the member variables.
	
	// Lognormal distribution functions
	double                      cdf(const RbLanguageObject& value);                                 //!< Cumulative density
	const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (RealPos)
	double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
	double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
	const Real&                 quantile(const double p);                                           //!< Quantile
	const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
private:

    // parameters
    RbVariablePtr               mu;
    RbVariablePtr               sigma;
    
    // memberfunction return variables
    RealPos                     randomVariable;
    
};

#endif

