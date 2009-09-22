/**
 * @file
 * This file contains the implementation of some functions
 * in RbStandardFxn, which is the abstract base class
 * for REvBayes functions.
 *
 * @brief Implementation of RbStandardFxn (some functions)
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @implements RbFunction (partially)
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "RbStandardFxn.h"
#include "SyntaxConstant.h"

/** Copy constructor */
RbStandardFxn::RbStandardFxn(const RbStandardFxn &s) {

    for (std::vector<SyntaxElement*>::const_iterator i=s.arguments.begin(); i!=s.arguments.end(); i++) {
        arguments.push_back((*i)->copy());
    }
}

/** Destructor deletes arguments */
RbStandardFxn::~RbStandardFxn() {

    for (std::vector<SyntaxElement*>::iterator i=arguments.begin(); i!=arguments.end(); i++) {
        delete (*i);
    }
}

/** Default argument rules (assume function takes no arguments) */
const ArgumentRule RbStandardFxn::argRules[] = {

    ArgumentRule()
};

/** Get DAG nodes in argument expressions */
std::set<DAGNode*> RbStandardFxn::getDAGNodes() const {

    std::set<DAGNode*> theDAGNodes;
    for (std::vector<SyntaxElement*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        std::set<DAGNode*> argDAGNodes = (*i)->getDAGNodes();
        theDAGNodes.insert(argDAGNodes.begin(), argDAGNodes.end());
    }
    return theDAGNodes;
}

/** Set arguments based on argument rules
 *
 *  We use the following logic:
 *
 *  1. Count the number of argument rules.
 *  2. Check argument rules for consistency, in particular
 *     the presence of duplicate labels.
 *  3. Check that the number of provided arguments is
 *     appropriate.
 *  4. Match labeled arguments to argument rules and
 *     argument positons as follows:
 *     - If no label is provided, the argument goes into
 *       the same position it is placed in the list of
 *       provided arguments. If that position is already
 *       filled, it is an error.
 *     - If a label is provided, the argument goes into
 *       the position with that label in its argument
 *       rule. If the label is not found among the
 *       argument rules, or the position is already
 *       filled, it is an error.
 *  5. Fill any remaining position with default values.
 *     If an empty position at this stage does not have
 *     a default value, it is an error.
 *  6. Call setWorkspace() to allow instances to
 *     allocate and set workspace if they are interested.
 */
bool RbStandardFxn::setArguments(std::vector<SyntaxLabeledExpr*> args) {

    /* Get the argument rules */
    const ArgumentRule* theRules = getArgRules();

    /* Get the number of rules */
    int numRules;
    for (numRules=0; theRules[numRules]!=ArgumentRule(); numRules++)
        ;

    /* Check for duplicate labels */
    for (int i=0; i<numRules; i++) {
        for (int j=i+1; j<numRules; j++) {
            if ( theRules[i].getLabel() == theRules[j].getLabel() ) {
                cerr << "Label '" << theRules[i].getLabel() << "' is duplicated among argument rules";
                return false;
            }
        }
    }

    /* Check that the number of provided arguments is adequate */
    if ( numRules < args.size() ) {
        if ( numRules == 0 ) {
            cerr << "Not expecting any arguemnts";
            return false;
        }
        else {
            cerr << "Too many arguments";
            return false;
        }
    }

    /* Match arguments */
    arguments.resize(numRules);
    for(std::vector<SyntaxElement*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
        (*i) = NULL;

    int index=0;
    for (std::vector<SyntaxLabeledExpr*>::iterator i=args.begin(); i!=args.end(); i++, index++) {
        int theArg = -1;
        if ( (*i)->getLabel() == "" ) {
            theArg = index;
        }
        else {
            for (theArg=0; theArg<numRules; theArg++) {
                if ( (*i)->getLabel() == theRules[theArg].getLabel() )
                    break;
            }
            if ( theArg == numRules ) {
                cerr << "Did not expect an argument with label '" << (*i)->getLabel() << "'.";
                arguments.clear();
                return false;
            }
        }
        if ( arguments[theArg] != NULL ) {
            cerr << "Multiple arguments fit argument rule " << index+1;
            arguments.clear();
            return false;
        }
        arguments[theArg] = (*i)->getExpression();
    }

    /* Fill in default values */
    index = 0;
    for (std::vector<SyntaxElement*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ( (*i) == NULL ) {
            (*i) = new SyntaxConstant(theRules[index].getDefault());
            if ( (*i) == NULL ) {
                cerr  << "No default value for argument label '" << theRules[index].getLabel() << "'";
                arguments.clear();
                return false;
            }
        }
    }

    /* Call setWorkspace to set up workspace, if the derived class is interested in this */
    setWorkspace();

    /* Success */
    return true;
}
