/**
 * @file
 * This file contains the declaration of RateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RateMatrix_H
#define RateMatrix_H

#include "MemberObject.h"

class ArgumentRule;
class DAGNode;
class MatrixReal;
class Simplex;
class VectorReal;
class VectorString;



class RateMatrix : public MemberObject {

    public:
                                            RateMatrix(void);                                                  //!< Default constructor (never call this except from the workspace once)
                                            RateMatrix(const RateMatrix& m);                                   //!< Copy constructor
                                            RateMatrix(size_t n);                                              //!< Construct rate matrix with n states
                                           ~RateMatrix(void);                                                  //!< Destructor
        VectorReal&                         operator[](size_t i);                                              //!< Subscript operator
        const VectorReal&                   operator[](size_t i) const;                                        //!< Subscript operator (const)
    
        // Basic utility functions
        RateMatrix*                         clone(void) const;                                                 //!< Clone object
        const VectorString&                 getClass(void) const;                                              //!< Get class vector   
        void                                printValue(std::ostream& o) const;                                 //!< Print value for user
        std::string                         richInfo(void) const;                                              //!< Complete info

        // Member variable rules
        const MemberRules&                  getMemberRules(void) const;                                        //!< Get member rules

        // Member method inits
        const MethodTable&                  getMethods(void) const;                                            //!< Get methods
        
        // RateMatrix functions
        double                              averageRate(void) const;                                           //!< Calculate the average rate
        void                                calculateStationaryFrequencies(void) const;                        //!< Calculate the stationary frequencies for the rate matrix
        bool                                getIsTimeReversible(void);
        void                                rescaleToAverageRate(const double r) const;
        void                                setDiagonal(void);                                                 //!< Set the diagonal such that each row sums to zero
        void                                setIsTimeReversible(const bool tf);  
        void                                setStationaryFrequencies(std::vector<double>& f) const;            //!< Directly set the stationary frequencies
        DAGNode*                            wrapIntoVariable(void); 

	protected:
        DAGNode*                            executeOperation(const std::string& name, ArgumentFrame& args);    //!< Execute method
    
    private:
        bool                                checkTimeReversibity(double tolerance);                            //!< Checks if the rate matrix is time reversible
        bool                                reversibilityChecked;                                              //!< Flag indicating if time reversibility has been checked
        bool                                isReversible;                                                      //!< Is the matrix time reversible
        size_t                              numStates;                                                         //!< The number of character states
        MatrixReal*                         theRateMatrix;                                                     //!< Holds the rate matrix
        Simplex*                            theStationaryFreqs;                                                //!< Holds the stationary frequencies
};

#endif

