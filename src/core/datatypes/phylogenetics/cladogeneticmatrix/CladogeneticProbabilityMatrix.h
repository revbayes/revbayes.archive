//
//  CladogeneticProbabilityMatrix.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/23/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef CladogeneticProbabilityMatrix_h
#define CladogeneticProbabilityMatrix_h

#include "Assignable.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include "Printable.h"
#include <map>
#include <vector>

namespace RevBayesCore {
    
//    class TransitionProbabilityMatrix;
    
    class CladogeneticProbabilityMatrix : public Cloneable, public Assignable, public Printable, public Serializable {
        
    public:
        CladogeneticProbabilityMatrix(void);                                                   //!< Construct rate matrix with
        CladogeneticProbabilityMatrix(size_t n);                                               //!< Construct rate matrix with n states
        virtual                             ~CladogeneticProbabilityMatrix(void);
        
        virtual bool                        operator==(const CladogeneticProbabilityMatrix &rm) const { return this == &rm; }
        virtual bool                        operator!=(const CladogeneticProbabilityMatrix &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const CladogeneticProbabilityMatrix &rm) const { return this < &rm; }
        virtual bool                        operator<=(const CladogeneticProbabilityMatrix &rm) const { return operator<(rm) || operator==(rm); }
        
        // pure virtual methods
        virtual CladogeneticProbabilityMatrix&              assign(const Assignable &m);
        virtual CladogeneticProbabilityMatrix*              clone(void) const;
        virtual void                                        initFromString( const std::string &s );
        
        // virtual methods that may need to overwritten
        virtual void                                        update(void) {};
        std::map<std::vector<unsigned>, double>             getEventMap(void);
        const std::map<std::vector<unsigned>, double>&      getEventMap(void) const;
        void                                                setEventMap(std::map<std::vector<unsigned>, double> m);
        
        // public methods
//        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;           //!< Calculate the transition probabilities for the rate matrix
        size_t                              getNumberOfStates(void) const;                                                              //!< Return the number of states
        size_t                              size(void) const;                                                                           //!< Get the size of the rate matrix, which is the same as the number of states
        
        virtual void                        printForUser( std::ostream &o, const std::string &sep, int l, bool left ) const;            //!< print object for user (in user-formatted way)
        virtual void                        printForSimpleStoring( std::ostream &o, const std::string &sep, int l, bool left ) const;   //!< print object for user (in user-formatted way)
        virtual void                        printForComplexStoring( std::ostream &o, const std::string &sep, int l, bool left ) const;  //!< print object for user (in user-formatted way)
        
    protected:
        
//        AbstractRateMatrix(size_t n);                                                                                                   //!< Construct rate matrix with n states
//        AbstractRateMatrix(const AbstractRateMatrix& m);                                                                                //!< Copy constructor
//        AbstractRateMatrix&                 operator=(const AbstractRateMatrix& r);                                                     //!< Assignment operator

        
        // protected members available for derived classes
        size_t                              num_states;                                                                                  //!< The number of character states
        std::map<std::vector<unsigned>, double> eventMapProbs;
        
    };
    
};

#endif /* CladogeneticProbabilityMatrix_h */
