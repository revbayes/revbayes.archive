/**
 * @file
 * This file contains the declaration of ValueContainer, which is
 * a generic container of values.
 *
 * @brief Declaration of ValueContainer
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

#ifndef ValueContainer_H
#define ValueContainer_H

#include "Container.h"
#include "ContainerIterator.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VariableContainer;
class VectorInteger;
class VectorNatural;
class VectorString;

/**
 * @brief Generic container for values
 *
 * This class is used as the generic value container.
 * For convenience, it is possible to derive specific
 * types of value containers from this class. For it
 * to be safe to downcast to these specific types from
 * a generic value container, it is important that
 * VariableNode function getValue() uses the same class
 * when returning its value. Thus, any new ValueContainer
 * specializations need to be added to this function for
 * the code to work as expected.
 * 
 * This class corresponds approximately to the basic
 * value type in R. Lists in R correspond more closely
 * to VariableContainer or even better to MemberObject
 * in RevBayes.
 */
class ValueContainer : public Container {

    public:
        friend class                    VariableContainer;                                                          //!< Give friend class access to elements

                                        ValueContainer(size_t n, RbObject* x);                                      //!< Vector with n copies of x
                                        ValueContainer(const std::vector<size_t>& len, RbObject* x);                //!< Array of given dimensions with copies of x
                                        ValueContainer(const TypeSpec& typeSpec);                                   //!< Empty array of given dimensions
                                        ValueContainer(const TypeSpec& typeSpec, const std::vector<size_t>& length);//!< Default value (NULL) array of given dimensions and length
                                        ValueContainer(VariableContainer& x);                                       //!< Construct from variable container
                                        ValueContainer(const VariableContainer& x);                                 //!< Construct from const variable container
                                        ValueContainer(const ValueContainer& x);                                    //!< Copy constructor
        virtual                        ~ValueContainer(void);                                                       //!< Destructor

        // Assignment operator
        ValueContainer&                 operator=(const ValueContainer& x);                                         //!< Assignment operator

        // Basic utility functions you have to override
        virtual ValueContainer*         clone(void) const;                                                          //!< Clone object
        virtual const VectorString&     getClass(void) const;                                                       //!< Get class

        // Basic utility functions you may want to override
        virtual void                    printValue(std::ostream& o) const;                                          //!< Print value for user
        virtual std::string             richInfo(void) const;                                                       //!< Complete info about object

        // Container functions you may want to override
        virtual void                    clear(void);                                                                //!< Clear
        virtual void                    resize(const std::vector<size_t>& len);                                     //!< Resize to new length vector
        virtual size_t                  size(void) const { return elements.size(); }                                //!< Get total number of elements

    protected:
        // Help function you may want to override
        virtual RbObject*               getDefaultElement(void) const { return NULL; }                              //!< Get default element for empty slots

        // Parser help functions you may want to override
        virtual DAGNode*                getElement(VectorInteger& index);                                           //!< Get element or subcontainer for parser
        virtual void                    setElement(const VectorNatural& index, DAGNode* var);                       //!< Allow parser to set element (or not)

        // Member variable
	    std::vector<RbObject*>          elements;                                                                   //!< Vector of values
};

#endif

