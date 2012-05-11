/**
 * @file
 * This file contains the declaration of DistributionDiscrete, which specifies
 * the interface for Distributions on discrete random variables.
 *
 * @brief Declaration of DistributionDiscrete
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionContinuous.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef DistributionDiscrete_H
#define DistributionDiscrete_H

#include "Distribution.h"
#include "Real.h"

#include <set>
#include <string>


class DistributionDiscrete: public Distribution {
    
public:
    virtual                                     ~DistributionDiscrete(void) {}                                                                 //!< Destructor
    
    // Basic utility function
    virtual DistributionDiscrete*               clone(void) const = 0;                                              //!< Clone object    
        
protected:
    DistributionDiscrete() {}                                //!< Constructor
    
    // functions you have to override
    virtual double                              lnPdfSingleValue(std::vector<size_t> &result_offsets) const = 0;    //!< Ln probability density
    virtual double                              pdfSingleValue(std::vector<size_t> &result_offsets) const = 0;      //!< Probability density function
    virtual void                                rvSingleValue(std::vector<size_t> &offsets) = 0;                    //!< Generate a random draw
    virtual void                                setInternalParameters(const std::vector<RbValue<void*> > &p) = 0;   //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    virtual void                                setInternalObservedValue(const RbValue<void*> &v) = 0;              //!< Set the pointers to the observation (random variable) of the distribution.
    
};

#endif

