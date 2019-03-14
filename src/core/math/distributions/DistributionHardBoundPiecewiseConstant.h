/**
 * @file DistributionHardBoundPiecewiseConstant
 * This file contains the functions of a piecewise constant distribution with hard bounds (a histogram).
 *
 * @brief Implementation of the HardBoundPiecewiseConstant distribution.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */


#ifndef DistributionHardBoundPiecewiseConstant_H
#define DistributionHardBoundPiecewiseConstant_H

#include <vector>

namespace RevBayesCore {

    class RandomNumberGenerator;

    namespace RbStatistics {

        namespace HardBoundPiecewiseConstant {

            double                      pdf(std::vector<double>& probs, std::vector<double>& breaks, double x);
            double                      lnPdf(std::vector<double>& probs, std::vector<double>& breaks, double x);
            double                      cdf(std::vector<double>& probs, std::vector<double>& breaks, double x);
            double                      quantile(std::vector<double>& probs, std::vector<double>& breaks, double p);
            double                      rv(std::vector<double>& probs, std::vector<double>& breaks, RandomNumberGenerator& rng);

            int                         findInterval(std::vector<double>& breaks, double x);

        }
    }
}

#endif
