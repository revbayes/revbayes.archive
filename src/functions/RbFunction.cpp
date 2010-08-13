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
#include "ContainerIterator.h"
#include "DAGNode.h"
#include "DAGNodeContainer.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Basic constructor */
RbFunction::RbFunction(void) : RbObject() {

    argumentsProcessed = false;
}


/** Copy constructor */
RbFunction::RbFunction(const RbFunction &x) : RbObject(x) {

    argumentsProcessed = x.argumentsProcessed;
    if (x.argumentsProcessed == true) {

        std::vector<bool>::const_iterator i;
        std::vector<DAGNode*>::const_iterator j;
        for (i=x.referenceArgument.begin(), j=x.processedArguments.begin(); i!=x.referenceArgument.end(); i++, j++) {
            referenceArgument.push_back(*i);
            processedArguments.push_back((*j));
        }
    }
}


/** Destructor: delete the processed arguments */
RbFunction::~RbFunction(void) {

    deleteProcessedArguments();
}


/** Assignment operator */
RbFunction& RbFunction::operator=(const RbFunction& x) {

    if (this != &x) {

        deleteProcessedArguments();

        argumentsProcessed = x.argumentsProcessed;
        if (x.argumentsProcessed == true) {

            std::vector<bool>::const_iterator i;
            std::vector<DAGNode*>::const_iterator j;
            for (i=x.referenceArgument.begin(), j=x.processedArguments.begin(); i!=x.referenceArgument.end(); i++, j++) {
                referenceArgument.push_back(*i);
                processedArguments.push_back((*j));
            }
        }
    }

    return (*this);
}


/** Brief info: in case it is not overridden, print some useful info */
std::string RbFunction::briefInfo(void) const {

    std::ostringstream o;
    o << getType() << ": " << (*this);

    return o.str();
}


/** Pointer-based equals comparison */
bool RbFunction::equals(const RbObject* x) const {

    return getClass() == x->getClass();
}


/** Delete processed arguments if not reference arguments */
void RbFunction::deleteProcessedArguments(void) {

    if (argumentsProcessed) {

        std::vector<bool>::iterator i;
        std::vector<DAGNode*>::iterator j;
        for (i=referenceArgument.begin(), j=processedArguments.begin(); i!=referenceArgument.end(); i++, j++) {
            /*
            if ((*j)->numRefs() == 0)
                delete (*j);
            */
        }
    }
    referenceArgument.clear();
    processedArguments.clear();
    argumentsProcessed = false;
}


/** Get class vector describing type of object */
const StringVector& RbFunction::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_name) + RbObject::getClass();
    return rbClass; 
}


/** Execute function with arguments simply passed in as they are given */
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


/** Get new result value */
RbObject* RbFunction::getValue() {

    if (!argumentsProcessed) {
        throw RbException("Arguments were not processed before executing function.");
    }
    
    const RbObject* result = executeOperation(processedArguments);
    if (result == NULL)
        return NULL;
    else
        return result->clone();
}


/** Print value for user */
void RbFunction::printValue(std::ostream& o) const {

    const ArgumentRules& argRules = getArgumentRules();

    o << "<" << getReturnType() << "> function (";
    for (size_t i=0; i<argRules.size(); i++) {
        if (i != 0)
            o << ", ";
        argRules[i]->printValue(o);
    }
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
    const ArgumentRules& theRules = getArgumentRules();

    /* Get the number of argument rules */
    int nRules = int(theRules.size());

    /* Delete previously processed arguments */
    deleteProcessedArguments();

    /* Check the number of arguments and get the final number we expect */
    int numFinalArgs;
    if (nRules > 0 && theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) < nRules)
        numFinalArgs = nRules - 1;
    else
        numFinalArgs = nRules;
    if ( (nRules == 0 || (nRules > 0 && !theRules[nRules-1]->isType(Ellipsis_name))) && int(args.size()) > numFinalArgs)
        return false;

    /* Fill processedArguments with null arguments and resize referenceArgument */
    processedArguments.insert(processedArguments.begin(), numFinalArgs, (DAGNode*)(NULL));
    referenceArgument.resize(numFinalArgs);

    /* Keep track of which arguments we have used */
    std::vector<bool> taken = std::vector<bool>(args.size(), false);


    /*********************  1. Deal with ellipsis  **********************/

    /* Wrap final args into one DAGNodeContainer object.
       TODO: Keep labels, discarded here. */
    if ( nRules > 0 && theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) >= nRules ) {

        int numEllipsisArgs = int(args.size()) - nRules + 1;
        DAGNodeContainer* ellipsisArgs = new DAGNodeContainer(numEllipsisArgs, std::string(RbObject_name));
        ContainerIterator ellipsisIt = ellipsisArgs->begin();

        for (size_t i=nRules-1; i<args.size(); i++) {

            const DAGNode* theDAGNode = args[i].getVariable();
            if ( theDAGNode == NULL )
                return false;
            ellipsisArgs->setElement(ellipsisIt++, theDAGNode->clone());
            taken[i] = true;
        }
        processedArguments[numFinalArgs-1] = ellipsisArgs;
        referenceArgument [numFinalArgs-1] = false;
        if (!theRules[nRules - 1]->isArgValid(ellipsisArgs))
            return false;
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

        /* Check for matches in all rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (int j=0; j<numFinalArgs; j++) {

            if ( args[i].getLabel() == theRules[j]->getLabel() ) {

                if ( theRules[j]->isArgValid(args[i].getVariable()) && processedArguments[j] == NULL ) {
                    taken[i] = true;
                    if (args[i].getVariable()->getName() == "")
                        processedArguments[j] = args[i].getVariable();
                    else
                        processedArguments[j] = args[i].getVariable();
                    referenceArgument[j] = theRules[j]->isWrapperRule();
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
 
        if ( theRules[matchRule]->isArgValid(args[i].getVariable()) ) {
            taken[i] = true;
            if (args[i].getVariable()->getName() == "")
                processedArguments[matchRule] = args[i].getVariable();
            else
                processedArguments[matchRule] = args[i].getVariable();
            referenceArgument[matchRule] = theRules[matchRule]->isWrapperRule();
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
                if ( theRules[j]->isArgValid(args[i].getVariable()) ) {
                    taken[i] = true;
                    if (args[i].getVariable()->getName() == "")
                        processedArguments[j] = args[i].getVariable();
                    else
                        processedArguments[j] = args[i].getVariable();
                    referenceArgument[j] = theRules[j]->isWrapperRule();
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

        if ( !theRules[i]->hasDefault() )
            return false;

        if ( theRules[i]->isWrapperRule() )
            processedArguments[i] = theRules[i]->getDefaultVariablePtr();
        else
            processedArguments[i] = new ConstantNode(theRules[i]->getDefaultValue());
        referenceArgument[i] = theRules[i]->isWrapperRule();
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

        matchScore->push_back(int(j));
    }

    return true;
}


/** Complete info about object */
std::string RbFunction::toString(void) const {

    std::ostringstream o;
    o << getType() << ": " << (*this) << std::endl;
    
    if (argumentsProcessed)
        o << "Arguments processed; there are " << processedArguments.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << processedArguments.size() << " values." << std::endl;
    
    int index=1;
    for (std::vector<DAGNode*>::const_iterator i=processedArguments.begin(); i!=processedArguments.end(); i++, index++) {
        o << " processedArguments[" << index << "] = ";
        (*i)->printValue(o);
        o << std::endl;
    }

    return o.str();
}

