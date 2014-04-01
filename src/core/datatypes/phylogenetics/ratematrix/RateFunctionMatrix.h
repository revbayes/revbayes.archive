//
//  RateFunctionMatrix.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/1/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateFunctionMatrix__
#define __rb_mlandis__RateFunctionMatrix__


#include "MatrixReal.h"
#include "RateMatrix.h"
#include <vector>


namespace RevBayesCore {
    
    class RateFunctionMatrix : public RateMatrix {
        
    public:
        virtual                            ~RateFunctionMatrix(void);                                                               //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                                                          //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                                                      //!< Return the number of characters
        size_t                              size(void) const;                                                                       //!< Get the size of the rate matrix, which is the same as the number of states
        double                              getRate(DiscreteCharacterState* fromState, DiscreteCharacterState* toState) const;
        
        // pure virtual methods you have to overwrite
        virtual RateFunctionMatrix*         clone(void) const = 0;
        virtual void                        updateMatrix(void) = 0;                                                                 //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    protected:
        // prevent instantiation
        RateFunctionMatrix(const  RateFunctionMatrix& m);                                                                           //!< Copy constructor
        RateFunctionMatrix(size_t n);                                                                                               //!< Construct rate matrix with n states
        RateFunctionMatrix&                 operator=(const RateFunctionMatrix& r);
        
        // protected members available for derived classes
        size_t                              numStates;                                                                              //!< The number of character states
        size_t                              numCharacters;
        bool                                needsUpdate;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateFunctionMatrix& x);                                   //!< Overloaded output operator
    
}

#endif /* defined(__rb_mlandis__RateFunctionMatrix__) */
