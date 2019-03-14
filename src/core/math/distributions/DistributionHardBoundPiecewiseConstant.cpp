/**
 * @file DistributionHardBoundPiecewiseConstant
 * This file contains the functions of the HardBoundPiecewiseConstant distribution.
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

#include <cmath>

#include "DistributionHardBoundPiecewiseConstant.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a HardBoundPiecewiseConstantly-distributed random variable.
 *
 * \brief HardBoundPiecewiseConstant probability density.
 * \param probs are the probabilities of being in each of the intervals, there are n probs/intervals
 * \param breaks define the intervals (probs[i] is the probability breaks[i]<=x<breaks[i+1]), there are n+1 breaks
 * \param x is the HardBoundPiecewiseConstant random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::HardBoundPiecewiseConstant::pdf(std::vector<double>& probs, std::vector<double>& breaks, double x)
{

  if ( x < breaks[0] || x >= breaks[breaks.size()-1])
  {
    return 0.0;
  }

  int i = findInterval(breaks, x);

	return probs[i];
}

/*!
 * This function calculates the natural log of the probability density
 * for a HardBoundPiecewiseConstantly-distributed random variable.
 *
 * \brief HardBoundPiecewiseConstant probability density.
 * \param probs are the probabilities of being in each of the intervals, there are n probs/intervals
 * \param breaks define the intervals (probs[i] is the probability breaks[i]<=x<breaks[i+1]), there are n+1 breaks
 * \param x is the HardBoundPiecewiseConstant random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::HardBoundPiecewiseConstant::lnPdf(std::vector<double>& probs, std::vector<double>& breaks, double x)
{

  if ( x < breaks[0] || x >= breaks[breaks.size()-1])
  {
    return RbConstants::Double::neginf;
  }

  int i = findInterval(breaks, x);

	return log(probs[i]);
}


/*!
 * This function calculates the cumulative probability
 * for a standard HardBoundPiecewiseConstantly-distributed random variable.
 *
 * \brief HardBoundPiecewiseConstant cumulative probability.
 * \param x is the HardBoundPiecewiseConstant random variable.
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::HardBoundPiecewiseConstant::cdf(std::vector<double>& probs, std::vector<double>& breaks, double x) {
  if (x < breaks[0])
  {
    return 0.0;
  }
  if (x >= breaks[breaks.size()-1])
  {
    return 1.0;
  }

  double cdf = 0.0;

  for (size_t i=0; i < breaks.size(); ++i)
  {
    if (x < breaks[i+1])
    {
      cdf += (x - breaks[i])/(breaks[i+1] - breaks[i]) * probs[i];
    }
    else
    {
      cdf += probs[i];
    }
  }
	return cdf;
}

/*!
 * This function calculates the quantiles of a standard HardBoundPiecewiseConstant distribution.
 *
 */
double RbStatistics::HardBoundPiecewiseConstant::quantile(std::vector<double>& probs, std::vector<double>& breaks, double p)
{
  double cdf = 0.0;

  for (size_t i=0; i < breaks.size(); ++i)
  {
    cdf += probs[i];

    // Find the interval this probability lies in
    if (p < cdf)
    {
      cdf -= probs[i];

      double p_remainder = p - cdf;

      // In this interval, we just need the fraction of probs[i] filled by the remaining portion of p
      // From there, it's a uniform random value
      return breaks[i] + p_remainder / probs[i] * (breaks[i+1] - breaks[i]);
    }
  }

}

/*!
 * Generates a HardBoundPiecewiseConstantly distributed random variable
 */

double RbStatistics::HardBoundPiecewiseConstant::rv(std::vector<double>& probs, std::vector<double>& breaks, RandomNumberGenerator& rng)
{

	double p = rng.uniform01();

  double x = quantile(probs, breaks, p);

  return x;
}

/*!
 * A simple binary search to find which interval a value lies in.
 *
 */
int findInterval(std::vector<double>& breaks, double x)
{
  int left = 0;
  int right = breaks.size() - 1;

  int m;

  while (left < right)
  {
    m = floor( (left + right) / 2 );

    if ( x < breaks[m] )
    {
      right = m - 1;
    }
    else if ( x >= breaks[m+1])
    {
      left = m + 1;
    }
    else
    {
      return(m);
    }
  }

  // If we haven't returned we have a problem
  throw(RbException("Failed to find interval in HardBoundPiecewiseConstant distribution."));

}
