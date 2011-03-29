/**
 * @file
 * This file contains the declaration of VariableContainer,
 * which is a container of variables.
 *
 * @brief Declaration of VariableContainer
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

#ifndef VariableContainer_H
#define VariableContainer_H

#include "Container.h"
#include "ContainerIterator.h"
#include "VectorInteger.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorString;


/**
 * This class is used for generic variable containers. All variables
 * may be constants, but they may also be variable nodes. However,
 * they are not allowed to be containers.
 *
 * @note Non-const subscript operator is declared protected so that
 *       only friend classes can access them. We do not want anyone
 *       to mess directly with the elements because it could violate
 *       the basic properties of the container, such as its elementType.
 */
class VariableContainer : public Container {

    public:
        friend class                ValueContainer;                                                             //!< Give value container direct access to elements
        friend class                ContainerNode;                                                              //!< Give container node direct access to elements
        friend class                Frame;                                                                      //!< Give container node direct access to elements

                                    VariableContainer(size_t n, DAGNode* x);                                    //!< Vector with n copies of x
                                    VariableContainer(const std::vector<size_t>& len, DAGNode* x);              //!< Array of given dimensions with copies of x
                                    VariableContainer(const TypeSpec& typeSpec);                                //!< Empty array
                                    VariableContainer(const TypeSpec& typeSpec, const std::vector<size_t>& len);//!< NULL variable array of given dimensions
                                    VariableContainer(const VariableContainer& x);                              //!< Copy constructor
        virtual                    ~VariableContainer(void);                                                    //!< Destructor

        // Overloaded operators for assignment and convenient programmer access
        VariableContainer&          operator=(const VariableContainer& x);                                      //!< Assignment operator
        const DAGNode* const&       operator[](const VectorInteger& i) const;                                   //!< Element const access

        // Basic utility functions
        VariableContainer*          clone(void) const;                                                          //!< Clone object
        const VectorString&         getClass(void) const;                                                       //!< Get class
        void                        printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                 richInfo(void) const;                                                       //!< Complete info about object

        // Container functions
        void                        clear(void);                                                                //!< Clear container
        ValueContainer*             getConstValue(void);                                                        //!< Get constant value
        void                        resize(const std::vector<size_t>& len);                                     //!< Resize container
        size_t                      size(void) const { return elements.size(); }                                //!< Get total number of elements

	protected:
        // Parser operator and help functions
        DAGNode*&                   operator[](const VectorInteger& i);                                         //!< Element modify access
        DAGNode*                    getElement(VectorInteger& index);                                           //!< Get element or subcontainer for parser

        // Member variable
	    std::vector<DAGNode*>       elements;                                                                   //!< Vector of variable elements
};

#endif

