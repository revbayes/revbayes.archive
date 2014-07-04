//
//  BppRateMatrix.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen on mercredi 16 avril 2014, à 23h 31.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "BppRateMatrix.h"

#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

using namespace RevBayesCore;
using namespace bpp;

#include <Bpp/Exceptions.h>

BppRateMatrix::BppRateMatrix(SubstitutionModel* model) : RateMatrix(model?model->getNumberOfStates():0),
                                                         pModel_(model), parList_(model->getParameters())
{    

}


BppRateMatrix::BppRateMatrix(const BppRateMatrix &rm) : RateMatrix(rm)
{
  pModel_=(rm.pModel_?rm.pModel_->clone():0);
}


BppRateMatrix::~BppRateMatrix(void)
{
  if (pModel_)
   delete pModel_;
  
    // nothing to do
}


BppRateMatrix& BppRateMatrix::operator=(const BppRateMatrix &rm)
{
   if (this != &rm)
   {
     RateMatrix::operator=( rm );
        
     pModel_=(rm.pModel_?rm.pModel_->clone():0);
   }
    
  return *this;
}


BppRateMatrix* BppRateMatrix::clone() const
{
  return new BppRateMatrix(*this);
}

// const std::vector<double>& BppRateMatrix::getTransitionRates(void)
// {
  
// }

double BppRateMatrix::averageRate(void) const
{
  return pModel_->getRate();
}

void BppRateMatrix::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
  for (size_t i= 0; i < getNumberOfStates(); i++)
    for (size_t j= 0; j < getNumberOfStates(); j++)
      P.getElement(i,j)=pModel_->Pij_t(i,j,t);
}

const std::vector<double>& BppRateMatrix::getStationaryFrequencies( void ) const
{
  return pModel_->getFrequencies();
}


/** Unvalid function : set the exchangeability rates directly. We
 assume that we know what the exchangeability rates are when this
 function is called. */

// void BppRateMatrix::setTransitionRates(const std::vector<double>& tr)
// {
//   throw bpp::Exception("Unvalid function : BppRateMatrix::setTransitionRates");
// }


/** Set the stationary frequencies directly. We assume that we know
 what the stationary frequencies are when this function is called. */
void BppRateMatrix::setStationaryFrequencies(const std::vector<double>& f)
{
  throw bpp::Exception("Unvalid function : BppRateMatrix::setStationaryFrequencies");
}


void BppRateMatrix::calculateStationaryFrequencies(void)
{
}

void BppRateMatrix::setParameterValue(const std::string& name, double value)
{
  parList_.setParameterValue(name,value);
}

  
void BppRateMatrix::updateMatrix( void )
{
    if ( needsUpdate ) 
    {
      pModel_->matchParametersValues(parList_);
      needsUpdate = false;
    }
}

