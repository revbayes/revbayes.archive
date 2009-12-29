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
#include "IntVector.h"
#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class RbContainer;
class StringVector;

class Container : public RbComplex {

    public:
                            Container(RbObject* x);                         //!< Vector with one node x
                            Container(int n, RbObject* x);                  //!< Vector with n copies of x
                            Container(const IntVector& len, RbObject* x);   //!< Array of given dimensions with copies of x
                            Container(const IntVector& len, const std::string& elemType); //!< Empty array of given dimensions
                            Container(const Container& x);                  //!< Copy constructor
                            ~Container();                                   //!< Destructor

        // Overloaded operators
        Container&          operator=(const Container& x);                  //!< Assignment operator
        RbObject*&          operator[](const ContainerIterator& i);         //!< Element access
        RbObject* const&    operator[](const ContainerIterator& i) const;   //!< Element const access
        RbObject*&          operator[](const int i);                        //!< Element access
        RbObject* const&    operator[](const int i) const;                  //!< Element const access

        // Replicated vector functions
        void                resize(int n) { resize (IntVector(n)); }        //!< Resize vector
        void                resize(const IntVector& len);                   //!< Resize container
        size_t              size() const { return elements.size(); }        //!< Get number of elements

        // Basic utility functions
        Container*          clone() const;                                  //!< Clone object
        bool                equals(const RbObject* x) const;                //!< Equals comparison
        const StringVector& getClass() const;                               //!< Get class
        void                printValue(std::ostream& o) const;              //!< Print value for user
        std::string         toString() const;                               //!< Complete info about object

        // Element access functions
        int                 getDim() const { return length.size(); }        //!< Get subscript dimensions
        const std::string&  getElementType(void) const { return elementType; }  //!< Get element type
        const RbObject*     getElement(const IntVector& index) const;       //!< Get element (read-only)
        const IntVector&    getLength() const { return length; }            //!< Get length in each dim
        Container*          getSubContainer(const IntVector& index) const;  //!< Get subcontainer
        void                setElement(const IntVector& index, RbObject* val);  //!< Set value element
        void                setLength(const IntVector& len);                //!< Reorganize container

    protected:
        int                 getOffset(const ContainerIterator& index) const;//!< Get offset in element vector
        RbObject*           getElementPtr(const IntVector& index);          //!< Allow modify access to element

        // Member variables
        const std::string&      elementType;    //!< Element type (from value of first element)
        IntVector               length;         //!< Length in each dimension
	    std::vector<RbObject*>  elements;       //!< Vector of nodes
};

#endif

