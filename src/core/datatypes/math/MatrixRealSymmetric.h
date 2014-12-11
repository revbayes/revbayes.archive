//
//  MatrixRealSymmetric.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__MatrixRealSymmetric__
#define __revbayes__MatrixRealSymmetric__

#include <iostream>

#include "MatrixReal.h"
#include "EigenSystem.h"
#include "MemberObject.h"

namespace RevBayesCore {
    
    class EigenSystem;
    
    class MatrixRealSymmetric : public MatrixReal, public MemberObject<double> {
        
    public:
                                            MatrixRealSymmetric(void);                                              //!< Default constructor required by revlanguae use
                                            MatrixRealSymmetric(size_t n);                                          //!< Construct rate matrix with n states
                                            MatrixRealSymmetric(const MatrixRealSymmetric& m);                          //!< Copy constructor
                                            MatrixRealSymmetric(const MatrixReal& m);                          //!< Copy constructor
        
        virtual                            ~MatrixRealSymmetric(void) {}                                            //!< Destructor
                
        // public methods
        // could not make them constant
        // because potentially need to update the eigensystem
        // should implement some update flags here
        bool                                isPositive() const;
        double                              getLogDet() const;
        
        size_t                              getDim() const;
        bool                                isNull() const;
                
        void                                touch();
        void                                update() const;
        
        bool                                isUpdated() const {return eigenflag;}
        
        double                              getCovariance(size_t k, size_t l) const;
        double                              getPrecision(size_t k, size_t l) const;
        double                              getCorrel(size_t k, size_t l) const;
        double                              getPartialCorrel(size_t k, size_t l) const;
        
        void                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls

        
        
        virtual MatrixRealSymmetric*        clone(void) const;
        
        void                                updateMatrix(void);
        //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
        const MatrixReal&                   getInverse(void) const;
        
        void                                drawNormalSamplePrecision(std::vector<double>& v) const;
        void                                drawNormalSampleCovariance(std::vector<double>& v) const;

    protected:
        
        
    private:
        
        mutable EigenSystem                 eigensystem;
        mutable bool                        eigenflag;

        mutable MatrixReal                  inverse;
        
    };
    
    std::ostream&                       operator<<(std::ostream& o, const MatrixRealSymmetric& x);                                           //!< Overloaded output operator
  
}

#endif /* defined(__revbayes__MatrixRealSymmetric__) */
