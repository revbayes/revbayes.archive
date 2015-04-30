//
//  RateGenerator.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__RateGenerator__
#define __revbayes_proj__RateGenerator__

#include "Assignable.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include <vector>

namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateGenerator : public Cloneable, public Assignable {
        
    public:
        virtual                             ~RateGenerator(void);
        
        virtual bool                        operator==(const RateGenerator &rm) const { return this == &rm; }
        virtual bool                        operator!=(const RateGenerator &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const RateGenerator &rm) const { return this < &rm; }
        virtual bool                        operator<=(const RateGenerator &rm) const { return operator<(rm) || operator==(rm); }

        // pure virtual methods
        virtual RateGenerator&              assign(const Assignable &m) = 0;
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition matrixmatrix
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;                       //!< Calculate the transition probabilities for the rate matrix
        virtual RateGenerator*              clone(void) const = 0;
        virtual double                      getRate(size_t from, size_t to, double age, double rate) const = 0;                                     //!< Calculate the rate from state i to state j over the given time interval scaled by a rate
        
        // virtual methods that may need to overwritten
        virtual void                        update(void) {};
        
        // public methods
        size_t                              getNumberOfStates(void) const;                                                                           //!< Return the number of states
        size_t                              size(void) const;                                                                                        //!< Get the size of the rate matrix, which is the same as the number of states
        
    protected:
        // prevent instantiation
        RateGenerator(size_t n);                                                                                                        //!< Construct rate matrix with n states
        
        // protected members available for derived classes
        size_t                              numStates;                                                                                  //!< The number of character states

        
    };
    
    std::ostream&                       operator<<(std::ostream& o, const RateGenerator& x);                                           //!< Overloaded output operator

};
#endif /* defined(__revbayes_proj__RateGenerator__) */
