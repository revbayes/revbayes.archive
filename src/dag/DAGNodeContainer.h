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

#include "ContainerIterator.h"
#include "DAGNode.h"
#include "IntVector.h"
#include "RbObjectWrapper.h"
#include "StringVector.h"

#include <ostream>
#include <vector>


class DAGNodeContainer : public RbObjectWrapper {

    public:
            // Constructors and destructor
            DAGNodeContainer(DAGNode* x);                       //!< Vector with one node x
	        DAGNodeContainer(int n, DAGNode* x);                //!< Vector with n copies of x
	        DAGNodeContainer(int n, const StringVector& atomicClassVec, int valDim=0);   //!< Empty vector of length n
	        DAGNodeContainer(const IntVector& len, DAGNode* x);   //!< Array of given dimensions with copies of x
	        DAGNodeContainer(const IntVector& len, const StringVector& atomicClassVec, int valDim=0);    //!< Empty array of given dimensions
            DAGNodeContainer(const DAGNodeContainer& x);        //!< Copy constructor
            ~DAGNodeContainer();                                //!< Destructor

        // Assignment operator
        DAGNodeContainer&       operator=(const DAGNodeContainer& x);               //!< Assignment operator

        // Basic utility functions
        DAGNodeContainer*       clone() const;                                      //!< Clone object
        const StringVector&     getClass() const;                                   //!< Get class
        const RbObject*         getValue() const;                                   //!< Get value
        void                    printValue(std::ostream& o) const;                  //!< Print value (for user)
        std::string             toString() const;                                   //!< Complete info about object

        // Element access functions
        const StringVector&     getAtomicClass() const { return atomicClass; }      //!< Get atomic class
        int                     getDim() const { return length.size() + valueDim; } //!< Get dimensions
        const IntVector&        getElementLength() const { return length; }         //!< Get length of dimensions
        DAGNodeContainer*       getSubContainer(const IntVector& index) const;      //!< Get subcontainer 
        const RbObject*         getValElement(const IntVector& index) const;        //!< Get value element
        const DAGNode*          getVarElement(const IntVector& index) const;        //!< Get variable element
        int                     getWrapperDim() const { return length.size(); }     //!< Get dimensions
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set value element
        void                    setElement(const IntVector& index, RbObjectWrapper* var);//!< Set var element 
        void                    setElementLength(const IntVector& len);             //!< Reorganize container

        // Iteration and access functions for multidimensional indices
        ContainerIterator       begin() const { return ContainerIterator(length); } //!< First index
        ContainerIterator       end() const;                                        //!< Last index + 1
        DAGNode*                operator[](const ContainerIterator& i);             //!< Element access
        const DAGNode* const    operator[](const ContainerIterator& i) const;       //!< Element const access

        // Regular functions
        void                    resize(int n) { resize (IntVector(n)); }            //!< Resize vector
        void                    resize(const IntVector& len);                       //!< Resize container
        size_t                  size() const { return nodes.size(); }               //!< Get number of elements

    protected:
        int                     getOffset(const IntVector& index) const;            //!< Get offset in nodes vector
        int                     getOffset(const ContainerIterator& index) const;    //!< Get offset in nodes vector
        
    private:
        StringVector            atomicClass;    //!< Value element atomic class (from value of DAG nodes)
        int                     valueDim;       //!< Value element dim (from value of DAG nodes)
        IntVector               length;         //!< Length in each dimension
	    std::vector<DAGNode*>   nodes;          //!< Vector of nodes
        RbObject*               value;          //!< Holder of value; fill in if somebody asks
};

#endif
