//
//  BppGeneticCode.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen on mardi 8 juillet 2014, à 15h 51.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "BppGeneticCode.h"

using namespace RevBayesCore;
using namespace bpp;

#include <Bpp/Exceptions.h>

BppGeneticCode::BppGeneticCode(const GeneticCode* gc) : pGeneticCode_(gc)
{    

}


BppGeneticCode::BppGeneticCode(const BppGeneticCode &rm) : pGeneticCode_(rm.pGeneticCode_->clone())
{
}


BppGeneticCode::~BppGeneticCode(void)
{
  if (pGeneticCode_)
    delete pGeneticCode_;
}


BppGeneticCode& BppGeneticCode::operator=(const BppGeneticCode &rm)
{
  if (this != &rm)
     pGeneticCode_=  rm.pGeneticCode_->clone();
    
  return *this;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const BppGeneticCode& x) {
  // To be updated
  o << "Genetic Code"; //x.getGeneticCode()->getGeneticCodeType();    
  return o;
}
