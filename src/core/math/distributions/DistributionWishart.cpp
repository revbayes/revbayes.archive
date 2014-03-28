//
//  DistributionWishart.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "DistributionWishart.h"


#include <cmath>
#include "DistributionWishart.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbStatisticsHelper.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a Wishart-distributed random variable.
 *
 * \brief Wishart probability density.
 * \param omega0 is a reference to a vector of doubles containing the center
 * \param df is the number of degrees of freedom
 * \param z is a reference to a precision matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::Wishart::pdf(const PrecisionMatrix &omega0, int df, const PrecisionMatrix &z) {
	
    return exp(lnPdf(omega0,df,z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a Wishart-distributed random variable.
 *
 * \brief Natural log of Wishart probability density.
 * \param omega0 is a reference to the center matrix.
 * \param df is the numger of degrees of freedom
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Wishart::lnPdf(const PrecisionMatrix &omega0, int df, const PrecisionMatrix &z) {
    
    double ret = 0;
    ret -= 0.5 * df * omega0.getLogDet();
    ret += 0.5 * (df - omega0.getDim() - 1) * z.getLogDet();
    
    double trace = 0;
    
    const MatrixReal& inv = omega0.getInverse();
    
    for (size_t i=0; i<omega0.getDim(); i++)   {
        for (size_t j=0; j<omega0.getDim(); j++)   {
            trace += inv[i][j] * z[j][i];
        }
    }
    
    ret -= 0.5 * trace;
    
    return ret;

}



/*!
 * This function generates a Wishart-distributed random variable.
 *
 * \brief Wishart random variable.
 * \param omega0 is a reference to the precision matrix (center).
 * \param df is the number of degrees of freedom
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the Wishart random variable.
 * \throws Does not throw an error.
 */
PrecisionMatrix RbStatistics::Wishart::rv(const PrecisionMatrix &omega0, int df, RandomNumberGenerator& rng) {
    
    size_t dim = omega0.getDim();
    
    PrecisionMatrix z(dim);

    std::vector<double> tmp(dim);
    for (size_t k=0; k<dim; k++)   {
        omega0.drawNormalSampleFromInverse(tmp);
        for (size_t i=0; i<dim; i++)   {
            for (size_t j=0; j<dim; j++)   {
                z[i][j] += tmp[i] * tmp[j];
            }
        }
    }
    
	return z;
}
