/**
 * @file
 * This file contains the declaration of VectorRealPos, a complex type
 * used to hold double vectors.
 *
 * @brief Declaration of VectorRealPos
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef VectorRealPos_H
#define VectorRealPos_H

#include "VectorInteger.h"
#include "VectorReal.h"

#include <iostream>
#include <string>
#include <vector>

class VectorRealPos : public VectorReal {

    public:
        // Constructors and destructor
                                    VectorRealPos(void);                                          //!< Default constructor (empty vector)
                                    VectorRealPos(double x);                                      //!< Construct vector with one double x
                                    VectorRealPos(size_t n, double x);                            //!< Construct vector with n doubles x
                                    VectorRealPos(const std::vector<double>& x);                  //!< Constructor from double vector
                                    VectorRealPos(const ContainerIterator& x);                    //!< Constructor from container iterator

        // Basic utility functions, pure virtual in RbComplex and implemented here
        virtual VectorRealPos*      clone(void) const;                                            //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                            //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                         //!< Get class vector
        virtual std::string         toString(void) const;                                         //!< Complete info about object

        // Regular functions, including STL-like functions
        void                        push_back(double x);                                          //!< Append element to end
        void                        push_front(double x);                                         //!< Add element in front
        void                        setValue(const VectorReal& x);                                //!< Set the value
        void                        setValue(const VectorRealPos& x);                             //!< Set the value
        void                        setValue(const std::vector<double>& x);                       //!< Set the value

        // Element access functions for parser
        virtual const std::string&  getElementType(void) const;                                   //!< Get element type
        virtual void                setElement(const VectorInteger& index, RbObject* val);        //!< Set element
};

#endif


