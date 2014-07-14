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

PrecisionMatrix::PrecisionMatrix(const PrecisionMatrix& from) : MatrixReal(from.getDim(), from.getDim(), 0), eigensystem(this), eigenflag(false), inverse(from.inverse) {}


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
    /*
    if (std::isnan(tot))    {
        std::cerr << "log det is nan\n";
        std::cerr << "eigen values:\n";
        for (size_t i=0; i<getDim(); i++)   {
            std::cerr << eigenval[i] << '\n';
        }
        if (getDim() == 2)  {
            std::cerr << '\n';
            std::cerr << "matrix is:\n";
            std::cerr << (*this)[0][0] << '\t' << (*this)[0][1] << '\n' << (*this)[1][0] << '\t' << (*this)[1][1] << '\n';
            std::cerr << "determinant computed based on simple 2x2 formula:\n";
            double det = (*this)[0][0] * (*this)[1][1] -  (*this)[1][0] * (*this)[0][1];
            std::cerr << det << '\n';
            std::cerr << "determinant of the characteristic polynomial\n";
            double trace = (*this)[0][0] + (*this)[1][1];
            double delta = trace * trace - 4 * det;
            std::cerr << delta << '\n';
            std::cerr << "eigenvalues:\n";
            double lambda1 =  0.5 * (trace + sqrt(delta));
            double lambda2 =  0.5 * (trace - sqrt(delta));
            std::cerr << lambda1 << '\t' << lambda2 << '\n';
            std::cerr << "final check that eigenvalues are roots of characteristic polynomial:\n";
            double res1 = ((*this)[0][0] - lambda1) * ((*this)[1][1] - lambda1) -  (*this)[1][0] * (*this)[0][1];
            double res2 = ((*this)[0][0] - lambda2) * ((*this)[1][1] - lambda2) -  (*this)[1][0] * (*this)[0][1];
            std::cerr << res1 << '\t' << res2 << '\n'; 
        }
        else    {
            std::cerr << (*this) << '\n';
        }
        throw(NULL);
    
    }
    */
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
    
    if (! eigenflag)    {
                
        try {
            
            eigensystem.update();

            // this may not be optimal but...
            // aim is to get the inverse of the matrix into inverse
            const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();

            double det = (*this)[0][0] * (*this)[1][1] -  (*this)[1][0] * (*this)[0][1];
            double trace = (*this)[0][0] + (*this)[1][1];
            double delta = trace * trace - 4 * det;
            // std::cerr << delta << '\n';
            // std::cerr << "eigenvalues:\n";
            double lambda1 =  0.5 * (trace + sqrt(delta));
            double lambda2 =  0.5 * (trace - sqrt(delta));
            std::cerr << "checking eigenvalues\n";
            std::cerr << "from eigensystem:\n";
            std::cerr << eigenval[0] << '\n';
            std::cerr << eigenval[1] << '\n';
            std::cerr << "from direct calcuation:\n";
            std::cerr << lambda1 << '\n';
            std::cerr << lambda2 << '\n';
            // std::cerr << "final check that eigenvalues are roots of characteristic polynomial:\n";
            // double res1 = ((*this)[0][0] - lambda1) * ((*this)[1][1] - lambda1) -  (*this)[1][0] * (*this)[0][1];
            // double res2 = ((*this)[0][0] - lambda2) * ((*this)[1][1] - lambda2) -  (*this)[1][0] * (*this)[0][1];
            // std::cerr << res1 << '\t' << res2 << '\n'; 
            exit(1);

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
            if (getDim() == 2)  {
                std::cerr << "symmetry : " << (*this)[0][1] - (*this)[1][0] << '\n';
                std::cerr << "det : " << (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0] << '\n';
            }
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
            o << x[i][i] << '\t';
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


