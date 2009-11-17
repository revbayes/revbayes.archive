/**
 * @file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * @brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3.0
 * @interface SyntaxElement
 * @since Version 1.0, 2009-09-02
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "../../main/RbObject.h"
#include "../../dag/DAGNode.h"
#include <iostream>
#include <set>

/* Forward declarations */
class SymbolTable;

/** This is the abstract base class for nodes in the syntax tree. It is derived
 *  from DAGNode, which contains essential functionality like a value member
 *  and a getValue method.
 *
 *  The syntax tree is built up by syntax element nodes. The syntax elements either
 *  have one or more operands, which are themselves syntax elements, or they have
 *  no operands and simply a predefined result of type (pointer to) RbDataType. In
 *  the former case, the elements correspond to interior nodes in the syntax tree
 *  and in the latter case, they correspond to terminal nodes.
 *
 *  If you call getValue on an interior node and the result has not been filled in,
 *  or the node is marked for recalculation, the syntax element should evaluate its
 *  semantic value, causing recursive execution of the syntax subtree rooted on that
 *  element, after which the semantic value is returned.
 *
 *  If you call getValue on a terminal element, the predefined result is simply returned.
 *  Some syntax elements operate only through their side effects, for instance labeled
 *  expressions, and their semantic value is NULL.
 *
 *  A syntax element has the ability to restore itself to a previous state, to speed
 *  up accept and reject steps for deterministic nodes in a model DAG. A touched flag is
 *  used to mark an element for later recalculation. The new value can be kept with keep
 *  or discarded with restore. This functionality is implemented in the DAGNode base
 *  class.
 *
 *  Syntax elements implement a dynamic copy method that makes a copy of part of the
 *  syntax tree for repeated fast evaluation of an expression. The variables in the
 *  expression are kept in a local symbol table if one is provided; otherwise they are
 *  in the global namespace. This functionality is used both in the creation of
 *  stochastic and deterministic DAG nodes, in the former case for distribution functions
 *  and in the latter for deterministic expressions.
 *
 *  Syntax elements also need to implement a function getDataType(), which returns the
 *  a string giving the type of the semantic value without executing the syntax element
 *  to get its semantic value.
 *
 *  The destructor of syntax elements should delete the entire syntax tree rooted at the
 *  element. We accomplish this by calling delete on all the parent nodes in the destructor
 *  of the base class. Derived syntax elements simply need to store their parents in the
 *  vector of parent nodes.
 */
class SyntaxElement : public RbObject {

    public:
            SyntaxElement() : RbObject(), value(NULL), storedValue(NULL), changed(false), touched(false) {}     //!< Default constructor calls base class
	        virtual ~SyntaxElement();           //!< Destructor; delete syntax subtree

        virtual SyntaxElement*      copy(SymbolTable* symbols=NULL) const = 0; //!< Get fast copy of syntax subtree
        virtual std::set<DAGNode*>  getDAGNodes() { return std::set<DAGNode*>(); }  //!< Get DAG Nodes
        virtual const string&       getDataType(void) const = 0;    //!< Get data type of semantic value
        RbDataType*                 getValue();                     //!< Get semantic value
        virtual bool                isConstExpr(void) const = 0;    //!< Is syntax subtree a constant expression?
        bool                        keep();                         //!< Keep current value
        virtual void                print(std::ostream& c) const = 0;       //!< Print some info
        virtual void                printConsole(std::ostream& c) const {}  //!< Print content to console
        bool                        restore();                      //!< Restore previous value
        bool                        touch();                        //!< Mark for recalculation

    protected:
        RbDataType*         storedValue;    //!< Holds the previous value
        RbDataType*         value;          //!< Holds the current value

        bool                changed;        //!< True if value has been recalculated
        bool                touched;        //!< Marks node for recalculation
        
        class SyntaxElement*        parent;             //!< Pointer to the parent syntax element
        class DeterministicNode*    affectedDAGNode;    //!< Pointer to affected deterministic node, if any
};

#endif
