//
//  MatrixRealSymmetric.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "MatrixRealSymmetric.h"

#include "EigenSystem.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"
#include "TypedDagNode.h"

#include <iomanip>

using namespace RevBayesCore;

//MatrixRealSymmetric::MatrixRealSymmetric(void) : MatrixReal(1,1,0.0),
//    eigensystem(this),
//    eigenflag(false),
//    inverse(1,1,0.0)
//{
//    
//}

MatrixRealSymmetric::MatrixRealSymmetric(size_t n) : MatrixReal(n,n,0),
    eigensystem(this),
    eigenflag(false),
    inverse(n,n,0)
{
    
}

//MatrixRealSymmetric::MatrixRealSymmetric(const MatrixRealSymmetric& from) : MatrixReal(from),
//    eigensystem(this),
//    eigenflag(false),
//    inverse(from.inverse)
//{
//    
//}

MatrixRealSymmetric::MatrixRealSymmetric(const MatrixReal& from) : MatrixReal(from),
    eigensystem(this),
    eigenflag(false),
    inverse(from.getNumberOfColumns(), from.getNumberOfColumns(), 0)
{
   
    if (getNumberOfRows() != getNumberOfColumns())
    {
        std::cerr << "error in MatrixRealSymmetric: copy constructor from a non-square matrix\n";
        throw(NULL);
    }
}


MatrixRealSymmetric*  MatrixRealSymmetric::clone(void) const
{
        return new MatrixRealSymmetric(*this);
}


size_t MatrixRealSymmetric::getDim() const
{
    
    return getNumberOfColumns();
}

bool MatrixRealSymmetric::isNull() const
{
    
    return ((getDim() == 1) && ((*this)[0][0] == 0));
}


double MatrixRealSymmetric::getLogDet() const
{
    
    update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    double tot = 0;
    for (size_t i=0; i<getDim(); i++)
    {
        tot += log(eigenval[i]);
    }
    if (std::isnan(tot))
    {
        std::cerr << "in MatrixRealSymmetric::getLogDet(): nan\n";
        std::cerr << "eigen values:\n";
        for (size_t i=0; i<getDim(); i++)
        {
            std::cerr << eigenval[i] << '\n';
        }
        throw(NULL);    
    }
    return tot;
}

MatrixRealSymmetric MatrixRealSymmetric::getInverse() const
{
    
    update();
    
    return inverse;
    
}

bool MatrixRealSymmetric::isPositive()  const
{
    
    update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    bool pos = true;
    for (size_t i=0; i<getDim(); i++)
    {
        pos &= (eigenval[i] > 0);
    }
    
    return pos;
}

void MatrixRealSymmetric::drawNormalSamplePrecision(std::vector<double>& v)  const
{
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (size_t i=0; i<getDim(); i++)
    {
       w[i] = RbStatistics::Normal::rv(0, 1.0 / sqrt(eigen[i]), *rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<getDim(); i++)
    {
        double tmp = 0;
        for (size_t j=0; j<getDim(); j++)
        {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }
    
}

void MatrixRealSymmetric::drawNormalSampleCovariance(std::vector<double>& v)  const
{
    
    update();
    
    // simulate the new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(getDim());
    for (size_t i=0; i<getDim(); i++)
    {
        w[i] = RbStatistics::Normal::rv(0, sqrt(eigen[i]), *rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<getDim(); i++)
    {
        double tmp = 0;
        for (size_t j=0; j<getDim(); j++)
        {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }
    
}

void MatrixRealSymmetric::touch(void)
{
    
    eigenflag = false;
}

void MatrixRealSymmetric::update()  const
{

    // just for debugging (checking that eigen systm is indeed updated when it says it is))
    /*
    if (eigenflag)  
     {

        MatrixReal tmp(getDim(), getDim(), 0);
        MatrixReal tmp2(getDim(), getDim(), 0);
        const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
        for (size_t i = 0; i < getDim(); i++) 
        {
            tmp[i][i] = eigenval[i];
        }

        tmp *= eigensystem.getInverseEigenvectors();
        tmp2 = eigensystem.getEigenvectors() * tmp;
        
        for (size_t i = 0; i < getDim(); i++) 
        {
            for (size_t j = 0; j < getDim(); j++) 
            {
                if (fabs(tmp2[i][j] - (*this)[i][j])>1e-6)  
                {
                    std::cerr << "error: diag not correctly set up\n";
                    std::cerr << i << '\t' << j << '\t' << tmp[i][j] << '\n';
                    exit(1);
                }
            }
        }

        
    }
    */
    if (! eigenflag)
    {
                
        try
        {

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
            for (size_t i = 0; i < getDim(); i++)
            {
                tmp[i][i] = 1.0 / eigenval[i];
            }
            
            tmp *= eigensystem.getInverseEigenvectors();
            inverse = eigensystem.getEigenvectors() * tmp;

            eigenflag = true;
           
        }
        
        catch(...)
        {
            throw RbException("MatrixRealSymmetric: eigen update failed");
        }

    }
}

double MatrixRealSymmetric::getCovariance(size_t k, size_t l)  const
{

    if (k>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    if (l>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    return (*this)[k][l];
}

double MatrixRealSymmetric::getPrecision(size_t k, size_t l)  const
{

    if (k>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    if (l>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    update();
    
    return inverse[k][l];
}

double MatrixRealSymmetric::getCorrel(size_t k, size_t l)  const
{

    if (k>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    if (l>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    return (*this)[k][l] / sqrt((*this)[k][k] * (*this)[l][l]);
}

double MatrixRealSymmetric::getPartialCorrel(size_t k, size_t l)  const
{

    if (k>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    if (l>getDim())
    {
        throw RbException("Index out of range in symmetric real matrix.");
    }
    
    update();
    
    return - inverse[k][l] / sqrt(inverse[k][k] * inverse[l][l]);
}

void MatrixRealSymmetric::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, double &rv) const
{
    
    if ( n == "covariance" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        const TypedDagNode< int >* l = static_cast<const TypedDagNode<int> *>( args[1] );
        rv = getCovariance(k->getValue()-1,l->getValue()-1);
    }
    else if ( n == "precision" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        const TypedDagNode< int >* l = static_cast<const TypedDagNode<int> *>( args[1] );
        rv = getPrecision(k->getValue()-1,l->getValue()-1);
    }
    else if ( n == "correlation" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        const TypedDagNode< int >* l = static_cast<const TypedDagNode<int> *>( args[1] );
        rv = getCorrel(k->getValue()-1,l->getValue()-1);
    }
    else if ( n == "partialCorrelation" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        const TypedDagNode< int >* l = static_cast<const TypedDagNode<int> *>( args[1] );
        rv = getPartialCorrel(k->getValue()-1,l->getValue()-1);
    }
    else
    {
        throw RbException("A MultivariateRealNodeContainer object does not have a member method called '" + n + "'.");
    }
    
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const MatrixRealSymmetric& x) {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.getNumberOfRows(); i++) 
    {
        /*
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        */
        
        o << "[ ";
        for (size_t j = 0; j < x.getNumberOfColumns(); ++j) 
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i != x.getNumberOfRows()-1)
            o << " , ";
        
    }
    
    o << " ] ";    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


