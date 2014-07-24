//
//  PrecisionMatrix.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "PrecisionMatrix.h"

#include "EigenSystem.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"

#include <iomanip>

using namespace RevBayesCore;

PrecisionMatrix::PrecisionMatrix(void) : MatrixReal(1,1,0.0), eigensystem(this), eigenflag(false), inverse(1,1,0.0) {
}

PrecisionMatrix::PrecisionMatrix(size_t n) : MatrixReal(n,n,0), eigensystem(this), eigenflag(false), inverse(n,n,0) {
}

PrecisionMatrix::PrecisionMatrix(const PrecisionMatrix& from) : MatrixReal(from), eigensystem(this), eigenflag(false), inverse(from.inverse) {
}

PrecisionMatrix::PrecisionMatrix(const MatrixReal& from) : MatrixReal(from), eigensystem(this), eigenflag(false), inverse(from.getNumberOfColumns(), from.getNumberOfColumns(), 0) {
    if (getNumberOfRows() != getNumberOfColumns())    {
        std::cerr << "error in PrecisionMatrix: copy constructor from a non-square matrix\n";
        throw(NULL);
    }
}


PrecisionMatrix*  PrecisionMatrix::clone(void) const    {
        return new PrecisionMatrix(*this);
}


size_t PrecisionMatrix::getDim() const  {
    
    return getNumberOfColumns();
}

double PrecisionMatrix::getLogDet()  const {
    
    update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    double tot = 0;
    for (size_t i=0; i<getDim(); i++)   {
        tot += log(eigenval[i]);
    }
    if (std::isnan(tot))    {
        std::cerr << "in PrecisionMatrix::getLogDet(): nan\n";
        std::cerr << "eigen values:\n";
        for (size_t i=0; i<getDim(); i++)   {
            std::cerr << eigenval[i] << '\n';
        }
        throw(NULL);    
    }
    return tot;
}

const MatrixReal& PrecisionMatrix::getInverse() const   {
    
    update();
    return inverse;
    
}

bool PrecisionMatrix::isPositive()  const {
    
    update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    bool pos = true;
    for (size_t i=0; i<getDim(); i++)   {
        pos &= (eigenval[i] > 0);
    }
    
    return pos;
}

void PrecisionMatrix::drawNormalSamplePrecision(std::vector<double>& v)  const {
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (size_t i=0; i<getDim(); i++)  {
       w[i] = RbStatistics::Normal::rv(0, 1.0 / sqrt(eigen[i]), *rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<getDim(); i++)  {
        double tmp = 0;
        for (size_t j=0; j<getDim(); j++)  {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }
    
}

void PrecisionMatrix::drawNormalSampleCovariance(std::vector<double>& v)  const {
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (size_t i=0; i<getDim(); i++)  {
        w[i] = RbStatistics::Normal::rv(0, sqrt(eigen[i]), *rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<getDim(); i++)  {
        double tmp = 0;
        for (size_t j=0; j<getDim(); j++)  {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }
    
}

void PrecisionMatrix::touch(void)   {
    eigenflag = false;
}

void PrecisionMatrix::update()  const {

    // just for debugging (checking that eigen systm is indeed updated when it says it is))
    /*
    if (eigenflag)  {

        MatrixReal tmp(getDim(), getDim(), 0);
        MatrixReal tmp2(getDim(), getDim(), 0);
        const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
        for (size_t i = 0; i < getDim(); i++) {
            tmp[i][i] = eigenval[i];
        }

        tmp *= eigensystem.getInverseEigenvectors();
        tmp2 = eigensystem.getEigenvectors() * tmp;
        
        for (size_t i = 0; i < getDim(); i++) {
            for (size_t j = 0; j < getDim(); j++) {
                if (fabs(tmp2[i][j] - (*this)[i][j])>1e-6)  {
                    std::cerr << "error: diag not correctly set up\n";
                    std::cerr << i << '\t' << j << '\t' << tmp[i][j] << '\n';
                    exit(1);
                }
            }
        }

        
    }
    */
    if (! eigenflag)    {
                
        try {

            // why is that necessary ???
            eigensystem.setRateMatrixPtr(this);

            eigensystem.update();

            /*
            if (check)  {
                for (size_t i=0; i<getDim(); i++)   {
                    if (std::fabs(eigenval[i] - bkeigenval[i]) > 1e-6)    {
                        std::cerr << "error: diag flag up but eigen vals not correct\n";
                        exit(1);
                    }
                }
            }
            */

            const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
            
            MatrixReal tmp(getDim(), getDim(), 0);
            for (size_t i = 0; i < getDim(); i++) {
                tmp[i][i] = 1.0 / eigenval[i];
            }

            tmp *= eigensystem.getInverseEigenvectors();
            inverse = eigensystem.getEigenvectors() * tmp;

            eigenflag = true;
           
        }
        
        catch(...)  {
            
            std::cerr << "in PrecisionMatrix: eigen update failed\n";
            std::cerr << *this << '\n';
            throw(NULL);
        }

    }
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const PrecisionMatrix& x) {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.getNumberOfRows(); i++) 
    {
        
        for (size_t j = i+1; j < x.getNumberOfColumns(); ++j) 
        {
            o << x[i][j] << '\t';
        }
    }
    for (size_t i=0; i < x.getNumberOfRows(); i++) {
            o << x[i][i];
            if (i < x.getNumberOfRows() -1) {
                o << '\t';
            }
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


