/**
 * @file
 * This file contains the declaration of Probability, which
 * is the primitive RevBayes type for positive real numbers between 0 and 1.
 *
 * @brief Declaration of RealPos
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Probability_H
#define Probability_H

#include "RealPos.h"

#include <ostream>
#include <string>

class VectorString;

class Probability : public RealPos {
    
public:
    Probability(void);                                              //!< Default constructor
    Probability(double x);                                          //!< Construct from double
    Probability(int x);                                             //!< Construct from int 
    Probability(unsigned int x);                                    //!< Construct from unsigned int 
    Probability(bool x);                                            //!< Construct from bool
    
    // Basic utility functions
    virtual std::string     briefInfo(void) const;                  //!< Brief info about the object
    Probability*            clone(void) const;                      //!< Clone object
    const VectorString&     getClass(void) const;                   //!< Get class vector
    std::string             richInfo(void) const;                   //!< Complete info about object
    
    // Setter disallowing nonpositive values
    void                    setValue(double x);                     //!< Set value
};

#endif

