/**
 * @file
 * This file contains the declaration of RbComplex, which is
 * the RevBayes abstract base class for complex objects.
 *
 * @brief Declaration of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: RbComplex.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbComplex_H
#define RbComplex_H

#include "RbObject.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class ContainerIterator;
class DAGNode;
class DeterministicNode;
class Frame;
class VectorInteger;
class VectorNatural;
class MoveSchedule;
class VectorString;


class RbComplex : public RbObject {

    public:
		virtual                            ~RbComplex(void) {}                                                      //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual RbComplex*                  clone(void) const = 0;                                                  //!< Clone object
		virtual const VectorString&         getClass(void) const;                                                   //!< Get class vector
		virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user
		virtual std::string                 richInfo(void) const = 0;                                               //!< Complete info 
        
        // Basic utility function you do not have to override
        virtual bool                        isType(const std::string& type) const;                                  //!< Is the object of type?

        // Default moves for types that contain sets of stochastic nodes (member objects and 
        MoveSchedule*                       getDefaultMoves(DeterministicNode* node) const { return NULL; }         //!< Get block moves applicable to the complex datatype

	protected:
                                            RbComplex(void) : RbObject() {}                                         //!< No objects of this class

};

#endif

