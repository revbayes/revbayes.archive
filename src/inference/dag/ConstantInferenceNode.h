/**
 * @file
 * This file contains the declaration of ConstantInferenceNode, which is derived
 * from DAGNode. ConstantInferenceNode is used for DAG nodes holding constant
 * values in a model DAG.
 *
 * This class is a template because we store the values on pointers tp basic c++ types
 * for faster access (no indirection and casting).
 *
 * @brief Declaration of ConstantInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-16, version 1.0
 * @extends DAGNode
 *
 * $Id: ConstantInferenceNode.h 1327 2012-03-10 11:55:11Z hoehna $
 */

#ifndef ConstantInferenceNode_H
#define ConstantInferenceNode_H

#include "InferenceDagNode.h"


class ConstantInferenceNode : public InferenceDagNode {
    
public:
    ConstantInferenceNode(const RbValue<void*> &val);                                                               //!< Constructor
    ConstantInferenceNode(const ConstantInferenceNode &x);                                                          //!< Copy constructor
    virtual                             ~ConstantInferenceNode(void);                                               //!< Destructor
        
    
    // DAG functions
//    DAGNode*                            cloneDAG(std::map<const DAGNode*, RbDagNodePtr>& newNodes) const;           //!< Clone entire graph
    bool                                isTouched (void) const { return false; }                                    //!< Touched by a move?
    bool                                isEliminated(void) const;
    bool                                isNotInstantiated(void) const;
    
    // temporary stuff
//    void                                printStruct(std::ostream &o) const;
    
protected:
    
    void                                getAffected(std::set<StochasticInferenceNode* >& affected);                  //!< Mark and get affected nodes
    void                                keepMe(void);                                                               //!< Keep value of this and affected nodes
    void                                restoreMe(void);                                                            //!< Restore value of this nodes
    void                                touchMe(void);                                                              //!< Tell affected nodes value is reset
        
};




#endif

