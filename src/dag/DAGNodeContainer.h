/**
 * @file
 * This file contains the declaration of DAGNodeContainer, which
 * is used to hold vectors or multidimensional arrays of DAG
 * nodes. The container is initialized with a particular value
 * type and a particular number of dimensions. After that, it
 * is impossible to change the value type or the number of
 * dimensions. You can assign DAG nodes with the correct
 * value type to elements in the container. You can also
 * assign values of the correct type to elements in the con-
 * tainer, in which case they get wrapped into constant nodes.
 *
 * @brief Declaration of DAGNodeContainer
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

#ifndef DAGNodeContainer_H
#define DAGNodeContainer_H

#include "DAGNode.h"
#include "IntVector.h"
#include "RbObjectWrapper.h"

#include <ostream>
#include <vector>


class DAGNodeContainer : public RbObjectWrapper {

    public:
            // Constructors and destructor
            DAGNodeContainer(DAGNode* x);                       //!< Vector with one node x
	        DAGNodeContainer(int n, DAGNode* x);                //!< Vector with n copies of x
	        DAGNodeContainer(int n, const std::string& type);    //!< Empty vector of length n
	        DAGNodeContainer(const IntVector& len, DAGNode* x); //!< Array of given dimensions with copies of x
	        DAGNodeContainer(const IntVector& len, const std::string& type);  //!< Empty array of given dimensions
            DAGNodeContainer(const DAGNodeContainer& x);        //!< Copy constructor
            ~DAGNodeContainer();                                //!< Destructor

        // Assignment operator
        DAGNodeContainer&       operator=(const DAGNodeContainer& x);               //!< Assignment operator

        // Basic utility functions
        std::string             briefInfo() const;                                  //!< Brief info about object
        DAGNodeContainer*       clone() const;                                      //!< Clone object
        bool                    equals(const RbObjectWrapper* x);                   //!< Equals comparison
        const StringVector&     getClass() const;                                   //!< Get class
        const RbObject*         getValue() const;                                   //!< Get value
        void                    printValue(std::ostream& o) const;                  //!< Print value (for user)
        std::string             toString() const;                                   //!< Complete info about object

        // Element access functions
        const DAGNode*          getElement(const IntVector& index) const;           //!< Get element
        int                     getElementDim() const { return length.size(); }     //!< Get dimensions
        const IntVector&        getElementLength() const { return length; }         //!< Get length of dimensions
        const std::string&      getElementType() const { return valueType; }        //!< Get element value type
        DAGNodeContainer*       getSubContainer(const IntVector& index) const;      //!< Get subcontainer 
        void                    setElement(const IntVector& index, RbObjectWrapper* val);   //!< Set element 
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set element from object
        void                    setElementLength(const IntVector& len);             //!< Reorganize container

        // Regular functions
        void                    resize(int n) { resize (IntVector(n)); }            //!< Resize vector
        void                    resize(const IntVector& len);                       //!< Resize container
        size_t                  size() const { return nodes.size(); }               //!< Get number of elements

    protected:
        void                    getNextIndex(IntVector& index, const IntVector& len) const; //!< Iteration 
        int                     getOffset(const IntVector& index) const;            //!< Get offset in nodes vector
        
    private:
        std::string             valueType;  //!< Element value type (type of value of DAG nodes)
        IntVector               length;     //!< Length in each dimension
	    std::vector<DAGNode*>   nodes;      //!< Vector of nodes
        RbObject*               value;      //!< Holder of value; only fill in if somebody asks
};

#endif
