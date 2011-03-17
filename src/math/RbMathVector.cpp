/**
 * @file RbMathMatrix
 * This file contains the math on matrices.
 *
 * @brief Implementation of matrix algebra.
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


#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "RbException.h"
#include "RbMath.h"
#include "RbMathVector.h"
#include "RbSettings.h"



// Vector Functions


/*!
 * This function normalizes a vector such that its sum is some value.
 *
 * \brief Vector normalization function.
 * \param x is a reference to the vector to be normalized .
 * \param sum is the desired sum for the normalized vector.
 * \return Does not return a value. 
 * \throws Does not throw an error.
 */
void RbMath::normalize(std::vector<double>& x, double sum) {
    
    double s = 0.0;
    for (size_t i=0; i<x.size(); i++)
        s += x[i];
    double factor = sum / s;
    for (size_t i=0; i<x.size(); i++)
        x[i] *= factor;
}

void RbMath::vectorMultiplication(const VectorReal& v1, const VectorReal& v2, MatrixReal& p) {
    
    if ( v1.size() != v2.size() )
        throw (RbException("Cannot multiply two vectors of different dimensions"));
    
    std::cout << "v1 : " << v1.getIsRowVector() << std::endl;
    std::cout << "v2 : " << v2.getIsRowVector() << std::endl;
    
    int n = v1.size();
    if ( v1.getIsRowVector() == true && v2.getIsRowVector() == false )
    {
        VectorInteger sizeVec(2);
        sizeVec[0] = 1;
        sizeVec[1] = 1;
        p.resize(sizeVec);
        
        double sum = 0.0;
        for (size_t i=0; i<n; i++)
            sum += v1[i] * v2[i];
        p[0][0] = sum;
    }
    else if ( v1.getIsRowVector() == false && v2.getIsRowVector() == true )
    {
        VectorInteger sizeVec(2);
        sizeVec[0] = n;
        sizeVec[1] = n;
        p.resize(sizeVec);
        
        for (size_t i=0; i<n; i++)
            for (size_t j=0; j<n; j++)
                p[i][j] = v1[i] * v2[j];
    }
    else if ( v1.getIsRowVector() == false && v2.getIsRowVector() == false )
    {
        throw (RbException("Cannot multiply two column vectors"));
    }
    else 
    {
        throw (RbException("Cannot multiply two row vectors"));
    }
}

