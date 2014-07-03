/**
 * @file
 * This file contains the declaration of RateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2013-04-25 14:28:23 +0200 (Thu, 25 Apr 2013) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RateMatrix.h 1997 2013-04-25 12:28:23Z hoehna $
 */

#ifndef GeneralRateMatrix_H
#define GeneralRateMatrix_H

#include "RateMatrix.h"
//#include <complex>
#include <vector>

//#include <Bpp/Phyl/Model/SubstitutionModel.h>

namespace RevBayesCore {
    
    class BppRateMatrix : public RateMatrix {
    private:
//      bpp::SubstitutionModel* pModel_;
      
    public:
      /**
       * @brief Takes ownership of a bpp SubstitutionModel*.
       *
       */
      
//      BppRateMatrix(bpp::SubstitutionModel* model);

      BppRateMatrix(const BppRateMatrix& m);
      //!< Copy constructor

      virtual ~BppRateMatrix(void);
      //!< Destructor
        
      // overloaded operators
      BppRateMatrix& operator=(const BppRateMatrix& r);
        
        
      // public methods
        
      virtual double                      averageRate(void) const;                                                            //!< Calculate the average rate
      void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
      virtual BppRateMatrix*          clone(void) const;
      const std::vector<double>&          getTransitionRates(void) const;
      const std::vector<double>&          getStationaryFrequencies(void) const;                                                   //!< Return the stationary frequencies
      bool                                isTimeReversible(void);                                                             //!< Return whether the rate matrix is time reversible
      void                                setTransitionRates(const std::vector<double> &tr);
      void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
      void                                updateMatrix(void);                                                                     //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    protected:
      
      void                                calculateStationaryFrequencies(void);                                                   //!< Calculate the stationary frequencies for the rate matrix
      std::vector<double>                 stationaryFreqs;
      std::vector<double>                 transitionRates;
        
    };
    
    
}

#endif


