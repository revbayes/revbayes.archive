/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values in a model DAG.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef ConstantNode_H
#define ConstantNode_H

#include "DAGNode.h"
#include "RbLanguageObject.h"

class RbObject;
class VectorString;

class ConstantNode : public DAGNode {

public:
    ConstantNode( void );                                                                                           //!< Constructor
    ConstantNode(RbLanguageObject* val);                                                                            //!< Constructor from value
    ConstantNode(const ConstantNode &x);                                                                            //!< Copy constructor
    virtual                             ~ConstantNode(void);                                                        //!< Destructor

    // Basic utility functions
    ConstantNode*                       clone(void) const;                                                          //!< Clone this object
    static const std::string&           getClassName(void);                                                         //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
    void                                printStruct(std::ostream& o) const;                                         //!< Print struct for user
    void                                printValue(std::ostream& o) const;                                          //!< Print value for user

    // ConstantNode functions
    const RbLanguageObject&             getStoredValue(void) const;                                                 //!< Get stored value
    const RbLanguageObject&             getValue(void) const;                                                       //!< Get value 
    RbLanguageObject&                   getValue(void);                                                             //!< Get value 
    void                                setValue(RbLanguageObject* val);                                            //!< Set the value of the constant node
//    const RbLanguageObject*             getValuePtr(void) const;                                                    //!< Get value 

    // DAG functions
    DAGNode*                            cloneDAG(std::map<const DAGNode*, RbDagNodePtr>& newNodes) const;           //!< Clone entire graph
    bool                                isTouched (void) const { return false; }                                    //!< Touched by a move?
    bool                                isEliminated(void) const;


protected:

    void                                getAffected(std::set<StochasticNode* >& affected);                          //!< Mark and get affected nodes
    void                                keepMe(void);                                                               //!< Keep value of this and affected nodes
    void                                restoreMe(void);                                                            //!< Restore value of this nodes
    void                                touchMe(void);                                                              //!< Tell affected nodes value is reset

private:
    static const TypeSpec               typeSpec;
    RbLanguageObject*                   value;                                                                      //!< Value

};

#endif

