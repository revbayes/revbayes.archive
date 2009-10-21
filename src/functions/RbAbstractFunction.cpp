/*
 * RbAbstractFunction.cpp
 *
 *  Created on: 17 sep 2009
 *      Author: Sebastian
 */
/**
 * @file
 * This file contains the implementation of some functions in
 * RbFunction, the interface for functions in REvBayes.
 *
 * @brief Implementation of RbAbstractFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include <list>

#include "RbDataType.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbAbstractFunction.h"
#include "RbException.h"
#include "Argument.h"

RbAbstractFunction::RbAbstractFunction() : RbFunction() {

}

/** Copy constructor */
RbAbstractFunction::RbAbstractFunction(const RbAbstractFunction &s) {

    for (std::vector<DAGNode*>::const_iterator i=s.arguments.begin(); i!=s.arguments.end(); i++) {
        arguments.push_back((DAGNode*)(*i)->clone());
    }
}

/** Destructor deletes arguments */
RbAbstractFunction::~RbAbstractFunction() {

    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++) {
        delete (*i);
    }
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
bool RbAbstractFunction::setArguments(std::vector<Argument*> args) {

    /* Get the argument rules */
    const ArgumentRule* theRules = getArgumentRules();

    /* Get the number of rules */
    //int numRules = sizeof(theRules)/sizeof(theRules[0]); not good because could have zero rules
    int numRules = getNumberOfRules();
    //for (numRules=0; theRules[numRules]!=ArgumentRule(); numRules++)
    //    ;

    /* Check for duplicate labels */
    for (int i=0; i<numRules; i++) {
        for (int j=i+1; j<numRules; j++) {
            if ( theRules[i].getLabel() == theRules[j].getLabel() ) {
                std::string message = "Label '" + theRules[i].getLabel() + "' is duplicated among argument rules";
                RbException e;
                e.setMessage(message);
                throw e;
                return false;
            }
        }
    }

    /* Check that the number of provided arguments is adequate */
    if ( numRules < args.size() ) {
        if ( numRules == 0 ) {
            std::string message = "Not expecting any arguments";
            RbException e;
            e.setMessage(message);
            throw e;
            return false;
        }
        else {
            std::string message = "Too many arguments";
            RbException e;
            e.setMessage(message);
            throw e;
            return false;
        }
    }

    /* Match arguments */
    arguments.resize(numRules);
    // reset the values of the current arguments
    for(std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
        (*i) = NULL;

    int index=0;
    for (std::vector<Argument*>::iterator i=args.begin(); i!=args.end(); i++, index++) {
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
                std::string message = "Did not expect an argument with label '" + (*i)->getLabel() + "'.";
                RbException e;
                e.setMessage(message);
                arguments.clear();
                throw e;
                return false;
            }
        }
        if ( arguments[theArg] != NULL ) {
            std::string message = "Multiple arguments fit argument rule " + index+1;
            RbException e;
            e.setMessage(message);
            arguments.clear();
            throw e;
            return false;
        }
        arguments[theArg] = (*i)->getDAGNode();
    }

    /* Fill in default values */
    index = 0;
    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ( (*i) == NULL ) {
            (*i) = new DeterministicNode(&(theRules[index].getDefaultValue()));
            if ( (*i) == NULL ) {
                std::string message = "No default value for argument label '" + theRules[index].getLabel() + "'";
                RbException e;
                e.setMessage(message);
                arguments.clear();
                throw e;
                return false;
            }
        }
    }

    /* Call setWorkspace to set up workspace, if the derived class is interested in this */
    //setWorkspace();

    /* Success */
    return true;
}

bool RbAbstractFunction::isAccessorFunction() const {
	return false;
}     //!< Is this an accessor fxn?

bool RbAbstractFunction::isDistributionFunction() const {
	return false;
} //!< Is this a distribution fxn?

//void setWorkspace() {
//
//}           //!< Do not use workspace by default

