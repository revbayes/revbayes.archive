/**
 * @file
 * This file contains the declaration of InferenceDagNode, which is the base
 * class for nodes in a model DAG.
 *
 * @brief Declaration of InferenceDagNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 * @interface DAGNode
 *
 * $Id: DAGNode.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef InferenceDagNode_H
#define InferenceDagNode_H

#include <set>
#include <string>

class StochasticInferenceNode;
class VariableInferenceNode;


class InferenceDagNode {
    
public:
    
    virtual                                            ~InferenceDagNode(void);                                                          //!< Virtual destructor
    
    
    // Basic utility functions you have to override
    
    // DAG function you have to override    
    
    // DAG functions you should not have to override
    void                                                addChildNode(VariableInferenceNode *c);                                         //!< Add child node
    void                                                getAffectedNodes(std::set<VariableInferenceNode* >& affected);                //!< get affected nodes
    const std::set<VariableInferenceNode*>&             getChildren(void) const { return children; }                                    //!< Return children
    const std::string&                                  getName(void) const;                                                            //!< get the name
    const std::set<InferenceDagNode*>&                  getParents(void) const;                                                         //!< Return parents
//    bool                                                isParentInDAG(const InferenceDagNode* x, std::list<InferenceDagNode*>& done) const;//!< Is node x a parent of the caller in the DAG?
    void                                                keep(void);                                                                     //!< Keep current state of this node and all affected nodes
    size_t                                              numberOfChildren(void) const { return children.size(); }                        //!< Number of children
    size_t                                              numberOfParents(void) const { return parents.size(); }                          //!< Number of parents
//    void                                                printChildren(std::ostream& o) const;                                           //!< Print children DAG nodes
//    void                                                printParents(std::ostream& o) const;                                            //!< Print children DAG nodes
    void                                                removeChildNode(VariableInferenceNode *c);                                      //!< Remove a child node
    void                                                restore(void);                                                                  //!< Restore value of this and affected nodes
    void                                                setName(const std::string &n) { name = n; }                                     //!< Replace the name of the variable
    void                                                touch(void);                                                                    //!< Tell affected nodes value is reset
    
    
protected:
    InferenceDagNode(void);                                                                                                             //!< Constructor of empty node
    InferenceDagNode(const InferenceDagNode& x);                                                                                        //!< Copy constructor
    
    virtual void                                        getAffected(std::set<VariableInferenceNode* >& affected) = 0;                 //!< get affected nodes
    virtual void                                        keepAffected(void);                                                             //!< Keep value of affected nodes
    virtual void                                        keepMe(void) = 0;                                                               //!< Keep value of myself
    virtual void                                        restoreAffected(void);                                                          //!< Restore value of affected nodes recursively
    virtual void                                        restoreMe(void) = 0;                                                            //!< Restore value of this nodes
    virtual void                                        touchAffected(void);                                                            //!< Touch affected nodes (flag for recalculation)
    virtual void                                        touchMe(void) = 0;                                                              //!< Touch myself (flag for recalculation)
    
    
    // Member variables keeping track of references
    std::set<VariableInferenceNode* >                   children;                                                                       //!< Set of children nodes
    std::set<InferenceDagNode*>                         parents;                                                                        //!< Set of parent nodes
        
    // Member value variables
    std::string                                         name;                                                                           //!< The name/identifier of the DAG node
};

#endif
