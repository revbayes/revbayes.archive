#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "FunctionTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "RlFunction.h"
#include "StochasticNode.h"

#include <iomanip>
#include <ostream>
#include <sstream>

using namespace RevLanguage;

/** Basic constructor, empty table with or without parent */
FunctionTable::FunctionTable(FunctionTable* parent) : std::multimap<std::string, Function*>(), parentTable(parent) {

}


/** Copy constructor */
FunctionTable::FunctionTable(const FunctionTable& x) {
    
    for (std::multimap<std::string, Function *>::const_iterator i=x.begin(); i!=x.end(); i++)
    {
        insert(std::pair<std::string, Function *>( (*i).first, ( (*i).second->clone() )));
    }
    parentTable = x.parentTable;
}


/** Destructor. We own the functions so we need to delete them. */
FunctionTable::~FunctionTable(void) {

    clear();
    
}

/** Assignment operator */
FunctionTable& FunctionTable::operator=(const FunctionTable& x) {

    if (this != &x) 
    {

        clear();
        for (std::multimap<std::string, Function *>::const_iterator i=x.begin(); i!=x.end(); i++)
        {
            insert(std::pair<std::string, Function *>((*i).first, ( (*i).second->clone() ) ) );
        }
        
        parentTable = x.parentTable;
    }

    return (*this);
}


/** Add function to table */
void FunctionTable::addFunction(const std::string name, Function *func) {
    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;

    retVal = equal_range(name);
    for (std::multimap<std::string, Function *>::iterator i=retVal.first; i!=retVal.second; i++) 
    {
        if ( i->second->isProcedure() != func->isProcedure() )
        {
            // Construct an error message
            std::ostringstream msg;
            if ( func->isProcedure() )
                msg << "Procedure ";
            else
                msg << "Function ";
            
            msg << name << " =  ";
            i->second->printValue(msg);
            msg << " cannot overload ";
            if ( i->second->isProcedure() )
                msg << " procedure ";
            else
                msg << " function ";
            msg << name << " = ";
            func->printValue(msg);
            
            // throw the error message
            throw RbException(msg.str());
        }

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

    // Insert the function
    insert(std::pair<std::string, Function* >(name, func));

    // Name the function so that it is aware of what it is called
    func->setName( name );
}


/**
 * Clear table. We own the functions so we need
 * to delete them. When that is completed, we
 * call the base class clear function.
 */
void FunctionTable::clear(void) {
    
    for ( std::multimap<std::string, Function *>::const_iterator i = begin(); i != end(); i++ )
        delete( i->second );

    std::multimap<std::string, Function*>::clear();
    
}


/** Return a type-safe clone of the function table */
FunctionTable* FunctionTable::clone( void ) const {
    
    return new FunctionTable( *this );
}


/** Erase function. @todo This does not work if there are several functions with the same name. Also memory leak. */
void FunctionTable::eraseFunction(const std::string& name) {

    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;

    retVal = equal_range(name);
    erase(retVal.first, retVal.second);
    
}


/** Execute function and get its variable value (evaluate once) */
RevPtr<Variable> FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) {

    Function&         theFunction = findFunction(name, args);
    RevPtr<Variable>  theValue    = theFunction.execute();

    theFunction.clear();

    return theValue;
}


/**
 * Does a function with the given name exist? We check this
 * function table and then delegate to parent table if we cannot
 * find the function here.
 */
bool FunctionTable::existsFunction(std::string const &name) const {
    
    const std::map<std::string, Function *>::const_iterator& it = find( name );
    
    // if this table doesn't contain the function, then we ask the parent table
    if ( it == end() )
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

    size_t hits = count(name);
    if (hits == 0)
    {
        if (parentTable != NULL)
            return parentTable->findFunctions( name );
        else
            return theFunctions;
    }

    std::pair<std::multimap<std::string, Function *>::const_iterator,
              std::multimap<std::string, Function *>::const_iterator> retVal;
    retVal = equal_range( name );

    std::multimap<std::string, Function *>::const_iterator it;
    for ( it=retVal.first; it!=retVal.second; it++ )
        theFunctions.push_back( (*it).second->clone() );

    return theFunctions;
}


/** Find function (also processes arguments) */
Function& FunctionTable::findFunction(const std::string& name, const std::vector<Argument>& args) {
    
    std::pair<std::multimap<std::string, Function *>::iterator,
              std::multimap<std::string, Function *>::iterator> retVal;
    
    size_t hits = count(name);
    if (hits == 0)
    {
        
        if (parentTable != NULL) 
        {
            // \TODO: We shouldn't allow const casts!!!
            FunctionTable* pt = const_cast<FunctionTable*>(parentTable);
            return pt->findFunction(name, args);
        }
        else
        {
            throw RbException("No function named '"+ name + "'");
        }
        
    }
    retVal = equal_range(name);
    if (hits == 1) {
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
                std::string type = "NULL";
                if (it->getVariable() != NULL) type = it->getVariable()->getRevObject().getType();
                msg << " " << type << " \"" << it->getLabel() << "\"";
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
        
        // free the memory
        delete matchScore;
        
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
                const RevPtr<const Variable>& theVar = j->getVariable();
                msg << " " << theVar->getRevObject().getTypeSpec().getType();
                
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
    
}


/**
 * Get first function. This function will find the first function with a matching name without
 * throwing an error. Compare with the getFunction(name) function, which will throw an error
 * if the function name is overloaded.
 */
const Function& FunctionTable::getFirstFunction( const std::string& name ) {
    
    // find the template function
    const std::vector<Function *>& theFunctions = findFunctions(name);
    
    return *theFunctions[0];
}


/** Get function. This function will throw an error if the name is missing or if there are several matches (overloaded functions) */
const Function& FunctionTable::getFunction( const std::string& name ) {
    
    // find the template function
    const std::vector<Function *>& theFunctions = findFunctions(name);
    
    if ( theFunctions.size() > 1 ) 
    {
        std::ostringstream o;
        o << "Found " << theFunctions.size() << " functions with name \"" << name + "\". Identification not possible if arguments are not specified.";
        throw RbException( o.str() );
    }
    
    return *theFunctions[0];
}


/** Get function. This function will throw an error if the name and args do not match any named function. */
Function& FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) {
    
    // find the template function
    Function& theFunction = findFunction(name, args);

    return theFunction;
}


/** Get a copy of the function table, including either the functions in the frame or in the entire environment */
std::multimap<std::string, Function*> FunctionTable::getTableCopy(bool env) const
{
    std::multimap<std::string, Function*> tableCopy = *this;

    // TODO: Do not insert hidden (overridden) functions from parent table
    if (env == true && parentTable != NULL)
    {
        std::multimap<std::string, Function*> parentTableCopy = parentTable->getTableCopy(true);
        std::multimap<std::string, Function*>::iterator it;
        
        for (it=parentTableCopy.begin(); it!=parentTableCopy.end(); it++)
            tableCopy.insert( (*it) );
    }
    
    return tableCopy;
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


/**
 * Is the function with the given name a procedure? We check this
 * function table and then delegate to parent table if we cannot
 * find the function here.
 */
bool FunctionTable::isProcedure(const std::string& name) const
{
    const std::map<std::string, Function *>::const_iterator& it = find( name );
    
    // If we have the function, we know the answer
    if ( it != end() )
        return it->second->isProcedure();

    // If this table doesn't contain the function, then we ask the parent table
    if ( parentTable != NULL )
    {
        return parentTable->isProcedure( name );
    }
    else
    {
        throw RbException( "No function or procedure '" + name + "'" );
    }
}


/** Print function table for user in pretty format */
void FunctionTable::printValue(std::ostream& o, bool env) const {
    
    std::multimap<std::string, Function*> printTable;
    
    // We get a single table for frame or environment, sorted appropriately
    printTable = getTableCopy( env );

    // Do not print anything if table is empty
    if (printTable.size() == 0)
        return;

    size_t maxFunctionNameLength = 6;   // Length of '<name>'
    size_t maxReturnTypeLength   = 12;  // Length of '<returnType>'

    for (std::multimap<std::string, Function *>::const_iterator i=printTable.begin(); i!=printTable.end(); i++)
    {
        if ( i->first.size() > maxFunctionNameLength )
            maxFunctionNameLength = i->first.size();
        if ( i->second->getReturnType().getType().size() > maxReturnTypeLength )
            maxReturnTypeLength = i->second->getReturnType().getType().size();
    }

    int nameLen       = int( maxFunctionNameLength );
    int returnTypeLen = int( maxReturnTypeLength );
    int argTabLen     = nameLen + returnTypeLen + 14;   // position of closing parenthesis
    int headerLen     = nameLen + returnTypeLen + 33;

    o << std::setw(nameLen) << std::left << "<name>" << " = ";
    o << std::setw(returnTypeLen) << "<returnType> " << " function (<formal arguments>)" << std::endl;
    o << std::setw(headerLen) << std::setfill('-') << "" << std::setfill(' ') << std::endl;

    for (std::multimap<std::string, Function *>::const_iterator i=printTable.begin(); i!=printTable.end(); i++)
    {
        o << std::setw(nameLen) << i->first << " = ";
        
        o << std::setw(returnTypeLen) << i->second->getReturnType().getType() << " function " << std::setw(1);
        
        const RevLanguage::ArgumentRules& argRules = i->second->getArgumentRules();
        
        if ( argRules.size() > 1 )
        {
            o << "(" << std::endl;
            for ( std::vector<ArgumentRule *>::const_iterator i = argRules.begin(); i != argRules.end(); i++ )
            {
                o << std::setw(argTabLen) << std::setfill(' ') << " " << std::setw(1);
                (*i)->printValue(o);
                if ( i != argRules.end() - 1 )
                    o << ",";
                o << std::endl;
            }
            o << std::setw(argTabLen) << std::setfill(' ') << std::right << ")" << std::left << std::setw(1) << std::endl;
        }
        else
        {
            o << "(";
            for ( std::vector<ArgumentRule *>::const_iterator i = argRules.begin(); i != argRules.end(); i++ )
                (*i)->printValue(o);
            o << ")" << std::endl;
        }
    }
}

