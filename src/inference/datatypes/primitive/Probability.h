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

class Probability : public RealPos {
    
public:
    Probability(void);                                                      //!< Default constructor
    Probability(double x);                                                  //!< Construct from double
    Probability(int x);                                                     //!< Construct from int 
    Probability(unsigned int x);                                            //!< Construct from unsigned int 
    Probability(bool x);                                                    //!< Construct from bool
    
    // Basic utility functions
    Probability*                    clone(void) const;                      //!< Clone object
    static const std::string&       getClassName(void);                     //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                 //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                //!< Get language type of the object
    
    // Setter disallowing nonpositive values
    void                            setValue(double x);                     //!< Set value
    
};

#endif

