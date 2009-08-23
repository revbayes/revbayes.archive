/*!
 * \file
 * This file contains the declaration of DAGNode, which is the base
 * class for nodes in a model DAG as well as for the nodes in a
 * syntax tree.
 *
 * \brief Declaration of DAGNode
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef DAGNode_H
#define DAGNode_H

#include <string>
#include <set>
#include "RbDataType.h"
#include "RbObject.h"

using namespace std;

/*! This is the abstract base class for nodes in a model graph, a DAG. It is
 *  also the base class for the nodes in a syntax tree.
 *
 *  A DAGNode has the ability to restore itself to a previous state to facilitate
 *  MCMC sampling. A touched flag is used to mark a node for later recalculation.
 *  The node will then be recalculated when its getValue method is called. Note
 *  that only some nodes need to recalculate their value; others have a fixed value
 *  and the base class behavior of just returning the value unconditionally on
 *  a call to getValue is appropriate. When getValue is called, the old value
 *  should be kept in storedValue in all derived classes that have a value that
 *  can change.  A subsequent call to restore will restore the value of the node to
 *  its previous value; a call to keep will simply unmark the node by setting
 *  the touched flag to false.
 *
 *  To facilitate operation on sections of DAGs, this class also implements a
 *  touchDeep function, which touches all affected children nodes until it reaches
 *  a node that says no additional children will be affected; this would typically
 *  be a stochastic node.
 *
 *  Similarly, keep and restore have recursive versions that affect touched
 *  subtrees in the graph. They pass from the root node of the affected subtree
 *  through their parents until they reach nodes that have not been touched.
 */
class DAGNode : public RbObject {

public:
        DAGNode() : value(NULL), storedValue(NULL), touched(false) {}   //!< Default constructor, sets pointers to NULL
	    DAGNode(RbDataType *dt);
	    //DAGNode(RbDataType *dt, std::set<DAGNode*> &p, std::set<DAGNode*> &c);
	    DAGNode(DAGNode &d);
	    ~DAGNode(void);

	void                addChildNode(DAGNode* c) { children.insert(c); }    //!< Add child node
	void                addParentNode(DAGNode* p) { parents.insert(p); }    //!< Do we really need this? The DAGNode can manage its parents on its own - Fredrik
	//void              addParentNode(std::set<DAGNode*> p); REMEMBER TO OVERLOAD THESE ADD/REMOVEs WITH SETS
	set<DAGNode*>&      getChildrenNodes(void) { return children; } //!< Get children nodes
	set<DAGNode*>&      getParentNodes(void) { return parents; }    //!< Get parent nodes
    virtual RbDataType* getValue() { return value; }    //!< Get value; base class just returns value
	virtual bool        isChanged(void) = 0;            //!< What does this do ? -- Fredrik
    bool                isTouched() { return touched; } //!< Is the node marked for recalculation?
    void                keep() { touched = false; }     //!< Keep the current value; do not recalculate
    void                keepRecursive();                //!< Keep the current value recursively
	virtual void        print(void) const = 0;          //!< Print this node
	void                printChildren(void) const;      //!< Print children nodes
	void                printParents(void) const;       //!< Print parent nodes
	void                removeChildNode(DAGNode* c) { children.erase(c); }
	void                removeParentNode(DAGNode* p) { parents.erase(p); }  //!< I think this can go - DAGNodes should be able to manage parentnodes themselves? -- Fredrik
    void                restore();                      //!< Restore node to previous value
    void                restoreRecursive();             //!< Restore nodes recursively to previous value
    void                touch() { touched = true; }     //!< Mark node for recalculation
    void                touchDeep();                    //!< Mark path to root of DAG / syntax tree
	
protected:
	RbDataType*         storedValue;    //!< Holds the previous value
	bool                touched;        //!< Marks node for recalculation
    RbDataType*         value;          //!< Holds the current value

	set<DAGNode*>       children;       //!< Set of children nodes
	set<DAGNode*>       parents;        //!< Set of parent nodes
};

#endif
