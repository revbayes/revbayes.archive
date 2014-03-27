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

using namespace RevBayesCore;

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

void PrecisionMatrix::drawNormalSample(std::vector<double>& v)  const {
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (int i=0; i<getDim(); i++)  {
        w[i] = RbStatistics::Normal::rv(0, 1.0 / sqrt(eigen[i]), *rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (int i=0; i<getDim(); i++)  {
        double tmp = 0;
        for (int j=0; j<getDim(); j++)  {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }
    
}

void PrecisionMatrix::drawNormalSampleFromInverse(std::vector<double>& v)  const {
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (int i=0; i<getDim(); i++)  {
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
        
        eigensystem.update();
        
        const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
        
        MatrixReal tmp(getDim(),getDim(),0);
     
        for (size_t i=0; i<getDim(); i++)   {
            tmp[i][i] = 1.0 / eigenval[i];
        }

        tmp *= eigensystem.getInverseEigenvectors();
        inverse = eigensystem.getEigenvectors() * tmp;
        
        eigenflag = true;
    }
}
