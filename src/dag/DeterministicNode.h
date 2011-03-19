/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value. They come in three
 * different flavors: FunctionNode, LookupNode and ConverterNode.
 *
 * @brief Declaration of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "VariableNode.h"

#include <string>
#include <vector>

class RbFunction;

class DeterministicNode : public VariableNode {

    public:
        virtual                        ~DeterministicNode(void);                                                    //!< Destructor

        // Utility functions implemented here
        const RbObject*                 getStoredValue(void);                                                           //!< Get stored value
        const RbObject*                 getValue(void);                                                                 //!< Get value
        const RbObject*                 getValue(void) const;                                                           //!< Get const value (if possible)
        const RbObject*                 getValElement(const VectorInteger& index) const;                                //!< Get value element
        void                            printValue(std::ostream& o) const;                                              //!< Print value for user
        void                            setElement(const VectorInteger& index, RbObject* val);                          //!< Set value element (throw error)
        void                            setValue(RbObject* val);                                                        //!< Set value (throw error)

        // Utility functions you have to override
        virtual DeterministicNode*      clone(void) const = 0;                                                          //!< Clone this node
        virtual const VectorString&     getDAGClass(void) const;                                                        //!< Get DAG node class vector
        virtual void                    printStruct(std::ostream& o) const = 0;                                         //!< Print struct for user
        virtual std::string             toString(void) const = 0;                                                       //!< Complete info about object

        // DAG functions implemented here
        void    	                    getAffected(std::set<StochasticNode*>& affected);                               //!< Mark and get affected nodes
        void    	                    keepAffected(void);                                                             //!< Keep value of affected nodes
        void                            restoreAffected(void);                                                          //!< Restore value of affected nodes
        void                            touchAffected(void);                                                            //!< Tell affected nodes value is reset

        // DAG functions you have to override
        virtual DeterministicNode*      cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const = 0;                     //!< Clone entire graph
        virtual bool                    isMutableTo(const DAGNode* newNode) const = 0;                                  //!< Is node mutable to newNode?
        virtual bool                    isMutableTo(const VectorInteger& index, const RbObject* newValue) const = 0;    //!< Is node mutable to contain newValue?
        virtual bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const = 0;    //!< Is parent mutable to newNode?
        virtual void                    mutateTo(DAGNode* newNode) = 0;                                                 //!< Mutate to new node
        virtual DeterministicNode*      mutateTo(const VectorInteger& index, RbObject* newValue) = 0;                   //!< Mutate to contain newValue
        virtual void                    swapParentNode(DAGNode* oldP, DAGNode* newP) = 0;                               //!< Swap a parent node

        // Get default moves
        MoveSchedule*                   getDefaultMoves(void);                                                          //!< Return default moves

    protected:
                                        DeterministicNode(const std::string& valType);                                     //!< Constructor from type

        // Utility function you have to override
        virtual void                    update(void) = 0;                                                               //!< Update value and storedValue

        // Member variables
        bool                            changed;                                                                        //!< True when value updated after touch
        RbObject*                       value;                                                                          //!< Current value
        RbObject*                       storedValue;                                                                    //!< Stored value
};

#endif
