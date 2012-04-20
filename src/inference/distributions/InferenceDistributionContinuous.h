/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for InferenceDistributions on continuous variables.
 *
 * @brief Declaration of InferenceDistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: InferenceDistributionContinuous.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef InferenceDistributionContinuous_H
#define InferenceDistributionContinuous_H

#include "InferenceDistribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class RandomNumberGenerator;

class InferenceDistributionContinuous: public InferenceDistribution {
    
public:
    virtual                                     ~InferenceDistributionContinuous(void) {}                                                                 //!< Destructor
    
    // Basic utility function
    virtual InferenceDistributionContinuous*    clone(void) const = 0;                                              //!< Clone object    

    // Interval InferenceDistribution functions you probably want to override
    virtual double                              getMax(void) const;                                                 //!< Get max value of coverage
    virtual double                              getMin(void) const;                                                 //!< Get min value of coverage
    
    // Interval InferenceDistribution functions you have to override
    virtual double                              cdf(double p) = 0;                                                  //!< Cumulative probability
    virtual double                              quantile(double p) = 0;                                             //!< Quantile
    virtual void                                rv(void) = 0;                                                       //!< Generate a random draw
    
protected:
    InferenceDistributionContinuous() {}                                //!< Constructor

    // functions you have to override
    virtual double                              lnPdfSingleValue(std::vector<size_t> &result_offsets) const = 0;    //!< Ln probability density
    virtual double                              pdfSingleValue(std::vector<size_t> &result_offsets) const = 0;      //!< Probability density function
    virtual void                                setInternalParameters(const std::vector<RbValue<void*> > &p) = 0;   //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    
};

#endif

