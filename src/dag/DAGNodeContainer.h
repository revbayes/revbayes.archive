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

#include "IntVector.h"
#include "VariableNode.h"

#include <ostream>
#include <vector>

class Container;
class ContainerIterator;
class DAGNode;
class StringVector;

class DAGNodeContainer : public VariableNode {

    public:
            // Constructors and destructor
            DAGNodeContainer(DAGNode* x);                       //!< Vector with one node x
            DAGNodeContainer(int n, DAGNode* x);                //!< Vector with n copies of x
            DAGNodeContainer(int n, const std::string& valType);//!< Empty vector of length n
            DAGNodeContainer(const IntVector& len, DAGNode* x); //!< Array of given dim with copies of x
            DAGNodeContainer(const IntVector& len, const std::string& valType); //!< Empty array of given dimensions
            DAGNodeContainer(const DAGNodeContainer& x);        //!< Copy constructor
            ~DAGNodeContainer();                                //!< Destructor

        // Overloaded operators
        DAGNodeContainer&       operator=(const DAGNodeContainer& x);               //!< Assignment operator
        DAGNode*&               operator[](const ContainerIterator& i);             //!< Element access
        DAGNode* const&         operator[](const ContainerIterator& i) const;       //!< Element const access
        DAGNode*&               operator[](const int i);                            //!< Element access
        DAGNode* const&         operator[](const int i) const;                      //!< Element const access

        // Replicated vector functions
        void                    resize(int n) { resize (IntVector(n)); }            //!< Resize vector
        void                    resize(const IntVector& len);                       //!< Resize container
        size_t                  size() const { return nodes.size(); }               //!< Get number of elements

        // Iteration functions for multidimensional indices
        ContainerIterator       begin() const { return ContainerIterator(length); } //!< First index
        ContainerIterator       end() const;                                        //!< Last index + 1

        // Basic utility functions
        DAGNodeContainer*       clone() const;                                      //!< Clone object
        const StringVector&     getClass() const;                                   //!< Get class
        const RbObject*         getStoredValue();                                   //!< Get stored value
        const RbObject*         getValue();                                         //!< Get value
        const RbObject*         getValue() const;                                   //!< Get value (const)
        void                    printStruct(std::ostream& o) const;                 //!< Print struct for user
        void                    printValue(std::ostream& o) const;                  //!< Print value for user
        void                    setValue(RbObject* val);                            //!< Set value
        std::string             toString() const;                                   //!< Complete info about object

        // Functions for updating part of a DAG
        void                    getAffected(std::set<StochasticNode*>& affected);   //!< Mark and get affected nodes
        void                    keepAffected(void);                                 //!< Keep value of affected nodes
        void                    restoreAffected(void);                              //!< Restore value of affected nodes

        // overwritten probability functions
        double              getLnLikelihoodRatio(void);             //!< Get log likelihood ratio
        double              getLnPriorRatio(void);                  //!< Get log prior ratio


        // Element access functions
        int                     getDim() const { return length.size(); }            //!< Get dimensions
        const IntVector&        getLength() const { return length; }                //!< Get length in each dim
        DAGNodeContainer*       getSubContainer(const IntVector& index) const;      //!< Get subcontainer
        const RbObject*         getValElement(const IntVector& index) const;        //!< Get value element
        const DAGNode*          getVarElement(const IntVector& index) const;        //!< Get variable element
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set value element
        void                    setElement(const IntVector& index, DAGNode* var);   //!< Set var element 
        void                    setLength(const IntVector& len);                    //!< Reorganize container

    private:
        // Utility functions
        int                     getOffset(const IntVector& index) const;            //!< Get offset in nodes vector
        int                     getOffset(const ContainerIterator& index) const;    //!< Get offset in nodes vector
        void                    update(void);                                       //!< Update value

        // Member variables
        bool                    changed;        //!< Recalculated after touch?
        IntVector               length;         //!< Length in each dimension
	    std::vector<DAGNode*>   nodes;          //!< Vector of nodes
        Container*              value;          //!< Holds value
        Container*              storedValue;    //!< Stored value
};

#endif
