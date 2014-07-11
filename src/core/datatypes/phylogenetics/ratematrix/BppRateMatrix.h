/**
 * @file
 * This file contains the declaration of Bpp RateMatrix, which is
 * class that holds a rate matrix from Bio++ definition.
 *
 * @brief Declaration of BppRateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: mardi 8 juillet 2014, à 15h 44 $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: BppRateMatrix.h 2014 Gueguen $
 */

#ifndef BppRateMatrix_H
#define BppRateMatrix_H

#include "RateMatrix.h"
//#include <complex>
#include <vector>

#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Phyl/Model/SubstitutionModel.h>

namespace RevBayesCore {
    
    class BppRateMatrix : public RateMatrix {
    private:
      bpp::SubstitutionModel* pModel_;

      size_t nbStates_;
      
      // List of parameters for later update
      bpp::ParameterList parList_;

      bool needsUpdate_;
      
    public:
      
      /**
       * @brief Takes ownership of a bpp SubstitutionModel*.
       *
       */
      
      BppRateMatrix(bpp::SubstitutionModel* model);

      BppRateMatrix(const BppRateMatrix& m);
      //!< Copy constructor

      virtual ~BppRateMatrix(void);
      //!< Destructor
        
      // overloaded operators
      BppRateMatrix& operator=(const BppRateMatrix& r);

      const std::vector<double>&                operator[](size_t i) const { return static_cast<const bpp::RowMatrix<double>* >(&pModel_->getGenerator())->getRow(i);}                                                             //!< Subscript operator

      BppRateMatrix* clone(void) const
      {
        return new BppRateMatrix(*this);
      }

      
      // public methods

      size_t getNumberOfStates()  const
      {
        return nbStates_;
      }
      
      virtual double averageRate(void) const
      {
        return pModel_->getRate();
      }    //!< Calculate the average rate

      
      void rescaleToAverageRate(double r)
      {
        pModel_->setRate(r);
      }
              
      void calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix


      void setParameterValue(const std::string& name, double value);
      
      const std::vector<double>&          getStationaryFrequencies(void) const;                                                   //!< Return the stationary frequencies

//      void                                setTransitionRates(const std::vector<double> &tr);
      void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
      void                                updateMatrix(void);                                                                     //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    // protected:
      
    //   void calculateStationaryFrequencies(void);      
        
    };
    
    
}

#endif


