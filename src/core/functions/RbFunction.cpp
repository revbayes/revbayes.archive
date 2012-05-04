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
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "Workspace.h"

#include <sstream>


/** Basic constructor. */
RbFunction::RbFunction(void) : RbInternal(), args( ) {

    argsProcessed = false;
}

/** Copy constructor. */
RbFunction::RbFunction(const RbFunction &x) : RbInternal(x), args(  ) {
    
    // the arguments should be copied properly by the derived classes
//    for (size_t i=0; i<x.args.size(); i++) {
//        const Argument& theArg = x.args[i];
//        setArgument(theArg.getLabel(), theArg);
//    }
    
    // here we only get a shallow copy of the vector of arguments
    args = x.args;
    
    argsProcessed = x.argsProcessed;
}


/** Destructor. We need to free the arguments here. */
RbFunction::~RbFunction(void) {
    
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
 * The evaluateOnce parameter MemberObject whether the function is to be evaluated once,
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
bool  RbFunction::checkArguments( const std::vector<Argument>& passedArgs, std::vector<unsigned int>* matchScore) {
    
    /*********************  0. Initialization  **********************/
    
    /* Get the argument rules */
    const ArgumentRules& theRules = getArgumentRules();
    
    /* Get the number of argument rules */
    size_t nRules = theRules.size();
    
    /* Check the number of arguments and rules; get the final number of arguments
     we expect and the number of non-ellipsis rules we have */
    size_t numFinalArgs;
    size_t numRegularRules;
    if (nRules > 0 && theRules[nRules-1].isTypeSpec(Ellipsis::getClassTypeSpec())) {
        numRegularRules = nRules - 1;
        // \TODO: This does not work if some rules have default values and can be ommitted!!!
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
    
    /* Keep track of which arguments we have used, and which argument slots we have filled, and with what passed arguments */
    std::vector<bool>   taken           = std::vector<bool>( passedArgs.size(), false );
    std::vector<bool>   filled          = std::vector<bool>( numFinalArgs, false );
    std::vector<int>    passedArgIndex  = std::vector<int> ( numFinalArgs, -1 );
    
    /*********************  1. Deal with ellipsis  **********************/
    
    /* Process ellipsis arguments. If we have an ellipsis, all preceding arguments must be passed in;
     no default values are allowed. Note that the argument labels are discarded here, which is not
     correct. */
    if ( nRules > 0 && theRules[nRules-1].isTypeSpec(Ellipsis::getClassTypeSpec()) && passedArgs.size() >= nRules ) {
        
        for (size_t i=nRules-1; i<passedArgs.size(); i++) {
            
            const Argument&     theArgument = passedArgs[i];
            const Variable*     theVar      = theArgument.getVariablePtr();
            if ( theVar == NULL )
                return false;   // This should never happen
            if ( !theRules[nRules-1].isArgumentValid( theVar ) )
                return false;
                        
            taken[i]          = true;
            filled[i]         = true;
            passedArgIndex[i] = static_cast<int>( i );
            
            if ( matchScore != NULL) {
                int score = computeMatchScore(passedArgs[i].getVariable().getDagNode(), theRules[nRules-1]);
                matchScore->push_back(score);
            }
        }
    }
    
    
    /*********************  2. Do exact matching  **********************/
    
    /* Do exact matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) {
        
        /* Test if swallowed by ellipsis; if so, we can quit because the remaining passedArgs will also be swallowed */
        if ( taken[i] )
            break;
        
        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
            continue;
        
        /* Check for matches in all regular rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (size_t j=0; j<numRegularRules; j++) {
            
            if ( passedArgs[i].getLabel() == theRules[j].getArgumentLabel() ) {
                
                if ( theRules[j].isArgumentValid(passedArgs[i].getVariablePtr() ) && !filled[j] ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                    
                    if ( matchScore != NULL) {
                        int score = computeMatchScore(passedArgs[i].getVariable().getDagNode(), theRules[j]);
                        matchScore->push_back(score);
                    }
                }
                else
                    return false;
                
                // We got an exact match -> we can skip the other labels for checking
                break;
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
        if ( passedArgs[i].getLabel().size() == 0 )
            continue;
        
        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;
        
        /* Try all rules */
        for (size_t j=0; j<numRegularRules; j++) {
            
            if ( !filled[j] && theRules[j].getArgumentLabel().compare(0, passedArgs[i].getLabel().size(), passedArgs[i].getLabel()) == 0 ) {
                ++nMatches;
                matchRule = static_cast<int>( j );
            }
        }
        
        if (nMatches != 1)
            return false;
        
        if ( theRules[matchRule].isArgumentValid(passedArgs[i].getVariablePtr() ) ) {
            taken[i]                  = true;
            filled[matchRule]         = true;
            passedArgIndex[matchRule] = static_cast<int>( i );
            
            if ( matchScore != NULL) {
                int score = computeMatchScore(passedArgs[i].getVariable().getDagNode(), theRules[matchRule]);
                matchScore->push_back(score);
            }
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
                const Variable* argVar = passedArgs[i].getVariablePtr();
                if ( theRules[j].isArgumentValid( argVar ) ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                    
                    if ( matchScore != NULL) {
                        int score = computeMatchScore(argVar->getDagNode(), theRules[j]);
                        matchScore->push_back(score);
                    }
                    
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
        
        if ( !theRules[i].hasDefault() && !theRules[i].isOptional() )
            return false;
    }
    
    return true;
}


/* Delete processed args */
void RbFunction::clear(void) {

    args.clear();
    
    argsProcessed = false;
    
    clearArguments();
}


/* Delete processed args */
void RbFunction::clearArguments(void) {
    
    // We have already done everything, but derived classes might want to overwrite this function.
    
}


/** Compute the match score between the argument and the argument rule. */
int RbFunction::computeMatchScore(const DAGNode *arg, const ArgumentRule &rule) {
   
    int     aLargeNumber = 10000;   // Needs to be larger than the max depth of the class hierarchy

    const TypeSpec& argClass = arg->getValue().getTypeSpec();
    size_t j = 0;
    const TypeSpec* parent = &argClass;
    do {
        if (*parent == rule.getArgumentTypeSpec() ) {
            return int(j);
        }
        parent = parent->getParentType();
        j++;
    } while (parent != NULL);
    
    return aLargeNumber;    // We needed type conversion for this argument
}


/** Debug info about object */
std::string RbFunction::debugInfo(void) const {
    
    std::ostringstream o;
    o << getTypeSpec() << ": " << std::endl;
    
    if (argsProcessed)
        o << "Arguments processed; there are " << args.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << args.size() << " slots in the frame." << std::endl;
    
    for ( size_t i = 0;  i < args.size(); i++ ) {
        o << " args[" << i << "] = " << args[i].getVariable().getValue() << std::endl;
    }
    
    return o.str();
}

/* 
 * Execute the Function. 
 *
 * This is the default implementation which calls execute with an stl vector of RbObjects. The arguments are converted
 * from DAG nodes to RbObjects.
 *
 */
const RbLanguageObject& RbFunction::execute(void) {
    
    std::vector<const RbObject*> newArgs;
    for (std::vector<Argument>::iterator i = args.begin(); i != args.end(); ++i) {
        newArgs.push_back( &i->getVariable().getValue() );
    }
    return execute( newArgs );
    
}


/* 
 * Execute the Function. 
 *
 * This is the default implementation which calls executeFunction repeatedly for each element of the arguments
 * if one or more arguments are containers.
 *
 */
const RbLanguageObject& RbFunction::execute( const std::vector<const RbObject*> &args ) {
    
    // check each argument if it is a vector and hence the function needs repeated evaluation
    bool repeatedExecution = false;
    size_t size = 0;
    for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
        if ( (*i)->isTypeSpec( Container::getClassTypeSpec() ) ) {
            repeatedExecution = true;
            size = static_cast<const Container*>( *i )->size();
            break;
        }
    }
    
    RbLanguageObject* retVal;
    if ( repeatedExecution ) {
        RbVector<RbLanguageObject>* retValVector = new RbVector<RbLanguageObject>();
        for ( size_t j = 0; j < size; ++j) {
            std::vector<const RbObject*> newArgs;
            for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
                if ( (*i)->isTypeSpec( Container::getClassTypeSpec() ) ) {
                    newArgs.push_back( &static_cast<const Container*>( (*i) )->getElement(j) );
                }
                else {
                    newArgs.push_back( *i );
                    
                }
            }
            // call the execute function now for the single elements
            const RbLanguageObject& singleRetVal = execute(newArgs);
            retValVector->push_back( singleRetVal.clone() );
            // \TODO If the execute functions returns a pointer to the object and the caller owns the object, 
            // then we don't need to copy each time the object.
        }
        
        retVal = retValVector;
    }
    else {
        
        // get the value by executing the internal function
        retVal = executeFunction(args).clone();
    }
    
    return *retVal;
    
}


/* Execute the Function. This is the default implementation which is called by execute(). It will be called 
 * repeatedly for each single value parameter in the multidimensional parameters.
 *
 * If you write your own execute function, you do not need to overwrite this function, otherwise you should.
 */
const RbLanguageObject& RbFunction::executeFunction( const std::vector<const RbObject*> &args ) {
    
    throw RbException("Call to unimplemented function executeFunction(args) in " + getTypeSpec().getType() + ".");
}


const std::vector<Argument>& RbFunction::getArguments(void) const {
    return args;
}


std::vector<Argument>& RbFunction::getArguments(void) {
    return args;
}


/** Get class name of object */
const std::string& RbFunction::getClassName(void) { 
    
    static std::string rbClassName = "Function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}


InferenceFunction* RbFunction::getLeanFunction( void ) const {
    throw RbException(getTypeSpec().getType() + " cannot be used for deterministic nodes in a model.");
}


/** Print value for user */
void RbFunction::printValue(std::ostream& o) const {

    const ArgumentRules& argRules = getArgumentRules();

    o << getReturnType() << " function (";
    for (size_t i=0; i<argRules.size(); i++) {
        if (i != 0)
            o << ", ";
        argRules[i].printValue(o);
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
 * The evaluateOnce parameter MemberObject whether the function is to be evaluated once,
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
void RbFunction::processArguments( const std::vector<Argument>& passedArgs ) {

    /*********************  0. Initialization  **********************/
    /* Get my own copy of the argument vector */
    std::vector<Argument> args = passedArgs;
    
    /* Get the argument rules */
    const ArgumentRules& theRules = getArgumentRules();

    /* Get the number of argument rules */
    size_t nRules = theRules.size();

    /* Clear previously processed arguments */
    this->args.clear();

    /* Check the number of arguments and rules; get the final number of arguments
       we expect and the number of non-ellipsis rules we have */
    size_t numFinalArgs;
    size_t numRegularRules;
    if (nRules > 0 && theRules[nRules-1].isTypeSpec(Ellipsis::getClassTypeSpec())) {
        numRegularRules = nRules - 1;
        // \TODO: This does not work if some rules have default values and can be ommitted!!!
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
        throw RbException("Too many arguments.");
    
    /* Keep track of which arguments we have used, and which argument slots we have filled, and with what passed arguments */
    std::vector<bool>   taken           = std::vector<bool>( passedArgs.size(), false );
    std::vector<bool>   filled          = std::vector<bool>( numFinalArgs, false );
    std::vector<int>    passedArgIndex  = std::vector<int> ( numFinalArgs, -1 );

    /*********************  1. Deal with ellipsis  **********************/

    /* Process ellipsis arguments. If we have an ellipsis, all preceding arguments must be passed in;
       no default values are allowed. */
    // We need to store all the ellipsis argument in a vector and add the at the end.
    std::vector<Argument> ellipsisArgs;
    if ( nRules > 0 && theRules[nRules-1].isTypeSpec(Ellipsis::getClassTypeSpec()) && passedArgs.size() >= nRules ) {

        for (size_t i=nRules-1; i<passedArgs.size(); i++) {

            const Argument&     theArgument     = passedArgs[i];
            const Variable*     theVar          = theArgument.getVariablePtr();
            if ( theVar == NULL )
                throw RbException("Null argument not valid.");
            if ( !theRules[nRules-1].isArgumentValid( theVar, true ) )
                throw RbException("Arguments do not match.");

            taken[i]          = true;
            filled[i]         = true;
            passedArgIndex[i] = static_cast<int>( i );
            
            // add this argument to the ellipsis arguments
            ellipsisArgs.push_back(theArgument);
        }
    }


    /*********************  2. Do exact matching  **********************/

    /* Do exact matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) {

        /* Test if swallowed by ellipsis; if so, we can quit because the remaining passedArgs will also be swallowed */
        if ( taken[i] )
            break;

        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
            continue;

        /* Check for matches in all regular rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (size_t j=0; j<numRegularRules; j++) {

            if ( passedArgs[i].getLabel() == theRules[j].getArgumentLabel() ) {

                if ( theRules[j].isArgumentValid(passedArgs[i].getVariablePtr(), true) && !filled[j] ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                }
                else
                    throw RbException("Arguments do not match.");
                
                // We got an exact match -> we can skip the other labels for checking
                break;
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
        if ( passedArgs[i].getLabel().size() == 0 )
            continue;

        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;

        /* Try all rules */
        for (size_t j=0; j<numRegularRules; j++) {

            if ( !filled[j] && theRules[j].getArgumentLabel().compare(0, passedArgs[i].getLabel().size(), passedArgs[i].getLabel()) == 0 ) {
                ++nMatches;
                matchRule = static_cast<int>( j );
            }
        }

        if (nMatches != 1)
            throw RbException("Argument matches mutliple parameters.");
 
        if ( theRules[matchRule].isArgumentValid(passedArgs[i].getVariablePtr(), true ) ) {
            taken[i]                  = true;
            filled[matchRule]         = true;
            passedArgIndex[matchRule] = static_cast<int>( i );
        }
        else
            throw RbException("Argument is not valid.");
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
                if ( theRules[j].isArgumentValid( passedArgs[i].getVariablePtr(), true ) ) {
                    taken[i]          = true;
                    filled[j]         = true;
                    passedArgIndex[j] = static_cast<int>( i );
                    
                    break;
                }
                else
                    throw RbException("Argument is not valid.");
            }
        }
    }

    /*********************  5. Fill with default values  **********************/

    /* Fill in empty slots using default values */
    for(size_t i=0; i<numRegularRules; i++) {

        if ( filled[i] == true )
            continue;

        // we just leave the optional arguments empty
        if ( !theRules[i].isOptional() ) {
            if ( !theRules[i].hasDefault() )
                throw RbException("No argument found for parameter '" + theRules[i].getArgumentLabel() + "'.");

            const ArgumentRule& theRule = theRules[i];
            RbVariablePtr theVar = theRule.getDefaultVariable().clone();
            theVar->setValueTypeSpec( theRule.getArgumentTypeSpec() );
            passedArgIndex[passedArgs.size()] = static_cast<int>( i );
            args.push_back( Argument("", theVar ) );
        }
    }

    argsProcessed = true;
    
    /*********************  6. Insert arguments into argument list  **********************/
    for (size_t j=0; j<numRegularRules; j++) {
        if ( passedArgIndex[j] >= 0 ) {
            setArgument(theRules[j].getArgumentLabel(), args[passedArgIndex[j]]);
        }
        else {
            std::cerr << "Optional argument for " << theRules[j].getArgumentLabel() << std::endl;
        }
    }
    
    /*********************  7. Insert ellipsis arguments  **********************/
    for (std::vector<Argument>::iterator i = ellipsisArgs.begin(); i != ellipsisArgs.end(); i++) {
        setArgument(i->getLabel(), *i);
    }

}


/** Set a member variable */
void RbFunction::setArgument(const std::string& name, const Argument& arg) {
    // calling the internal method to set the DAG node
    // the derived classes should know how to set their members
//    setArgumentVariable(name, arg.getVariablePtr() );
    
    // make sure that the argument has the correct label
    Argument myArg = Argument( name, arg.getVariablePtr() );
    
    // just add this node to the vector
    args.push_back(myArg);
}





