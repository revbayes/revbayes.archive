/*
 * AbstractFunction.cpp
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

#include "../datatypes/RbDataType.h"
#include "../main/RbParameter.h"
#include "AbstractFunction.h"

AbstractFunction::AbstractFunction() : RbFunction() {

}

/** Copy constructor */
AbstractFunction::AbstractFunction(const RbStandardFxn &s) {

    for (std::vector<Parameter*>::const_iterator i=s.arguments.begin(); i!=s.arguments.end(); i++) {
        arguments.push_back((*i)->copy());
    }
}

/** Destructor deletes arguments */
AbstractFunction::~AbstractFunction() {

    for (std::vector<Parameter*>::iterator i=arguments.begin(); i!=arguments.end(); i++) {
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
bool RbAbstractFunction::setArguments(std::vector<Parameter*> args) {

    /* Get the argument rules */
    const ArgumentRule* theRules = getArgumentRules();

    /* Get the number of rules */
    int numRules;
    for (numRules=0; theRules[numRules]!=ArgumentRule(); numRules++)
        ;

    /* Check for duplicate labels */
    for (int i=0; i<numRules; i++) {
        for (int j=i+1; j<numRules; j++) {
            if ( theRules[i].getLabel() == theRules[j].getLabel() ) {
            	//TODO throw error
                cerr << "Label '" << theRules[i].getLabel() << "' is duplicated among argument rules";
                return false;
            }
        }
    }

    /* Check that the number of provided arguments is adequate */
    if ( numRules < args.size() ) {
        if ( numRules == 0 ) {
        	//TODO throw error
            cerr << "Not expecting any arguments";
            return false;
        }
        else {
        	//TODO throw error
            cerr << "Too many arguments";
            return false;
        }
    }

    /* Match arguments */
    arguments.resize(numRules);
    // reset the values of the current arguments
    for(std::vector<Parameter*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
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
            	//TODO throw error
                cerr << "Did not expect an argument with label '" << (*i)->getLabel() << "'.";
                arguments.clear();
                return false;
            }
        }
        if ( arguments[theArg] != NULL ) {
        	//TODO throw error
            cerr << "Multiple arguments fit argument rule " << index+1;
            arguments.clear();
            return false;
        }
        arguments[theArg] = (*i)->getExpression();
    }

    /* Fill in default values */
    index = 0;
    for (std::vector<RbParameter*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ( (*i) == NULL ) {
            (*i) = new RbParameter(theRules[index].getDefault());
            if ( (*i) == NULL ) {
            	//TODO throw error
                cerr  << "No default value for argument label '" << theRules[index].getLabel() << "'";
                arguments.clear();
                return false;
            }
        }
    }

    /* Call setWorkspace to set up workspace, if the derived class is interested in this */
    //setWorkspace();

    /* Success */
    return true;
}

bool AbstractFunction::isAccessorFunction() const {
	return false;
}     //!< Is this an accessor fxn?

bool isDistributionFunction() const {
	return false;
} //!< Is this a distribution fxn?

//void setWorkspace() {
//
//}           //!< Do not use workspace by default

