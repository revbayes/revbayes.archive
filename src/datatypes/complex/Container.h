/**
 * @file
 * This file contains the declaration of Container, which is
 * a generic container of RbObject objects.
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

#include "ContainerIterator.h"
#include "VectorInteger.h"
#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class RbContainer;
class VectorString;

class Container : public RbComplex {

    public:
	                                Container(void);                                                  //!< Default constructor
                                    Container(RbObject* x);                                           //!< Vector with one node x
                                    Container(size_t n, RbObject* x);                                 //!< Vector with n copies of x
                                    Container(const VectorInteger& len, RbObject* x);                 //!< Array of given dimensions with copies of x
                                    Container(const VectorInteger& len, const std::string& elemType); //!< Empty array of given dimensions
                                    Container(const Container& x);                                    //!< Copy constructor
								   ~Container(void);                                                  //!< Destructor

        // Overloaded operators
        Container&                  operator=(const Container& x);                                    //!< Assignment operator
        RbObject*&                  operator[](const VectorInteger& i);                               //!< Element access
        RbObject* const&            operator[](const VectorInteger& i) const;                         //!< Element const access
        RbObject*&                  operator[](const size_t i);                                       //!< Element access
        RbObject* const&            operator[](const size_t i) const;                                 //!< Element const access

        // Replicated vector functions
        void                        resize(size_t n) { resize (VectorInteger(int(n))); }              //!< Resize vector
        void                        resize(const VectorInteger& len);                                 //!< Resize container
        size_t                      size(void) const { return elements.size(); }                      //!< Get number of elements

        // Basic utility functions
        Container*                  clone(void) const;                                                //!< Clone object
        bool                        equals(const RbObject* x) const;                                  //!< Equals comparison
        const VectorString&         getClass(void) const;                                             //!< Get class
        void                        printValue(std::ostream& o) const;                                //!< Print value for user
        std::string                 toString(void) const;                                             //!< Complete info about object

        // Element access functions
        int                         getDim(void) const { return int(length.size()); }                 //!< Get subscript dimensions
        const std::string&          getElementType(void) const { return elementType; }                //!< Get element type
        const RbObject*             getElement(const VectorInteger& index) const;                     //!< Get element (read-only)
        const VectorInteger&        getLength(void) const { return length; }                          //!< Get length in each dim
        Container*                  getSubContainer(const VectorInteger& index) const;                //!< Get subcontainer
        void                        setElement(const VectorInteger& index, RbObject* val);            //!< Set value element
        void                        setLength(const VectorInteger& len);                              //!< Reorganize container

	protected:
        RbObject*                   convertTo(const std::string& type) const;                         //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;                   //!< Is convertible to type?
        size_t                      getOffset(const VectorInteger& index) const;                      //!< Get offset in element vector
        RbObject*                   getElementPtr(const VectorInteger& index);                        //!< Allow modify access to element

        // Member variables
        const std::string&          elementType;                                                      //!< Element type (from value of first element)
        VectorInteger               length;                                                           //!< Length in each dimension
	    std::vector<RbObject*>      elements;                                                         //!< Vector of nodes
};

#endif

