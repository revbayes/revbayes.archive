//
//  DistributionBivariatePoisson.h
//  revbayes
//
//  Created by Alexander Zarebski on 2018-12-18.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__DistributionBivariatePoisson__
#define __revbayes__DistributionBivariatePoisson__

#include <iostream>

#endif /* defined(__revbayes__DistributionBivariatePoisson__) */


#include <vector>

namespace RevBayesCore {

    class RandomNumberGenerator;


    namespace RbStatistics {

        namespace BivariatePoisson {

          double                      pdf(const std::vector<int>& x, const double& theta0, const double& theta1, const double& theta2, const bool& log);
          std::vector<int>            rv(const double& theta0, const double& theta1, const double& theta2, RandomNumberGenerator& rng);

        }
    }
}
