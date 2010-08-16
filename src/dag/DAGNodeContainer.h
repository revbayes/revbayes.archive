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
class IntVector;
class RbString;
class StringVector;

class DAGNodeContainer : public VariableNode {

    public:
        friend class Frame;                                     //!< Give Frame direct access
        friend class Func__lookup;                              //!< Give Func__lookup direct access

            DAGNodeContainer(DAGNode* x);                       //!< Vector with one node x
            DAGNodeContainer(size_t n, DAGNode* x);             //!< Vector with n copies of x
            DAGNodeContainer(size_t n, const std::string& valType); //!< Empty vector of length n
            DAGNodeContainer(const IntVector& len, DAGNode* x); //!< Array of given dim with copies of x
            DAGNodeContainer(const IntVector& len, const std::string& valType); //!< Empty array of given dimensions
            DAGNodeContainer(const DAGNodeContainer& x);        //!< Copy constructor
            ~DAGNodeContainer();                                //!< Destructor

        // Overloaded operators
        DAGNodeContainer&       operator=(const DAGNodeContainer& x);               //!< Assignment operator
        DAGNode*&               operator[](const IntVector& i);                     //!< Element access
        DAGNode* const&         operator[](const IntVector& i) const;               //!< Element const access
        DAGNode*&               operator[](const size_t i);                         //!< Element access
        DAGNode* const&         operator[](const size_t i) const;                   //!< Element const access

        // Modified vector functions
        ContainerIterator       begin(void) const;                                  //!< Begin iterator
        ContainerIterator       end(void) const;                                    //!< End iterator
        void                    clear(void);                                        //!< Clear vector
        void                    resize(size_t n) { resize (IntVector(int(n))); }    //!< Resize vector
        void                    resize(const IntVector& len);                       //!< Resize container
        size_t                  size(void) const { return nodes.size(); }           //!< Get size of container

        // Basic utility functions
        DAGNodeContainer*       clone() const;                                      //!< Clone object
        const StringVector&     getClass() const;                                   //!< Get class
        const RbObject*         getStoredValue();                                   //!< Get stored value
        const RbObject*         getValue();                                         //!< Get value
        const RbObject*         getValue() const;                                   //!< Get const value if poss.
        void                    printStruct(std::ostream& o) const;                 //!< Print struct for user
        void                    printValue(std::ostream& o) const;                  //!< Print value for user
        void                    setName(std::string& name);                         //!< Set name of container and elements
        void                    setValue(RbObject* val);                            //!< Set value
        std::string             toString() const;                                   //!< Complete info about object

        // DAG functions
        DAGNodeContainer*       cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const; //!< Clone entire graph                                      //!< Clone object
        void                    getAffected(std::set<StochasticNode*>& affected);   //!< Mark and get affected nodes
        void                    keepAffected(void);                                 //!< Keep value of affected nodes
        void                    restoreAffected(void);                              //!< Restore value of affected nodes
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);       //!< Swap a parent node
        void                    touchAffected(void);                                //!< Tell affected nodes value is reset

        // Move function
        MoveSchedule*           getDefaultMoves(void) { return NULL; }              //!< Get default moves

        // Element access functions
        int                     getDim() const { return int(length.size()); }       //!< Get dimensions
        const IntVector&        getLength() const { return length; }                //!< Get length in each dim
        std::string             getElementName(const IntVector& index) const;       //!< Get name of element 
        DAGNodeContainer*       getSubContainer(const IntVector& index) const;      //!< Get subcontainer
        const RbObject*         getValElement(const IntVector& index) const;        //!< Get value element
        const DAGNode*          getVarElement(const IntVector& index) const;        //!< Get variable element
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set value element
        void                    setElement(const IntVector& index, DAGNode* var);   //!< Set var element 
        void                    setElementName(const IntVector& index, const std::string& name);    //!< Set name of element 
        void                    setLength(const IntVector& len);                    //!< Reorganize container

    private:
        // Utility functions
        size_t                  getOffset(const IntVector& index) const;            //!< Get offset in nodes vector
        void                    update(void);                                       //!< Update value

        // Give friend class Frame access to elements
        DAGNode*                getVarElement(const IntVector& index);              //!< Get variable element

        // Member variables
        bool                    changed;        //!< Recalculated after touch?
        IntVector               length;         //!< Length in each dimension
	    std::vector<DAGNode*>   nodes;          //!< Vector of nodes
        StringVector*           names;          //!< Vector of element names
        Container*              value;          //!< Holds value
        Container*              storedValue;    //!< Stored value
};

#endif
