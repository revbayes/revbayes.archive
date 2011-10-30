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
#include "DAGNode.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Basic constructor. */
RbFunction::RbFunction(void) : RbInternal() {

    argsProcessed = false;
}

/** Copy constructor. */
RbFunction::RbFunction(const RbFunction &x) : RbInternal(x) {
    
    for (size_t i=0; i<x.args->size(); i++) {
        const std::string &name = x.args->getName(i);
        RbPtr<const VariableSlot> theSlot = (*x.args)[i];
        args->addVariable(name, RbPtr<VariableSlot>( theSlot->clone() ));
    }
    
    argsProcessed = x.argsProcessed;
}


/** Brief info: in case it is not overridden, print some useful info */
std::string RbFunction::briefInfo(void) const {

    std::ostringstream o;
    printValue( o );

    return o.str();
}


/* Delete processed args */
void RbFunction::clearArguments(void) {

    args->clear();
    
    argsProcessed = false;
}


RbPtr<const Environment> RbFunction::getArguments(void) const {
    return RbPtr<const Environment>( args );
}


RbPtr<Environment> RbFunction::getArguments(void) {
    return args;
}


/** Get class vector describing type of object */
const VectorString& RbFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(RbFunction_name) + RbObject::getClass();
    return rbClass; 
}


/** Print value for user */
void RbFunction::printValue(std::ostream& o) const {

    const RbPtr<const ArgumentRules>& argRules = getArgumentRules();

    o << getReturnType() << " function (";
    for (size_t i=0; i<argRules->size(); i++) {
        if (i != 0)
            o << ", ";
        (*argRules)[i]->printValue(o);
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
 * need type conversion.
 *
 * The evaluateOnce parameter signals whether the function is to be evaluated once,
 * immediately after the call to processArguments, or whether the processed arguments
 * will be used in repeated function calls in a function node. Argument matching is
 * based on current values in the first case, but on the wrapper type in the second.
 *
 * @todo Labels need to be stored for ellipsis arguments.
 *
 * These are the argument matching rules:
 *
 *  1. If the last argument rule is an ellipsis, and it is the kth argument passed
 *     in, then all arguments passed in, from position k to the end, are wrapped
 *     in a single ContainerNode object. These arguments are not matched to any
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
bool  RbFunction::processArguments(const std::vector<RbPtr<Argument> >& passedArgs, RbPtr<VectorInteger> matchScore) {
    
    bool    conversionNeeded;
    int     aLargeNumber = 10000;   // Needs to be larger than the max depth of the class hierarchy

    /*********************  0. Initialization  **********************/

    /* Get the argument rules */
    const RbPtr<const ArgumentRules>& theRules = getArgumentRules();

    /* Get the number of argument rules */
    size_t nRules = theRules->size();

    /* Clear previously processed arguments */
    args->clear();

    /* Check the number of arguments and rules; get the final number of arguments
       we expect and the number of non-ellipsis rules we have */
    size_t numFinalArgs;
    size_t numRegularRules;
    if (nRules > 0 && (*theRules)[nRules-1]->isType(Ellipsis_name)) {
        numRegularRules = nRules - 1;
        if ( passedArgs.size() < nRules )
            numFinalArgs = nRules - 1;
        else
            numFinalArgs = passedArgs.size();
    }
    else {
        numRegularRules = nRules;
        numFinalArgs = nRules;
    }

    /* Check if we have too many arguments */
    if ( passedArgs.size() > numFinalArgs )
        return false;

    /* Fill processedArguments with empty variable slots */
    for (size_t i=0; i<numRegularRules; i++) {
        args->addVariable( (*theRules)[i]->getArgumentLabel(), RbPtr<VariableSlot>( new VariableSlot( (*theRules)[i]->getArgumentLabel(), (*theRules)[i]->getArgumentTypeSpec() ) ) );
    }
    for (size_t i=numRegularRules; i<numFinalArgs; i++) {
        RbPtr<VariableSlot> theEllipsisSlot( new VariableSlot( EmptyString, (*theRules)[nRules-1]->getArgumentTypeSpec() ) );
        args->addVariable( theEllipsisSlot->getLabel(), theEllipsisSlot );
    }
    
    /* Keep track of which arguments we have used, and which argument slots we have filled, and with what passed arguments */
    std::vector<bool>   taken           = std::vector<bool>( passedArgs.size(), false );
    std::vector<bool>   filled          = std::vector<bool>( numFinalArgs, false );
    std::vector<int>    passedArgIndex  = std::vector<int> ( numFinalArgs, -1 );

    /*********************  1. Deal with ellipsis  **********************/

    /* Process ellipsis arguments. If we have an ellipsis, all preceding arguments must be passed in;
       no default values are allowed. Note that the argument labels are discarded here, which is not
       correct. */
    if ( nRules > 0 && (*theRules)[nRules-1]->isType(Ellipsis_name) && passedArgs.size() >= nRules ) {

        for (size_t i=nRules-1; i<passedArgs.size(); i++) {

            RbPtr<Argument> theArgument = passedArgs[i];
            RbPtr<DAGNode> theDAGNode = theArgument->getDagNode();
            if ( theDAGNode == NULL )
                return false;   // This should never happen
            if ( !(*theRules)[nRules-1]->isArgumentValid( theDAGNode, conversionNeeded ) )
                return false;
            
            // add this variable to the argument list
            (*args)[i]->setVariable( theArgument->getVariable() );

            if ( theArgument->getLabel() != "" )
                args->setName( i, theArgument->getLabel() );

            taken[i]          = true;
            filled[i]         = true;
            passedArgIndex[i] = static_cast<int>( i );
        }
    }


    /*********************  2. Do exact matching  **********************/

    /* Do exact matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) {

        /* Test if swallowed by ellipsis; if so, we can quit because the remaining passedArgs will also be swallowed */
        if ( taken[i] )
            break;

        /* Skip if no label */
        if ( passedArgs[i]->getLabel().size() == 0 )
            continue;

        /* Check for matches in all regular rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (size_t j=0; j<numRegularRules; j++) {

            if ( passedArgs[i]->getLabel() == (*theRules)[j]->getArgumentLabel() ) {

                if ( (*theRules)[j]->isArgumentValid(passedArgs[i]->getDagNode(), conversionNeeded) && !filled[j] ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                    
                    // add this variable to the argument list
                    args[j]->setVariable( passedArgs[i]->getVariable() );
                }
                else
                    return false;
            }
        }
    }

 
    /*********************  3. Do partial matching  **********************/

    /* Do partial matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) {

        /* Skip if already matched */
        if ( taken[i] )
            continue;

        /* Skip if no label */
        if ( passedArgs[i]->getLabel().size() == 0 )
            continue;

        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;

        /* Try all rules */
        for (size_t j=0; j<numRegularRules; j++) {

            if ( !filled[j] && (*theRules)[j]->getArgumentLabel().compare(0, passedArgs[i]->getLabel().size(), passedArgs[i]->getLabel()) == 0 ) {
                ++nMatches;
                matchRule = static_cast<int>( j );
            }
        }

        if (nMatches != 1)
            return false;
 
        if ( (*theRules)[matchRule]->isArgumentValid(passedArgs[i]->getDagNode(), conversionNeeded) ) {
            taken[i]                  = true;
            filled[matchRule]         = true;
            passedArgIndex[matchRule] = static_cast<int>( i );
            
            // add this variable to the argument list
            args[matchRule]->setVariable( passedArgs[i]->getVariable() );
        }
        else
            return false;
    }


    /*********************  4. Fill with unused passedArgs  **********************/

    /* Fill in empty slots using the remaining args in order */
    for(size_t i=0; i<passedArgs.size(); i++) {

        /* Skip if already matched */
        if ( taken[i] )
            continue;

        /* Find first empty slot and try to fit argument there */
        for (size_t j=0; j<numRegularRules; j++) {

            if ( filled[j] == false ) {
                RbPtr<DAGNode> argVar = passedArgs[i]->getDagNode();
                if ( (*theRules)[j]->isArgumentValid( argVar, conversionNeeded ) ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                    
                    // add this variable to the argument list
                    args[j]->setVariable( passedArgs[i]->getVariable() );
                    
                    break;
                }
                else
                    return false;
            }
        }
    }

    /*********************  5. Fill with default values  **********************/

    /* Fill in empty slots using default values */
    for(size_t i=0; i<numRegularRules; i++) {

        if ( filled[i] == true )
            continue;

        if ( !(*theRules)[i]->hasDefault() )
            return false;

        args[i]->setVariable( (*theRules)[i]->getDefaultVariable() );
    }

    /*********************  6. Count match score and return  **********************/

    argsProcessed = true;

    if ( matchScore == NULL )
        return true;

    /* Now count the score, first for normal args */
    matchScore->clear();
    size_t argIndex;
    for(argIndex=0; argIndex<numRegularRules; argIndex++) {

        int k = passedArgIndex[argIndex];
        if ( k >= 0 ) {
            
            const VectorString& argClass = passedArgs[k]->getDagNode()->getValue()->getClass();

            size_t j;
            for (j=0; j<argClass.size(); j++)
                if ( argClass[j] == (*theRules)[argIndex]->getArgumentType() )
                    break;

            if ( j == argClass.size() )
                matchScore->push_back(aLargeNumber);    // We needed type conversion for this argument
            else
                matchScore->push_back(int(j));          // No type conversion, score is distance in class vector
        }
        else {
            matchScore->push_back( 0 );                 // We used default value for this argument
        }
    }

    /* ... then for ellipsis args (ellipsis args always passed in order and no default values allowed for preceding args) */
    for ( ; argIndex < numFinalArgs; argIndex++ ) {
    
        
        const VectorString& argClass = args[argIndex]->getDagNodePtr()->getValue()->getClass();
        size_t j;
        for (j=0; j<argClass.size(); j++)
            if ( argClass[j] == (*theRules)[nRules-1]->getArgumentType() )
                break;

        if ( j == argClass.size() )
            matchScore->push_back(aLargeNumber);    // We needed type conversion for this argument
        else
            matchScore->push_back(int(j));          // No type conversion, score is distance in class vector

    }

    return true;
}


/** Complete info about object */
std::string RbFunction::richInfo(void) const {

    std::ostringstream o;
    o << getType() << ": " << std::endl;
    
    if (argsProcessed)
        o << "Arguments processed; there are " << args.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << args.size() << " slots in the frame." << std::endl;
    
    for ( size_t i = 0;  i < args.size(); i++ ) {
        o << " args[" << i << "] = " << *args[i]->getValue() << std::endl;
    }

    return o.str();
}

