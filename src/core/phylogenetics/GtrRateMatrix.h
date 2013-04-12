//
//  GtrRateMatrix.h
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 12/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef GtrRateMatrix_H
#define GtrRateMatrix_H


#include <vector>

namespace RevBayesCore {
    
    class RateMatrix_GTR;
    
    namespace RbPhylogenetics {
        
        namespace Gtr {
            RateMatrix_GTR          computeRateMatrix(const std::vector<double> &er, const std::vector<double> &bf);
            void                    computeRateMatrix(const std::vector<double> &er, const std::vector<double> &bf, RateMatrix_GTR *rm);
        }
        
    }
    
}


#endif        
