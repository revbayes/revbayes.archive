#ifndef RateMatrix_H
#define RateMatrix_H

#include "Assignable.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include <vector>


namespace RevBayesCore {

    class TransitionProbabilityMatrix;


    class RateMatrix : public Cloneable, public Assignable {

    public:
        virtual                            ~RateMatrix(void);                                                                           //!< Destructor

        // overloaded operators
        virtual std::vector<double>&                operator[](size_t i) = 0;                                                           //!< Subscript operator
        virtual const std::vector<double>&          operator[](size_t i) const = 0;                                                     //!< Subscript operator (const)
        
        virtual bool                        operator==(const RateMatrix &rm) const { return this == &rm; }
        virtual bool                        operator!=(const RateMatrix &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const RateMatrix &rm) const { return this < &rm; }
        virtual bool                        operator<=(const RateMatrix &rm) const { return operator<(rm) || operator==(rm); }
        
        virtual std::vector<std::vector<double> >::const_iterator       begin(void) const = 0;
        virtual std::vector<std::vector<double> >::iterator             begin(void) = 0;
        virtual std::vector<std::vector<double> >::const_iterator       end(void) const = 0;
        virtual std::vector<std::vector<double> >::iterator             end(void) = 0;


        // pure virtual methods you have to overwrite
        virtual double                      averageRate(void) const = 0;                                                                //!< Calculate the average rate
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;       //!< Calculate the transition probabilities for the rate matrix
        virtual RateMatrix*                 clone(void) const = 0;
        virtual const std::vector<double>&  getStationaryFrequencies(void) const = 0;                                                   //!< Return the stationary frequencies
        virtual void                        rescaleToAverageRate(double r) = 0;                                                         //!< Rescale the rate matrix such that the average rate is "r"
        virtual void                        setDiagonal(void) = 0;                                                                      //!< Set the diagonal such that each row sums to zero
        virtual void                        updateMatrix(void) = 0;                                                                     //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)

        virtual RateMatrix&                 assign(const Assignable &m);

        // public methods
        size_t                              getNumberOfStates(void) const;                                                              //!< Return the number of states
        size_t                              size(void) const;                                                                           //!< Get the size of the rate matrix, which is the same as the number of states

    protected:
        // prevent instantiation
        RateMatrix(size_t n);                                                                                                           //!< Construct rate matrix with n states

        
        // protected members available for derived classes
        size_t                              numStates;                                                                                  //!< The number of character states
        
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMatrix& x);                                           //!< Overloaded output operator

}

#endif

