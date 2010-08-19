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

#include "VectorInteger.h"
#include "VariableNode.h"

#include <ostream>
#include <vector>

class ContainerObject;
class ContainerIterator;
class DAGNode;
class VectorInteger;
class RbString;
class VectorString;

class DAGNodeContainer : public VariableNode {

    public:
        friend class            Frame;                                                                   //!< Give Frame direct access
        friend class            Func__lookup;                                                            //!< Give Func__lookup direct access

                                DAGNodeContainer(DAGNode* x);                                            //!< Vector with one node x
                                DAGNodeContainer(size_t n, DAGNode* x);                                  //!< Vector with n copies of x
                                DAGNodeContainer(size_t n, const std::string& valType);                  //!< Empty vector of length n
                                DAGNodeContainer(const VectorInteger& len, DAGNode* x);                  //!< Array of given dim with copies of x
                                DAGNodeContainer(const VectorInteger& len, const std::string& valType);  //!< Empty array of given dimensions
                                DAGNodeContainer(const DAGNodeContainer& x);                             //!< Copy constructor
                               ~DAGNodeContainer(void);                                                  //!< Destructor

        // Overloaded operators
        DAGNodeContainer&       operator=(const DAGNodeContainer& x);                                    //!< Assignment operator
        DAGNode*&               operator[](const VectorInteger& i);                                      //!< Element access
        DAGNode* const&         operator[](const VectorInteger& i) const;                                //!< Element const access
        DAGNode*&               operator[](const size_t i);                                              //!< Element access
        DAGNode* const&         operator[](const size_t i) const;                                        //!< Element const access

        // Modified vector functions
        ContainerIterator       begin(void) const;                                                       //!< Begin iterator
        ContainerIterator       end(void) const;                                                         //!< End iterator
        void                    clear(void);                                                             //!< Clear vector
        void                    resize(size_t n) { resize (VectorInteger(int(n))); }                     //!< Resize vector
        void                    resize(const VectorInteger& len);                                        //!< Resize container
        size_t                  size(void) const { return nodes.size(); }                                //!< Get size of container

        // Basic utility functions
        DAGNodeContainer*       clone(void) const;                                                       //!< Clone object
        const VectorString&     getClass(void) const;                                                    //!< Get class
        const RbObject*         getStoredValue(void);                                                    //!< Get stored value
        const RbObject*         getValue(void);                                                          //!< Get value
        const RbObject*         getValue(void) const;                                                    //!< Get const value if poss.
        void                    printStruct(std::ostream& o) const;                                      //!< Print struct for user
        void                    printValue(std::ostream& o) const;                                       //!< Print value for user
        void                    setName(const std::string& name);                                        //!< Set name of container and elements
        void                    setValue(RbObject* val);                                                 //!< Set value
        std::string             toString(void) const;                                                    //!< Complete info about object

        // DAG functions
        DAGNodeContainer*       cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                  //!< Clone entire graph
        void                    getAffected(std::set<StochasticNode*>& affected);                        //!< Mark and get affected nodes
        void                    keepAffected(void);                                                      //!< Keep value of affected nodes
        void                    restoreAffected(void);                                                   //!< Restore value of affected nodes
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                            //!< Swap a parent node
        void                    touchAffected(void);                                                     //!< Tell affected nodes value is reset

        // Move function
        MoveSchedule*           getDefaultMoves(void) { return NULL; }                                   //!< Get default moves

        // Element access functions
        int                     getDim(void) const { return int(length.size()); }                        //!< Get dimensions
        const VectorInteger&    getLength(void) const { return length; }                                 //!< Get length in each dim
        std::string             getElementName(const VectorInteger& index) const;                        //!< Get name of element 
        DAGNodeContainer*       getSubContainer(const VectorInteger& index) const;                       //!< Get subcontainer
        const RbObject*         getValElement(const VectorInteger& index) const;                         //!< Get value element
        const DAGNode*          getVarElement(const VectorInteger& index) const;                         //!< Get variable element
        void                    setElement(const VectorInteger& index, RbObject* val);                   //!< Set value element
        void                    setElement(const VectorInteger& index, DAGNode* var);                    //!< Set var element 
        void                    setElementName(const VectorInteger& index, const std::string& name);     //!< Set name of element 
        void                    setLength(const VectorInteger& len);                                     //!< Reorganize container

    private:
        // Utility functions
        size_t                  getOffset(const VectorInteger& index) const;                            //!< Get offset in nodes vector
        void                    update(void);                                                           //!< Update value

        // Give friend class Frame access to elements
        DAGNode*                getVarElement(const VectorInteger& index);                              //!< Get variable element

        // Member variables
        bool                    changed;                                                                //!< Recalculated after touch?
        VectorInteger           length;                                                                 //!< Length in each dimension
	    std::vector<DAGNode*>   nodes;                                                                  //!< Vector of nodes
        VectorString*           names;                                                                  //!< Vector of element names
        ContainerObject*        value;                                                                  //!< Holds value
        ContainerObject*        storedValue;                                                            //!< Stored value
};

#endif
    