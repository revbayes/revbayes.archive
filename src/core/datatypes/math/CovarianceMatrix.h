//
//  CovarianceMatrix.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__CovarianceMatrix__
#define __revbayes__CovarianceMatrix__

#include <iostream>

#include "MatrixReal.h"
#include "EigenSystem.h"

namespace RevBayesCore {
    
    class EigenSystem;
    
    class CovarianceMatrix : public MatrixReal {
        
    public:
        CovarianceMatrix(size_t n);                                                             //!< Construct rate matrix with n states
        CovarianceMatrix(const CovarianceMatrix& m);                                                  //!< Copy constructor
        
        virtual                            ~CovarianceMatrix(void);                                                                 //!< Destructor
        
        // overloaded operators
        CovarianceMatrix&                  operator=(const CovarianceMatrix& r);
        
        
        // public methods
        bool                                isPositive() const {return true;}
        
        int                                 getDim() const {return 0;}
        
        double                              getCorrelationCoefficient(int i, int j) const;
        
        virtual CovarianceMatrix*           clone(void) const;
        
        void                                updateMatrix(void);
        //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    protected:
        
        
    private:
        EigenSystem                         eigensystem;

    };
    
    
}

#endif /* defined(__revbayes__CovarianceMatrix__) */
