/**
 * @file
 * This file contains the declaration of ContainerObject, which is
 * a generic container of any dimension of RbObject objects.
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

#ifndef ContainerObject_H
#define ContainerObject_H

#include "ContainerIterator.h"
#include "VectorInteger.h"
#include "Container.h"

#include <ostream>
#include <string>
#include <vector>

class VectorString;

class ContainerObject : public Container {

    public:
	                                ContainerObject(void);                                                  //!< Default constructor
                                    ContainerObject(RbObject* x);                                           //!< Vector with one node x
                                    ContainerObject(size_t n, RbObject* x);                                 //!< Vector with n copies of x
                                    ContainerObject(const VectorInteger& len, RbObject* x);                 //!< Array of given dimensions with copies of x
                                    ContainerObject(const VectorInteger& len, const std::string& elemType); //!< Empty array of given dimensions
                                    ContainerObject(const ContainerObject& x);                              //!< Copy constructor
								   ~ContainerObject(void);                                                  //!< Destructor

        // Overloaded operators
        ContainerObject&            operator=(const ContainerObject& x);                                    //!< Assignment operator
        RbObject*&                  operator[](const VectorInteger& i);                                     //!< Element access
        RbObject* const&            operator[](const VectorInteger& i) const;                               //!< Element const access
        RbObject*&                  operator[](const size_t i);                                             //!< Element access
        RbObject* const&            operator[](const size_t i) const;                                       //!< Element const access

        // Replicated vector functions
        void                        resize(const VectorInteger& len);                                       //!< Resize container
        size_t                      size(void) const { return elements.size(); }                            //!< Get number of elements

        // Basic utility functions
        ContainerObject*            clone(void) const;                                                      //!< Clone object
        bool                        equals(const RbObject* x) const;                                        //!< Equals comparison
        const VectorString&         getClass(void) const;                                                   //!< Get class
        void                        printValue(std::ostream& o) const;                                      //!< Print value for user
        std::string                 toString(void) const;                                                   //!< Complete info about object

        // Element access functions
        const RbObject*             getElement(const VectorInteger& index) const;                           //!< Get element (read-only)
        ContainerObject*            getSubContainer(const VectorInteger& index) const;                      //!< Get subcontainer
        void                        setElement(const VectorInteger& index, RbObject* val);                  //!< Set value element
        void                        setLength(const VectorInteger& len);                                    //!< Reorganize container

	protected:
        RbObject*                   convertTo(const std::string& type) const;                         //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;                   //!< Is convertible to type and dim?

        // Member variables
	    std::vector<RbObject*>      elements;                                                         //!< Vector of nodes
};

#endif

