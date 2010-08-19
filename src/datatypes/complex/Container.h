/**
 * @file
 * This file contains the declaration of Container, which is
 * the interface for container objects such as vectors and
 * arrays.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#ifndef Container_H
#define Container_H

#include "VectorInteger.h"
#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class VectorString;

class Container : public RbComplex {

    public:
        virtual                    ~Container(void) {}                                                //!< Virtual destructor

        // Replicated vector functions
        void                        resize(size_t n) { resize (VectorInteger(int(n))); }              //!< Resize vector
        virtual void                resize(const VectorInteger& len) = 0;                             //!< Resize container
        virtual size_t              size(void) const;                                                 //!< Get number of elements

        // Basic utility functions
        virtual Container*          clone(void) const = 0;                                            //!< Clone object
        virtual bool                equals(const RbObject* x) const = 0;                              //!< Equals comparison
        const VectorString&         getClass(void) const;                                             //!< Get class
        virtual void                printValue(std::ostream& o) const = 0;                            //!< Print value for user
        virtual std::string         toString(void) const = 0;                                         //!< Complete info about object

        // Element access functions
        int                         getDim(void) const { return int(length.size()); }                 //!< Get subscript dimensions
        const std::string&          getElementType(void) const { return elementType; }                //!< Get element type
        virtual const RbObject*     getElement(const VectorInteger& index) const = 0;                 //!< Get element (read-only)
        const VectorInteger&        getLength(void) const { return length; }                          //!< Get length in each dim
        virtual Container*          getSubContainer(const VectorInteger& index) const;                //!< Get subcontainer
        virtual void                setElement(const VectorInteger& index, RbObject* val) = 0;        //!< Set value element
        virtual void                setLength(const VectorInteger& len) = 0;                          //!< Reorganize container

	protected:
                                    Container(const std::string& elemType);                           //!< No objects of this class


        virtual RbObject*           convertTo(const std::string& type) const = 0;                     //!< Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const = 0;               //!< Is convertible to type and dim?
        size_t                      getOffset(const VectorInteger& index) const;                      //!< Get offset in element vector

        // Member variables
        const std::string&          elementType;                                                      //!< Element type (from value of first element)
        VectorInteger               length;                                                           //!< Length in each dimension
};

#endif

