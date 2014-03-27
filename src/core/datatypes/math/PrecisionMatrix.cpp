//
//  PrecisionMatrix.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "PrecisionMatrix.h"

#include "EigenSystem.h"

using namespace RevBayesCore;
    

size_t PrecisionMatrix::getDim() const  {
    
    return getNumberOfColumns();
}

double PrecisionMatrix::getLogDet()  const {
    
    eigensystem.update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    double tot = 0;
    for (size_t i=0; i<getDim(); i++)   {
        tot += log(eigenval[i]);
    }
    
    return tot;
}

bool PrecisionMatrix::isPositive()  const {
    
    eigensystem.update();
    
    const std::vector<double>& eigenval = eigensystem.getRealEigenvalues();
    
    bool pos = true;
    for (size_t i=0; i<getDim(); i++)   {
        pos &= (eigenval[i] > 0);
    }
    
    return pos;
}

