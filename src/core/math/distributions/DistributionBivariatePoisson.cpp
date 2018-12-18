/**
 * @file DistributionBivariatePoisson
 * This file contains the functions of the bivariate poisson distribution.
 *
 * @brief Implementation of the bivariate poisson distribution.
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

#include <cmath>
#include <iostream>
#include <vector>

#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathFunctions.h"
#include "DistributionBivariatePoisson.h"
#include "DistributionPoisson.h"
#include "RbStatisticsHelper.h"
#include "RbException.h"


using namespace RevBayesCore;



double RbStatistics::BivariatePoisson::lnPdf(double theta1, double theta2, double theta0, std::vector<long> x) {

  long x1, x2;
  x1 = x[0];
  x2 = x[1];
  double result = - ( theta1 + theta2 + theta0 ) + x1 * std::log( theta1 ) + x2 * std::log( theta2 ) - (RbMath::lnGamma( x1 + 1 ) + RbMath::lnGamma( x2 + 1 ));


  double theta_ratio = theta0 / (theta1 * theta2);
  long min_x1x2 = std::min(x1, x2);
  double tmp = 0;

  for (int i = 0; i <= min_x1x2; i++) {
    tmp += RbMath::choose(x1, i) * RbMath::choose(x2, i) * RbMath::gamma(i + 1) * std::pow(theta_ratio, i);
  }

  return result + std::log(tmp);
}


std::vector<long> RbStatistics::BivariatePoisson::rv(double theta1, double theta2, double theta0, RandomNumberGenerator& rng)
{
  long a, b, c;
  a = RbStatistics::Poisson::rv(theta1, rng);
  b = RbStatistics::Poisson::rv(theta2, rng);
  c = RbStatistics::Poisson::rv(theta0, rng);
  std::vector<long> x;
  x.push_back(a + c);
  x.push_back(b + c);
  return x;
}
