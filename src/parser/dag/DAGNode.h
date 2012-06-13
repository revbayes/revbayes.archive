/**
 * @file
 * This file contains the declaration of DAGNode, which is the base
 * class for nodes in a model DAG.
 *
 * @brief Declaration of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface DAGNode
 *
 * $Id$
 */

#ifndef DAGNode_H
#define DAGNode_H

#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "RlValue.h"
#include "TypeSpec.h"
#include "Variable.h"


#include <list>
#include <map>
#include <set>
#include <string>

class Argument;
class InferenceDagNode;
class MethodTable;
class Monitor;
class Plate;
class StochasticNode;
class VariableNode;

class DAGNode : public RbLanguageObject {

    public:
        friend class DeterministicNode;                     //!< Make getAffected etc available for DeterministicNode in some compilers
        friend class StochasticNode;                        //!< Make getAffected etc available for StochasticNode in some compilers

        virtual                                            ~DAGNode(void);                                                                          //!< Virtual destructor

        // Basic utility functions you may want to override
        virtual bool                                        isConst(void) const;                                                                    //!< Is DAG node const value?

        // Basic utility functions you have to override
        virtual DAGNode*                                    clone(void) const = 0;                                                                  //!< Clone this node
        static const std::string&                           getClassName(void);                                                                     //!< Get DAG node class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                                 //!< Get DAG node class type spec
        virtual RlValue<const RbLanguageObject>             getValue(void) const = 0;                                                               //!< Get value (const)
        virtual const RlValue<RbLanguageObject>&            getValue(void) = 0;                                                                     //!< Get value (non-const)
        virtual const TypeSpec&                             getTypeSpec(void) const = 0;                                                            //!< Get the type spec of the instance
        virtual void                                        printStruct(std::ostream& o) const = 0;                                                 //!< Print struct for user
        virtual void                                        printValue(std::ostream& o) const = 0;                                                  //!< Print value for user
    
        // DAG function you have to override
        virtual DAGNode*                                    cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const = 0;                //!< Clone graph
        virtual InferenceDagNode*                           createLeanDag(std::map<const DAGNode*, InferenceDagNode*>& newNodes) const = 0;         //!< Create a lean DAG from this "fat" DAG
        virtual RbPtr<RbLanguageObject>                     executeMethod(const std::string& name, const std::vector<RlValue<const RbLanguageObject> >& args);      //!< Override to map member methods to internal functions
        virtual RbPtr<RbLanguageObject>                     executeSimpleMethod(const std::string& name, const std::vector<const RbLanguageObject *>& args);        //!< Override to map member methods to internal functions
        virtual const MethodTable&                          getMethods(void) const;                                                                 //!< Get member methods (const)
        virtual bool                                        isEliminated(void) const = 0;
        virtual bool                                        isNotInstantiated(void) const = 0;

        // DAG functions you should not have to override
        void                                                addChildNode(VariableNode *c);                                                          //!< Add child node
        void                                                expandAll(void);                                                                        //!< Expand the current value n times. This is equivalent to dropping this node on a plate of size n.
        void                                                getAffectedNodes(std::set<RbPtr<StochasticNode> >& affected);                                 //!< Mark and get affected nodes
        const std::set<VariableNode*>&                      getChildren(void) const;                                                                //!< Return children
        const std::string&                                  getName(void) const;                                                                    //!< get the name
        const std::set<RbPtr<DAGNode> >&                    getParents(void) const;                                                                 //!< Return parents
        const RbPtr<const Plate>&                           getPlate(void) const;                                                                   //!< Get the plate on which this DAG node sits on.
        bool                                                isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const;                       //!< Is node x a parent of the caller in the DAG?
        void                                                keep(void);                                                                             //!< Keep current state of this node and all affected nodes
        size_t                                              numberOfChildren(void) const;                                                           //!< Number of children
        size_t                                              numberOfParents(void) const;                                                            //!< Number of parents
        void                                                printChildren(std::ostream& o) const;                                                   //!< Print children DAG nodes
        void                                                printParents(std::ostream& o) const;                                                    //!< Print children DAG nodes
        void                                                removeChildNode(VariableNode *c);                                                       //!< Remove a child node
        void                                                restore(void);                                                                          //!< Restore value of this and affected nodes
        void                                                setName(const std::string &n) { name = n; }                                             //!< Replace the name of the variable
        void                                                setPlate(const Plate *p);                                                               //!< Set the plate on which this node lives.
        void                                                touch(void);                                                                            //!< Tell affected nodes value is reset


    protected:
                                                            DAGNode(const RbPtr<const Plate> &p);                                                   //!< Constructor of empty node
                                                            DAGNode(const DAGNode& x);                                                              //!< Copy constructor

        virtual void                                        expand(void) = 0;                                                                       //!< Expand the current value n times. This is equivalent to dropping this node on a plate of size n.
        virtual void                                        getAffected(std::set<RbPtr<StochasticNode> >& affected) = 0;                            //!< Mark and get affected nodes
        virtual void                                        keepAffected(void);                                                                     //!< Keep value of affected nodes
        virtual void                                        keepMe(void) = 0;                                                                       //!< Keep value of myself
        virtual void                                        restoreMe(void) = 0;                                                                    //!< Restore value of this nodes
        virtual void                                        restoreAffected(void);                                                                  //!< Restore value of affected nodes recursively
        virtual void                                        touchAffected(void);                                                                    //!< Touch affected nodes (flag for recalculation)
        virtual void                                        touchMe(void) = 0;                                                                      //!< Touch myself (flag for recalculation)
        

        // Member variables keeping track of references
        std::set<VariableNode* >                            children;                                                                               //!< Set of children nodes
        std::set<RbPtr<DAGNode> >                           parents;                                                                                //!< Set of parent nodes
            
        // Member value variables
        std::string                                         name;                                                                                   //!< The name/identifier of the DAG node
        RbPtr<const Plate>                                  plate;                                                                                  //!< The plate on which this DAG node lives.
};

#endif
