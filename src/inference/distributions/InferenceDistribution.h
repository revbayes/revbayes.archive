/**
 * @file
 * This file contains the declaration of InferenceDistribution, which specifies the
 * interface for InferenceDistributions in RevBayes. A InferenceDistribution is taken in
 * the statistical sense.
 *
 * @brief Declaration of InferenceDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 * @interface InferenceDistribution
 *
 * $Id: InferenceDistribution.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef InferenceDistribution_H
#define InferenceDistribution_H

#include "RbValue.h"
#include <set>
#include <string>
#include <vector>

class RandomNumberGenerator;


class InferenceDistribution {
    
public:
    virtual                            ~InferenceDistribution(void) {}                              //!< Destructor
    
    
    // InferenceDistribution functions you have to override
    virtual InferenceDistribution*      clone(void) const = 0;                                      //!< Create a clone of the function
    virtual void                        setParameters(const std::vector<RbValue<void*> > &p) = 0;   //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    virtual double                      lnPdf(void) const = 0;                                      //!< Ln probability density
    virtual double                      pdf(void) const = 0;                                        //!< Probability density function
    virtual void                        rv(void) = 0;                                               //!< Generate a random draw
    
protected:
    InferenceDistribution( void ) {};                                                               //!< Simple constructor
    
    
private:
    
};

#endif

