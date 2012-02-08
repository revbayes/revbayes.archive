/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values in a model DAG.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */

#ifndef ConstantNode_H
#define ConstantNode_H

#include "DAGNode.h"
#include "RbLanguageObject.h"
#include "RbPtr.h"

class RbObject;
class VectorString;

const std::string ConstantNode_name = "Constant Node";

class ConstantNode : public DAGNode {

public:
    ConstantNode(RbLanguageObject* val);                                                                            //!< Constructor from value
    ConstantNode(const std::string& typeSpec);                                                                      //!< Constructor from language object type
    ConstantNode(const ConstantNode &x);                                                                            //!< Copy constructor


    // Basic utility functions
    ConstantNode*                       clone(void) const;                                                          //!< Clone this object
    const VectorString&                 getClass(void) const;                                                       //!< Get DAG node class vector
    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
    void                                printStruct(std::ostream& o) const;                                         //!< Print struct for user
    void                                printValue(std::ostream& o) const;                                          //!< Print value for user
    std::string                         richInfo(void) const;                                                       //!< Complete info on object

    // ConstantNode functions
    const RbLanguageObject&             getStoredValue(void) const;                                                 //!< Get stored value
    const RbLanguageObject&             getValue(void) const;                                                       //!< Get value 
    RbLanguageObject&                   getValue(void);                                                             //!< Get value 
    const RbLanguageObject*             getValuePtr(void) const;                                                    //!< Get value 

    // DAG functions
    RbPtr<DAGNode>                      cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const;        //!< Clone entire graph
    bool                                isTouched (void) const { return false; }                                    //!< Touched by a move?
   

protected:

    void                                getAffected(std::set<StochasticNode* >& affected);                          //!< Mark and get affected nodes
    void                                keepMe(void);                                                               //!< Keep value of this and affected nodes
    void                                restoreMe(void);                                                            //!< Restore value of this nodes
    void                                touchMe(void);                                                              //!< Tell affected nodes value is reset

private:
    static const TypeSpec               typeSpec;
};

#endif

