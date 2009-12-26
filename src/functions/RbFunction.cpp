/**
 * @file
 * This file contains the implementation of some functions in
 * RbFunction, the interface fnd abstract base class for RevBayes
 * functions.
 *
 * @brief Partial implementation of RbFunction
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

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNodeContainer.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "RbObjectWrapper.h"
#include "RbUndefined.h"
#include "StringVector.h"

#include <sstream>

/** Basic constructor */
RbFunction::RbFunction(void) : RbObject() {

    argumentsProcessed = false;
}


/** Copy constructor; we do not want to copy the processed arguments */
RbFunction::RbFunction(const RbFunction &fn) : RbObject() {

    argumentsProcessed = false;    
}


/** Brief ino on the function */
std::string RbFunction::briefInfo(void) const {

    std::ostringstream o;
    o << "RbFunction: ";
    printValue(o);

    return o.str();
}


/** Get class vector describing type of object */
const StringVector& RbFunction::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_name) + RbObject::getClass();
	return rbClass; 
}


/** Pointer-based equals comparison */
bool RbFunction::equals(const RbObject* obj) const {

	const StringVector& a = obj->getClass();
	const StringVector& b = getClass();
	return (a == b);
}


/** Simple execute function with arguments simply passed in as they are given */
const RbObject* RbFunction::execute(const std::vector<Argument>& args) {

    if (processArguments(args) == false)
        throw RbException("Arguments do not match formals.");

	return executeOperation(processedArguments);
}


/** Execute function for repeated evaluation after arguments have been set */
const RbObject* RbFunction::execute() {

    if (!argumentsProcessed) {
        throw RbException("Arguments were not processed before executing function.");
    }
    const RbObject* result = executeOperation(processedArguments);
    return result;
}


/** Print value for user */
void RbFunction::printValue(std::ostream& o) const {

    const ArgumentRule** argRules = getArgumentRules();

    o << getReturnType() << " function (";
    for (int i=0; argRules[i]!=NULL; i++)
        argRules[i]->printValue(o);
    o << ")";    
}


/**
 * @brief Process arguments
 *
 * This function processes arguments based on argument rules. First it deletes
 * any previously stored arguments. If the matching of the new arguments
 * succeeds, the processedArguments will be set to the new vector of processed
 * arguments and the function returns true. Any subsequent calls to execute()
 * will then use the processed arguments. You can also call the function with
 * the arguments directly, in which case processArguments will be called first
 * before the operation is actually performed.
 *
 * In matching arguments to argument rules, we use the same rules as in R with
 * the addition that types are also used in the matching process, after arguments
 * have been reordered as in R. The FunctionTable ensure that all argument rules
 * are distinct. However, several functions can nevertheless match the same
 * arguments because of the inheritance hierarchy. In these clases, the closest
 * match is chosen based on the first argument, then on the second, etc.
 *
 * @todo This function really needs to return a match score based on how closely
 *       the arguments match the rules, with 0 being perfect match, 1 being a
 *       match to an immediate base class type, 2 a match to a grandparent class,
 *       etc. Also, ellipsis arguments are not dealt with yet. -- Fredrik
 *
 * These are the argument matching rules:
 *
 *  1. If the last argument rule is an ellipsis, and it is the kth argument passed
 *     in, then all arguments passed in, from position k to the end, are wrapped
 *     in a single DAGNodeContainer object. These arguments are not matched to any
 *     rules.
 *  2. The remaining arguments are matched to labels using exact matching. If the
 *     type does not match the type of the rule, it is an error.
 *  3. The remaining arguments are matched to any remaining slots using partial
 *     matching. If there is ambiguity or the types do not match, it is an error.
 *  4. The remaining arguments are used for the empty slots in the order they were
 *     passed in. If the types do not match, it is an error.
 *  5. Any remaining empty slots are filled with default values stored in the argument
 *     rules (we use copies of the values, of course).
 *  6. If there are still empty slots, the arguments do not match the rules.
 */
bool  RbFunction::processArguments(const std::vector<Argument>& args, IntVector* matchScore) {

    /*********************  0. Initialization  **********************/

	/* Get the argument rules */
	const ArgumentRule** theRules = getArgumentRules();
	
	/* Get the number of argument rules */
	int nRules = 0;
	while ( theRules[nRules++] != NULL )
		;

	// Comment Sebastian: I cannot explain why, but we counted to many rules
	nRules--;

    /* Forget previously processed arguments; we own the object wrappers and need to delete them */
    for (std::vector<RbObjectWrapper*>::iterator i= processedArguments.begin();
        i!=processedArguments.end(); i++)
        delete (*i);
    processedArguments.clear();
    argumentsProcessed = false;

    /* Check the number arguments and get the final number we expect */
    int numFinalArgs;
    if ( theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) < nRules)
        numFinalArgs = nRules - 1;
    else
        numFinalArgs = nRules;
    if ( !theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) > numFinalArgs )
        return false;

    /* Fill processedArguments with null arguments */
    processedArguments.insert(processedArguments.begin(), numFinalArgs, (RbObjectWrapper*)(NULL));

    /* Keep track of which arguments we have used */
    std::vector<bool> taken = std::vector<bool>(args.size(), false);


    /*********************  1. Deal with ellipsis  **********************/

    /* Wrap final args into one DAGNodeContainer object.
       @todo Keep labels, discarded here. We should probably introduce a
             new kind of wrapper for ellipsis arguments, so we can take
             DAG node containers as arguments also to variable-argument formals.
    */
    if ( theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) >= nRules ) {

        int numEllipsisArgs = args.size() - nRules + 1;
        DAGNodeContainer* ellipsisArgs = new DAGNodeContainer(numEllipsisArgs, std::string(RbObject_name));
        ContainerIterator ellipsisIndex = (*ellipsisArgs).begin();

        for (size_t i=nRules-1; i<args.size(); i++) {

            const DAGNode* theDAGNode = (const DAGNode*)(args[i].getWrapper());
            if ( theDAGNode == NULL )
                return false;
            (*ellipsisArgs)[ellipsisIndex++] = theDAGNode->clone();
            taken[i] = true;
        }
        processedArguments[numFinalArgs-1] = ellipsisArgs;
    }


    /*********************  2. Do exact matching  **********************/

    /* Do exact matching of labels */
    for(size_t i=0; i<args.size(); i++) {

        /* Test if swallowed by ellipsis; if so, we can quit because the remaining args will also be swallowed */
        if ( taken[i] )
            break;

        /* Skip if no label */
        if ( args[i].getLabel().size() == 0 )
            continue;

        /* Check for matches in all rules (we assume that all labels are unique; this is checked by the FunctionTable) */
        for (int j=0; j<numFinalArgs; j++) {

            if ( args[i].getLabel() == theRules[j]->getLabel() ) {

                if ( theRules[j]->isArgValid(args[i].getWrapper()) && processedArguments[j] == NULL ) {
                    taken[i] = true;
                    processedArguments[j] = args[i].getWrapper()->clone();
                }
                else
                    return false;
            }
        }
    }

 
    /*********************  3. Do partial matching  **********************/

    /* Do partial matching of labels */
    for(size_t i=0; i<args.size(); i++) {

        /* Skip if already matched */
        if ( taken[i] )
            continue;

        /* Skip if no label */
        if ( args[i].getLabel().size() == 0 )
            continue;

        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;

        /* Try all rules */
        for (int j=0; j<numFinalArgs; j++) {

            if ( processedArguments[j] == NULL &&
                 theRules[j]->getLabel().compare(0, args[i].getLabel().size(), args[i].getLabel()) == 0 ) {
                ++nMatches;
                matchRule = j;
            }
        }

        if (nMatches != 1)
            return false;
 
        if ( theRules[matchRule]->isArgValid(args[i].getWrapper()) ) {
            taken[i] = true;
            processedArguments[matchRule] = args[i].getWrapper()->clone();
        }
        else
            return false;
    }


    /*********************  4. Fill with unused args  **********************/

    /* Fill in empty slots using the remaining arguments in order */
    for(size_t i=0; i<args.size(); i++) {

        /* Skip if already matched */
        if ( taken[i] )
            continue;

        /* Find first empty slot and try to fit argument there */
        for (int j=0; j<numFinalArgs; j++) {

            if ( processedArguments[j] == NULL ) {
                if ( theRules[j]->isArgValid(args[i].getWrapper()) ) {
                    taken[i] = true;
                    processedArguments[j] = args[i].getWrapper()->clone();
                    break;
                }
                else
                    return false;
            }
        }
    }

    /*********************  5. Fill with default values  **********************/

    /* Fill in empty slots using default values */
    for(int i=0; i<numFinalArgs; i++) {

        if ( processedArguments[i] != NULL )
            continue;

        if ( theRules[i]->getDefaultValue().isType(RbUndefined_name) )
            return false;

        processedArguments[i] = new ConstantNode(theRules[i]->getDefaultValue().clone());
    }

    /*********************  6. Count match score and return  **********************/

    argumentsProcessed = true;

    if ( matchScore == NULL )
        return true;

    /* Now count the score */
    matchScore->clear();
    for(int i=0; i<numFinalArgs; i++) {

        const StringVector& argClass = processedArguments[i]->getClass();
        size_t j;
        for (j=0; j<argClass.size(); j++)
            if ( argClass[j] == theRules[i]->getType() )
                break;

        matchScore->push_back(j);
    }

    return true;
}


/** Complete info about object */
std::string RbFunction::toString(void) const {

    std::ostringstream o;
    o << "RbFunction: ";
    printValue(o);
    o << std::endl;
    
    if (argumentsProcessed)
        o << "Arguments processed; there are " << processedArguments.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << processedArguments.size() << " values." << std::endl;
    
    int index=1;
    for (std::vector<RbObjectWrapper*>::const_iterator i=processedArguments.begin(); i!=processedArguments.end(); i++, index++) {
        o << " processedArguments[" << index << "] = ";
        (*i)->getValue()->printValue(o);
        o << std::endl;
    }

    return o.str();
}

