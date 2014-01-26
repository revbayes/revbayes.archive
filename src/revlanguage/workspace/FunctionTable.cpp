/**
 * @file
 * This file contains the implementation of FunctionTable, which is
 * used to hold global functions in the base environment (the global
 * workspace) and the user workspace.
 *
 * @brief Implementation of FunctionTable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "FunctionTable.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RlFunction.h"
#include "RbUtil.h"
#include "StochasticNode.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor, empty table with or without parent */
FunctionTable::FunctionTable(FunctionTable* parent) : table(), parentTable(parent) {

}


/** Copy constructor */
FunctionTable::FunctionTable(const FunctionTable& x) {
    
    for (std::multimap<std::string, Function *>::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
    {
        table.insert(std::pair<std::string, Function *>( (*i).first, ( (*i).second->clone() )));
    }
    parentTable = x.parentTable;
}


/** Destructor */
FunctionTable::~FunctionTable(void) {

    clear();
    
}

/** Assignment operator */
FunctionTable& FunctionTable::operator=(const FunctionTable& x) {

    if (this != &x) 
    {

        clear();
        for (std::multimap<std::string, Function *>::const_iterator i=x.table.begin(); i!=x.table.end(); i++) 
        {
            table.insert(std::pair<std::string, Function *>((*i).first, ( (*i).second->clone() ) ) );
        }
        
        parentTable = x.parentTable;
    }

    return (*this);
}


/** Add function to table */
void FunctionTable::addFunction(const std::string name, Function *func) {
    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;

    retVal = table.equal_range(name);
    for (std::multimap<std::string, Function *>::iterator i=retVal.first; i!=retVal.second; i++) 
    {
        if (!isDistinctFormal(i->second->getArgumentRules(), func->getArgumentRules())) 
        {
            std::ostringstream msg;
            msg << name << " =  ";
            i->second->printValue(msg);
            msg << " cannot overload " << name << " = ";
            func->printValue(msg);
            
            // throw the error message
            throw RbException(msg.str());
        }
    }
    table.insert(std::pair<std::string, Function* >(name, func));
    
}


/** Clear table */
void FunctionTable::clear(void) {
    
    table.clear();
    
}


FunctionTable* FunctionTable::clone( void ) const {
    
    return new FunctionTable( *this );
}


/** Erase function */
void FunctionTable::eraseFunction(const std::string& name) {

    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;

    retVal = table.equal_range(name);
    table.erase(retVal.first, retVal.second);
    
}


/** Execute function and get its variable value (evaluate once) */
RbLanguageObject* FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) {

    Function&         theFunction = findFunction(name, args);
    RbLanguageObject*   theValue    = theFunction.execute();

    theFunction.clear();

    return theValue;
}



bool FunctionTable::existsFunction(std::string const &name) const {
    
    const std::map<std::string, Function *>::const_iterator& it = table.find( name );
    
    // if this table doesn't contain the function, then we ask the parent table
    if ( it == table.end() ) 
    {
        if ( parentTable != NULL ) 
        {
            return parentTable->existsFunction( name );
        }
        else 
        {
            return false;
        }
    }
    
    return true;
}


/**
 * Find functions matching name
 *
 * @todo Inherited functions are not returned if there
 *       are functions matching the name in the current
 *       workspace.
 */
std::vector<Function *> FunctionTable::findFunctions(const std::string& name) const {

    std::vector<Function *>  theFunctions;

    size_t count = table.count(name);
    if (count == 0) 
    {
        if (parentTable != NULL)
            return parentTable->findFunctions( name );
        else
            return theFunctions;
    }

    std::pair<std::multimap<std::string, Function *>::const_iterator,
              std::multimap<std::string, Function *>::const_iterator> retVal;
    retVal = table.equal_range( name );

    std::multimap<std::string, Function *>::const_iterator it;
    for ( it=retVal.first; it!=retVal.second; it++ )
        theFunctions.push_back( (*it).second->clone() );

    return theFunctions;
}


/** Find function (also processes arguments) */
Function& FunctionTable::findFunction(const std::string& name, const std::vector<Argument>& args) {
    
    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;
    
    size_t count = table.count(name);
    if (count == 0) 
    {
        if (parentTable != NULL) 
        {
            // \TODO: We shouldn't allow const casts!!!
            FunctionTable* pt = const_cast<FunctionTable*>(parentTable);
            return pt->findFunction(name, args);
        }
        else
            throw RbException("No function named '"+ name + "'");
    }
    retVal = table.equal_range(name);
    if (count == 1) {
        if (retVal.first->second->checkArguments(args,NULL) == false) 
        {
            
            std::ostringstream msg;
            msg << "Argument mismatch for call to function '" << name << "'(";
            // print the passed arguments
            for (std::vector<Argument>::const_iterator it = args.begin(); it != args.end(); it++) 
            {
                if (it != args.begin()) 
                {
                    msg << ",";
                }
                msg << " " << it->getVariable()->getValue().getTypeSpec() << " \"" << it->getLabel() << "\"";
            }
            msg << " ). Correct usage is:" << std::endl;
            retVal.first->second->printValue( msg );
            msg << std::endl;
            throw RbException( msg );
        }
        return *retVal.first->second;
    }
    else 
    {
        std::vector<unsigned int>* matchScore = new std::vector<unsigned int>();
        std::vector<unsigned int> bestScore;
        Function* bestMatch = NULL;

        bool ambiguous = false;
        std::multimap<std::string, Function *>::iterator it;
        for (it=retVal.first; it!=retVal.second; it++) 
        {
            matchScore->clear();
            if ( (*it).second->checkArguments(args, matchScore) == true ) 
            {
                if ( bestMatch == NULL ) 
                {
                    bestScore = *matchScore;
                    bestMatch = it->second;
                    ambiguous = false;
                }
                else 
                {
                    size_t j;
                    for (j=0; j<matchScore->size() && j<bestScore.size(); j++) 
                    {
                        if ((*matchScore)[j] < bestScore[j]) 
                        {
                            bestScore = *matchScore;
                            bestMatch = it->second;
                            ambiguous = false;
                            break;
                        }
                        else if ((*matchScore)[j] > bestScore[j])
                            break;
                    }
                    if (j==matchScore->size() || j==bestScore.size()) 
                    {
                        ambiguous = true;   // Continue checking, there might be better matches ahead
                    }
                }
            }
        }
        
        /* Delete all processed arguments except those of the best matching function, if it is ambiguous */
        for ( it = retVal.first; it != retVal.second; it++ ) 
        {
            if ( !( (*it).second == bestMatch && ambiguous == false ) )
                (*it).second->clear();
        }
        if ( bestMatch == NULL || ambiguous == true ) 
        {
            std::ostringstream msg;
            if ( bestMatch == NULL )
                msg << "No overloaded function '" << name << "' matches for arguments (";
            else
                msg << "Ambiguous call to function '" << name << "' with arguments (";
            // print the passed arguments
            for (std::vector<Argument>::const_iterator j = args.begin(); j != args.end(); j++) 
            {
                if (j != args.begin()) 
                {
                    msg << ",";
                }
//                msg << " " << it->getVariable().getDagNode()->getValue().getTypeSpec();
                const RbPtr<const Variable>& theVar = j->getVariable();
                msg << " " << theVar->getValue().getTypeSpec();
                
            }
            msg << " )" << std::endl;
            
            msg << "Potentially matching functions are:" << std::endl;
            for ( it = retVal.first; it != retVal.second; it++ ) 
            {
                (*it).second->printValue( msg );
                msg << std::endl;
            }
            throw RbException( msg );
        }
        else 
        {
            return *bestMatch;
        }
    }
    
    // We should never reach this, but to make the compiler happy ...
//    return RbNullObject::getInstance();
}



/* Get function */
const Function& FunctionTable::getFunction( const std::string& name ) {
    
    // find the template function
    const std::vector<Function *>& theFunctions = findFunctions(name);
    
    if ( theFunctions.size() > 1 ) 
    {
        std::ostringstream o;
        o << "Found " << theFunctions.size() << " functions with name \"" << name + "\". Identification not possible if not types of the arguments are specified.";
        throw RbException( o.str() );
    }
    
    return *theFunctions[0];
}


/* Get function */
const Function& FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) {
    
    // find the template function
    Function& theFunction = findFunction(name, args);

    return theFunction;
}


/** Check if two formals are unique */
bool FunctionTable::isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const  {

    /* Check that all labels are unique in both sets of argument rules */
    for (size_t i=0; i<x.size(); i++) 
    {
        for (size_t j=i+1; j < x.size(); j++) 
        {
            if (x[i].getArgumentLabel().size() != 0 && x[j].getArgumentLabel().size() != 0)
            if (x[i].getArgumentLabel() == x[j].getArgumentLabel())
                return false;
        }
    }
    for (size_t i=0; i<y.size(); i++) {
        for (size_t j=i+1; j<y.size(); j++) {
            if (y[i].getArgumentLabel().size() != 0 && y[j].getArgumentLabel().size() != 0)
            if (y[i].getArgumentLabel() == y[j].getArgumentLabel())
                return false;
        }
    }

    /* Check that the same labels are not used for different positions */
    for (size_t i=0; i<x.size(); i++) 
    {

        const std::string& xLabel = x[i].getArgumentLabel();
        if (xLabel.size() == 0)
            continue;

        for (size_t j=0; j<y.size(); j++) {

            const std::string& yLabel = y[j].getArgumentLabel();
            if (yLabel.size() == 0)
                continue;

            if (xLabel == yLabel && i != j)
                return false;
        }
    }

    /* Check that types are different for at least one argument without default values */
    size_t i;
    for (i=0; i<x.size() && i<y.size(); i++) 
    {
        if ( !(x[i].hasDefault() == true && y[i].hasDefault() == true) &&
            !x[i].isEllipsis() &&
            !y[i].isEllipsis() &&
            x[i].getArgumentTypeSpec() != y[i].getArgumentTypeSpec())
            return true;
    }
    for (size_t j=i; j<x.size(); j++) 
    {
        if (x[j].hasDefault() == false &&
            !x[j].isEllipsis())
            return true;
    }
    for (size_t j=i; j<y.size(); j++) 
    {
        if (y[j].hasDefault() == false &&
            !y[j].isEllipsis())
            return true;
    }

    return false;
}


/** Print function table for user */
void FunctionTable::printValue(std::ostream& o) const {

    o << "<name> = <returnType> function (<formal arguments>)" << std::endl;
    for (std::multimap<std::string, Function *>::const_iterator i=table.begin(); i!=table.end(); i++) 
    {
        o << i->first << " = ";
        i->second->printValue(o);
        o << std::endl;
    }
}
