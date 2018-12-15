//
//  DistributionBivariatePoisson.h
//  revbayes
//
//  Created by Alexander Zarebski on 2018-12-18.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//


#include "DistributionMultivariateNormal.h"
#include "DistributionNormal.h"
#include "DistributionPoisson.h"
#include "EigenSystem.h"
#include "CholeskyDecomposition.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <vector>

using namespace RevBayesCore;

/*!
 *
 */
double pdf(const std::vector<int>& x, const double& theta0, const double& theta1, const double& theta2, const bool& asLog)
{
  const int x1 = x[0];
  const int x2 = x[1];
  int num_terms = std::min(x1, x2) + 1;
  const double th_ratio = theta0 / (theta1 * theta2);
  const double norm = - (theta0 + theta1 + theta2) + x1 * log(theta1) + x2 * log(theta2);

  double llhd = 0;
  for (int ix = 0; ix < num_terms; ix ++) {
    llhd += RbMath::choose(x1, ix) * RbMath::choose(x2, ix) * RbMath::factorial(ix) * std::pow(th_ratio, ix);
  }
  // auto llhd [](const double& i) { choose(x1, i) * choose(x2, i) * factorial(i) * powf(th_ratio, i); };
  // std::array<double, num_terms> indicies;
  // double llhd_terms;
  // std::iota(indicies.begin(), indicies.end(), 0);
  // std::transform(indicies.begin(), indicies.end(), llhd_terms.begin(), llhd);
  // double result = norm * std::accumulate(llhd_terms.begin(), llhd_terms.end(), 0.0) / (factorial(x1) * factorial(x2));
  double result = norm * llhd / (RbMath::factorial(x1) * RbMath::factorial(x2));
  return asLog ? log(result) : result;
}


/*!
 *
 */
std::vector<int> rv(const double& theta0, const double& theta1, const double& theta2, RandomNumberGenerator& rng)
{
  const int x0 = RbStatistics::Poisson::rv(theta0, rng);
  const int x1 = RbStatistics::Poisson::rv(theta1, rng);
  const int x2 = RbStatistics::Poisson::rv(theta2, rng);
  std::vector<int> result;
  result.push_back(x0 + x1);
  result.push_back(x0 + x2);
  return result;
}
