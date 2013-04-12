/**
 * @file
 * This file contains the declaration of RealPos, which
 * is the primitive RevBayes type for positive real numbers.
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

#ifndef RealPos_H
#define RealPos_H

#include "Real.h"

#include <ostream>
#include <string>

namespace RevLanguage {

class RealPos : public Real {

    public:
        RealPos(void);                                                          //!< Default constructor
        RealPos(RevBayesCore::TypedDagNode<double> *x);                         //!< Construct from double
        RealPos(double x);                                                      //!< Construct from double
        RealPos(int x);                                                         //!< Construct from int 

        // Basic utility functions
        virtual RealPos*                clone(void) const;                      //!< Clone object
        static const std::string&       getClassName(void);                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                //!< Get language type of the object
    
};
    
}

#endif

