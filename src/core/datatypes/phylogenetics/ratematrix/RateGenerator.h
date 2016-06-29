#ifndef RateGenerator_H
#define RateGenerator_H

#include "Assignable.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include "Printable.h"
#include <vector>

namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateGenerator : public Cloneable, public Assignable, public Printable {
        
    public:
        virtual                             ~RateGenerator(void);
        
        virtual bool                        operator==(const RateGenerator &rm) const { return this == &rm; }
        virtual bool                        operator!=(const RateGenerator &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const RateGenerator &rm) const { return this < &rm; }
        virtual bool                        operator<=(const RateGenerator &rm) const { return operator<(rm) || operator==(rm); }

        // pure virtual methods
        virtual RateGenerator&              assign(const Assignable &m) = 0;
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition matrixmatrix
        virtual RateGenerator*              clone(void) const = 0;
        virtual double                      getRate(size_t from, size_t to, double age, double rate) const = 0;                         //!< Calculate the rate from state i to state j over the given time interval scaled by a rate
        
        // virtual methods that may need to overwritten
        virtual void                        update(void) {};
        
        // public methods
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;           //!< Calculate the transition probabilities for the rate matrix
        size_t                              getNumberOfStates(void) const;                                                              //!< Return the number of states
        size_t                              size(void) const;                                                                           //!< Get the size of the rate matrix, which is the same as the number of states

        virtual void                        printForUser( std::ostream &o, const std::string &sep, int l, bool left ) const;            //!< print object for user (in user-formatted way)
        virtual void                        printForSimpleStoring( std::ostream &o, const std::string &sep, int l, bool left ) const;   //!< print object for user (in user-formatted way)
        virtual void                        printForComplexStoring( std::ostream &o, const std::string &sep, int l, bool left ) const;  //!< print object for user (in user-formatted way)
        
    protected:
        // prevent instantiation
        RateGenerator(size_t n);                                                                                                        //!< Construct rate matrix with n states
        
        // protected members available for derived classes
        size_t                              num_states;                                                                                  //!< The number of character states

        
    };
    
};
#endif
