/**
 * @file
 * This file contains the declaration of Dist_lnorm, which is used to hold
 * parameters and functions related to a lognormal distribution.
 *
 * @brief Declaration of Dist_lnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-17, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z sibonli $
 */

#ifndef Dist_lnorm_H
#define Dist_lnorm_H

#include "DistributionContinuous.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class VectorString;

const std::string Dist_lnorm_name = "lognormal distribution";

class Dist_lnorm: public DistributionContinuous {
	
public:
								Dist_lnorm(void);                                                   //!< Parser constructor
	
	// Basic utility functions
	Dist_lnorm*                 clone(void) const;                                                  //!< Clone object
	const VectorString&         getClass(void) const;                                               //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
	
	// Member variable setup
    RbPtr<const MemberRules>    getMemberRules(void) const;                                         //!< Get member variable rules
	
	// Lognormal distribution functions
	double                      cdf(RbPtr<const RbLanguageObject> value);                           //!< Cumulative density
	const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (RealPos)
	double                      lnPdf(RbPtr<const RbLanguageObject> value) const;                   //!< Ln probability density
	double                      pdf(RbPtr<const RbLanguageObject> value) const;                     //!< Probability density
	RbPtr<Real>                 quantile(const double p);                                           //!< Quantile
	RbPtr<RbLanguageObject>     rv(void);                                                           //!< Generate random variable
    
private:
    static const TypeSpec       typeSpec;
    static const TypeSpec       varTypeSpec;
};

#endif

