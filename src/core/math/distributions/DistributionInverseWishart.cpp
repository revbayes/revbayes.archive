/* 
 * File:   DistributionInverseInverseWishart.cpp
 * Author: nl
 * 
 * Created on 15 juillet 2014, 10:13
 */

#include "DistributionInverseWishart.h"


#include <cmath>
#include "DistributionInverseWishart.h"
#include "RbException.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart probability density.
 * \param sigma0 is a reference to the scale matrix
 * \param df is the number of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::InverseWishart::pdf(const MatrixRealSymmetric &sigma0, size_t df, const MatrixRealSymmetric &z) {
	
    return exp(lnPdf(sigma0,df,z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief Natural log of InverseWishart probability density.
 * \param sigma0 is a reference to the scale matrix (a precision matrix)
 * \param df is the numger of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseWishart::lnPdf(const MatrixRealSymmetric &sigma0, size_t df, const MatrixRealSymmetric &z) {
    
    sigma0.update();
    z.update();
     
    if (! z.isPositive())    {
        return RbConstants::Double::neginf;
    }
    
    double ret = 0;
    ret += 0.5 * df * sigma0.getLogDet();
    ret -= 0.5 * (df + sigma0.getDim() + 1) * z.getLogDet();
    
    double trace = 0;
    
    const MatrixReal& invz = z.getInverse();
    
    for (size_t i=0; i<sigma0.getDim(); i++)   {
        for (size_t j=0; j<sigma0.getDim(); j++)   {
            trace += sigma0[i][j] * invz[j][i];
        }
    }
    
    ret -= 0.5 * trace;
    
    return ret;

}



/*!
 * This function generates a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart random variable.
 * \param sigma0 is a reference to the scale matrix (a precision matrix)
 * \param df is the numger of degrees of freedom
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the InverseWishart random variable.
 * \throws Does not throw an error.
 */
MatrixRealSymmetric RbStatistics::InverseWishart::rv(const MatrixRealSymmetric &sigma0, size_t df, RandomNumberGenerator& rng)
{

    sigma0.update();
    
    size_t dim = sigma0.getDim();
        
    MatrixRealSymmetric z(dim);
    std::vector<double> tmp(dim);
    
    for (size_t k=0; k<df; k++)
    {
        sigma0.drawNormalSamplePrecision(tmp);
        for (size_t i=0; i<dim; i++)
        {
            for (size_t j=0; j<dim; j++)
            {
                z[i][j] += tmp[i] * tmp[j];
            }
        }
    }

    return z.getInverse();
}


/*!
 * This function calculates the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart probability density.
 * \param kappa is a reference to a vector of double; the scale matrix is then sigma0 = Diagonal(kappa)
 * \param df is the number of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::InverseWishart::pdf(const std::vector<double>& kappa, size_t df, const MatrixRealSymmetric &z) {
	
    return exp(lnPdf(kappa,df,z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief Natural log of InverseWishart probability density.
 * \param kappa is a reference to a vector of double; the scale matrix is then sigma0 = Diagonal(kappa)
 * \param df is the numger of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */

// this log density is only up to a normalization factor that *does* depend on df
// df is therefore assumed to be constant throughout

double RbStatistics::InverseWishart::lnPdf(const std::vector<double>& kappa, size_t df, const MatrixRealSymmetric &z) {

    
    z.update();
    size_t dim = z.getDim();
    
    if (! z.isPositive())    {
        return RbConstants::Double::neginf;
    }
    
    double ret = 0;
    for (size_t i=0; i<dim; i++)   {
        ret += 0.5 * df * log(kappa[i]);
    }
    ret -= 0.5 * (df + dim + 1) * z.getLogDet();
   
    double trace = 0;
    
    const MatrixReal& invz = z.getInverse();
    for (size_t i=0; i<dim; i++)   {
        trace += kappa[i] * invz[i][i];
    }
    
    ret -= 0.5 * trace;
    
    return ret;

}

/*!
 * This function generates a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart random variable.
 * \param kappa is a reference to a vector of double; the scale matrix is then sigma0 = Diagonal(kappa)
 * \param df is the numger of degrees of freedom
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the InverseWishart random variable.
 * \throws Does not throw an error.
 */
MatrixRealSymmetric RbStatistics::InverseWishart::rv(const std::vector<double>& kappa, size_t df, RandomNumberGenerator& rng) {
        
    size_t dim = kappa.size();
    MatrixRealSymmetric z(dim);
    std::vector<double> tmp(dim);

    for (size_t k=0; k<df; k++)   {

        for (size_t i=0; i<dim; i++)  {
            double sk = 1.0 / sqrt(kappa[i]);
            tmp[i] = RbStatistics::Normal::rv(0, sk, rng);
        }

        for (size_t i=0; i<dim; i++)   {
            for (size_t j=0; j<dim; j++)   {
                z[i][j] += tmp[i] * tmp[j];
            }
        }
    }

    return z.getInverse();
}



/*!
 * This function calculates the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart probability density.
 * \param kappa is a reference to a double; the scale matrix is then sigma0 = kappa * I
 * \param df is the number of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::InverseWishart::pdf(double kappa, size_t df, const MatrixRealSymmetric &z) {
	
    return exp(lnPdf(kappa,df,z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a InverseWishart-distributed random variable.
 *
 * \brief Natural log of InverseWishart probability density.
 * \param kappa is a reference to a double; the scale matrix is then sigma0 = kappa * I
 * \param df is the numger of degrees of freedom
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */

// this log density is only up to a normalization factor that *does* depend on df
// df is therefore assumed to be constant throughout

double RbStatistics::InverseWishart::lnPdf(double kappa, size_t df, const MatrixRealSymmetric &z) {

    
    z.update();
    size_t dim = z.getDim();
    
    if (! z.isPositive())    {
        return RbConstants::Double::neginf;
    }
    
    double ret = 0;
    ret += 0.5 * df * dim * log(kappa);
    ret -= 0.5 * (df + dim + 1) * z.getLogDet();
   
    double trace = 0;
    
    const MatrixReal& invz = z.getInverse();
    for (size_t i=0; i<dim; i++)   {
        trace += kappa * invz[i][i];
    }
    
    ret -= 0.5 * trace;
    
    return ret;

}

/*!
 * This function generates a InverseWishart-distributed random variable.
 *
 * \brief InverseWishart random variable.
 * \param kappa is a reference to a double; the scale matrix is then sigma0 = kappa * I
 * \param df is the numger of degrees of freedom
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the InverseWishart random variable.
 * \throws Does not throw an error.
 */
MatrixRealSymmetric RbStatistics::InverseWishart::rv(double kappa, size_t dim, size_t df, RandomNumberGenerator& rng) {
        
    MatrixRealSymmetric z(dim);
    std::vector<double> tmp(dim);

    double sk = 1.0 / sqrt(kappa);
    for (size_t k=0; k<df; k++)   {
        
        for (size_t i=0; i<dim; i++)  {
            tmp[i] = RbStatistics::Normal::rv(0, sk, rng);
        }

        for (size_t i=0; i<dim; i++)   {
            for (size_t j=0; j<dim; j++)   {
                z[i][j] += tmp[i] * tmp[j];
            }
        }
    }
    
    return z.getInverse();
}

