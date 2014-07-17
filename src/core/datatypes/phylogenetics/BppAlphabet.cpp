//
//  BppAlphabet.cpp
//  RevBayesCore
//
//  Created by Laurent Guéguen on mardi 8 juillet 2014, à 15h 51.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "BppAlphabet.h"

using namespace RevBayesCore;
using namespace bpp;

#include <Bpp/Exceptions.h>

BppAlphabet::BppAlphabet(const Alphabet* alphabet) : pAlphabet_(alphabet)
{    

}


BppAlphabet::BppAlphabet(const BppAlphabet &rm) : pAlphabet_(rm.pAlphabet_->clone())
{
}


BppAlphabet::~BppAlphabet(void)
{
  if (pAlphabet_)
    delete pAlphabet_;
  
}


BppAlphabet& BppAlphabet::operator=(const BppAlphabet &rm)
{
  if (this != &rm)
    pAlphabet_=  rm.pAlphabet_->clone();
    
  return *this;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const BppAlphabet& x) {
  o << x.getAlphabet()->getAlphabetType();    
  return o;
}
