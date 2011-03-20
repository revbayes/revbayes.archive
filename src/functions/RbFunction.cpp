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
 * @author The RevBayes core team
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
#include "DAGNodePlate.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Basic constructor */
RbFunction::RbFunction(void) : RbObject() {

    argumentsProcessed = false;
}


/** Copy constructor */
RbFunction::RbFunction(const RbFunction &x) : RbObject(x) {

    argumentsProcessed = x.argumentsProcessed;
    if (x.argumentsProcessed == true) {

        std::vector<DAGNode*>::const_iterator i;
        for (i=x.processedArguments.begin(); i!=x.processedArguments.end(); i++) {
            processedArguments.push_back((*i));
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

            std::vector<DAGNode*>::const_iterator i;
            for (i=x.processedArguments.begin(); i!=x.processedArguments.end(); i++) {
                processedArguments.push_back((*i));
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


/** Delete processed arguments */
void RbFunction::deleteProcessedArguments(void) {

    // @todo This does not work. Use variable slots to manage memory correctly.
    if (argumentsProcessed) {
        for ( std::vector<DAGNode*>::iterator i=processedArguments.begin(); i!=processedArguments.end(); i++ ) {
            /* 
            if ( (*i)->numRefs() == 0 )
                delete ( (*i) );
            */
        }
    }
    processedArguments.clear();
    argumentsProcessed = false;
}


/** Execute function with arguments simply passed in as they are given */
DAGNode* RbFunction::execute(const std::vector<Argument>& args) {

    if (processArguments(args) == false)
        throw RbException("Arguments do not match formals.");

    return executeOperation(processedArguments);
}


/** Execute function for repeated evaluation after arguments have been set */
DAGNode* RbFunction::execute() {

    if (!argumentsProcessed) {
        throw RbException("Arguments were not processed before executing function.");
    }
    
    return executeOperation(processedArguments);
}


/** Get class vector describing type of object */
const VectorString& RbFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(RbFunction_name) + RbObject::getClass();
    return rbClass; 
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
 * The function returns a match score based on how closely the arguments match the
 * rules, with 0 being perfect match, 1 being a match to an immediate base class type,
 * 2 a match to a grandparent class, etc. A large number is used for arguments that
 * need type conversion
 *
 * @todo Ellipsis arguments are not quite dealt with properly yet; most importantly,
 *       the labels are stripped off here although they should be kept.
 *
 * These are the argument matching rules:
 *
 *  1. If the last argument rule is an ellipsis, and it is the kth argument passed
 *     in, then all arguments passed in, from position k to the end, are wrapped
 *     in a single DAGNodePlate object. These arguments are not matched to any
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
bool  RbFunction::processArguments(const std::vector<Argument>& args, VectorInteger* matchScore) {

    bool    needConversion;
    int     aLargeNumber = 10000;   // Needs to be larger than the max depth of the class hierarchy

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

    /* Fill processedArguments with null arguments and resize convertedArgument */
    processedArguments.insert(processedArguments.begin(), numFinalArgs, (DAGNode*)(NULL));

    /* Keep track of which arguments we have used */
    std::vector<bool> taken = std::vector<bool>(args.size(), false);


    /*********************  1. Deal with ellipsis  **********************/

    /* Wrap final args into one DAGNodePlate object.
       TODO: Keep labels, discarded here. */
    if ( nRules > 0 && theRules[nRules-1]->isType(Ellipsis_name) && int(args.size()) >= nRules ) {

        int numEllipsisArgs = int(args.size()) - nRules + 1;
        DAGNodePlate* ellipsisArgs = new DAGNodePlate(numEllipsisArgs, theRules[nRules-1]->getArgType());
        ContainerIterator ellipsisIt = ellipsisArgs->begin();

        for (size_t i=nRules-1; i<args.size(); i++) {

            const DAGNode* theDAGNode = args[i].getVariable();
            if ( theDAGNode == NULL )
                return false;
            if ( !Workspace::globalWorkspace().isXOfTypeY(theDAGNode->getValueType(), ellipsisArgs->getValueType()) )
                return false;
            ellipsisArgs->setElement(ellipsisIt++, theDAGNode->clone());
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

        /* Check for matches in all rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (int j=0; j<numFinalArgs; j++) {

            if ( args[i].getLabel() == theRules[j]->getArgLabel() ) {

                if ( theRules[j]->isArgValid(args[i].getVariable(), needConversion) && processedArguments[j] == NULL ) {
                    taken[i] = true;
                    if ( needConversion )
                        processedArguments[j] = theRules[j]->convert( args[i].getVariable() );
                    else
                        processedArguments[j] = args[i].getVariable();
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
                 theRules[j]->getArgLabel().compare(0, args[i].getLabel().size(), args[i].getLabel()) == 0 ) {
                ++nMatches;
                matchRule = j;
            }
        }

        if (nMatches != 1)
            return false;
 
        if ( theRules[matchRule]->isArgValid(args[i].getVariable(), needConversion) ) {
            taken[i] = true;
            if ( needConversion )
                processedArguments[matchRule] = theRules[matchRule]->convert( args[i].getVariable() );
            else
                processedArguments[matchRule] = args[i].getVariable();
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
                if ( theRules[j]->isArgValid(args[i].getVariable(), needConversion) ) {
                    taken[i] = true;
                    if ( needConversion )
                        processedArguments[j] = theRules[j]->convert( args[i].getVariable() );
                    else
                        processedArguments[j] = args[i].getVariable();
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

        if ( theRules[i]->isReference() )
            processedArguments[i] = theRules[i]->getDefaultReference();
        else
            processedArguments[i] = theRules[i]->getDefaultVariable();
    }

    /*********************  6. Count match score and return  **********************/

    argumentsProcessed = true;

    if ( matchScore == NULL )
        return true;

    /* Now count the score, first for normal arguments */
    matchScore->clear();
    int argIndex;
    for(argIndex=0; argIndex<numFinalArgs; argIndex++) {

        if ( theRules[argIndex]->isType(Ellipsis_name) )
            break;

        const VectorString& argClass = processedArguments[argIndex]->getValue()->getClass();
        size_t j;
        for (j=0; j<argClass.size(); j++)
            if ( argClass[j] == theRules[argIndex]->getArgType() )
                break;

        if ( j == argClass.size() )
            matchScore->push_back(aLargeNumber);    // We needed type conversion for this argument
        else
            matchScore->push_back(int(j));          // No type conversion, score is distance in class vector
    }

    /* ... then for ellipsis arguments */
    if ( argIndex < numFinalArgs ) {
    
        DAGNodePlate* container = (DAGNodePlate*)(processedArguments[argIndex]);

        for (ContainerIterator it=container->begin(); it!=container->end(); it++) {

            const VectorString& argClass = container->getValElement(it)->getClass();
            size_t j;
            for (j=0; j<argClass.size(); j++)
                if ( argClass[j] == theRules[argIndex]->getArgType() )
                    break;

            if ( j == argClass.size() )
                matchScore->push_back(aLargeNumber);    // We needed type conversion for this argument
            else
                matchScore->push_back(int(j));          // No type conversion, score is distance in class vector
        }
    }

    return true;
}


/** Complete info about object */
std::string RbFunction::richInfo(void) const {

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

