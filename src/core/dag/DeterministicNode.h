/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
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

const std::string DeterministicNode_name = "Deterministic Node";

class DeterministicNode : public VariableNode {

public:
                                            DeterministicNode(const std::string& valType);                      //!< Constructor from type
                                            DeterministicNode(RbPtr<RbFunction> func);                          //!< Constructor with function
                                            DeterministicNode(const DeterministicNode& x);                      //!< Copy Constructor
    virtual                                ~DeterministicNode(void);                                            //!< Destructor

    // Utility functions implemented here
    RbPtr<const RbLanguageObject>           getStoredValue(void) const;                                         //!< Get stored value 
    RbPtr<const RbLanguageObject>           getValue(void) const;                                               //!< Get value 
    RbPtr<RbLanguageObject>                 getValue(void);                                                     //!< Get value (non-const to return non-const value)
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user 
    RbPtr<const RbFunction>                 getFunction(void) const;
    RbPtr<RbFunction>                       getFunction(void);

    // Utility functions you have to override
    DeterministicNode*                      clone(void) const;                                                  //!< Clone this node
    const VectorString&                     getClass(void) const;                                               //!< Get class vector
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
    std::string                             richInfo(void) const;                                               //!< Complete info about object

    // DAG functions implemented here
    void                                    getAffected(std::set<RbPtr<StochasticNode> >& affected);            //!< Mark and get affected nodes
    void                                    keep(void);                                                         //!< Update and then keep current value
    void                                    keepAffected(void);                                                 //!< Keep value of affected nodes
    void                                    restoreAffected(void);                                              //!< Restore value of affected nodes
    void                                    swapParentNode(RbPtr<DAGNode> oldP, RbPtr<DAGNode> newP);           //!< Swap a parent node
    void                                    touch(void);                                                        //!< Tell affected nodes value is reset

    // DAG function you have to override
    virtual RbPtr<DAGNode>                  cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const;//!< Clone entire graph

protected:

    // Utility function you have to override
    virtual void                            update(void);                                                       //!< Update value and storedValue

    // Member variable
    bool                                    changed;                                                            //!< True when value updated after touch
    RbPtr<RbFunction>                       function;
    
private:
    static const TypeSpec                   typeSpec;
};

#endif
