/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for Distributions on continuous variables.
 *
 * @brief Declaration of DistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionContinuous.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef DistributionContinuous_H
#define DistributionContinuous_H

#include "Distribution.h"
#include "Real.h"

#include <set>
#include <string>


class DistributionContinuous: public Distribution {
    
public:
    virtual                                     ~InferenceDistributionContinuous(void) {}                                                                 //!< Destructor
    
    // Basic utility function
    virtual DistributionContinuous*    clone(void) const = 0;                                              //!< Clone object    

    // Interval Distribution functions you probably want to override
    virtual double                              getMax(void) const;                                                 //!< Get max value of coverage
    virtual double                              getMin(void) const;                                                 //!< Get min value of coverage
    
    // Interval Distribution functions you have to override
    virtual double                              cdf(double p) = 0;                                                  //!< Cumulative probability
    virtual double                              quantile(double p) = 0;                                             //!< Quantile
    
protected:
    DistributionContinuous() {}                                //!< Constructor

    // functions you have to override
    virtual double                              lnPdfSingleValue(std::vector<size_t> &result_offsets) const = 0;    //!< Ln probability density
    virtual double                              pdfSingleValue(std::vector<size_t> &result_offsets) const = 0;      //!< Probability density function
    virtual void                                rvSingleValue(std::vector<size_t> &offsets) = 0;                    //!< Generate a random draw
    virtual void                                setInternalParameters(const std::vector<RbValue<void*> > &p) = 0;   //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    virtual void                                setInternalObservedValue(const RbValue<void*> &v);                  //!< Set the pointers to the observation (random variable) of the distribution.
    
    // return values
    RbValue<double*>                            randomVariable;
};

#endif

