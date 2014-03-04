/**
 * @file
 * This file contains the implementation of Workspace, which is
 * used to hold the global workspace and the user workspace.
 *
 * @brief Implementation of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

// Regular include files
#include "ConstructorFunction.h"
#include "FunctionTable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "RbOptions.h"         // For PRINTF
#include "RlDistribution.h"
#include "Function.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <sstream>
#include <vector>

using namespace RevLanguage;

/* Constructor of global workspace */
Workspace::Workspace() : Environment(), typesInitialized(false) {

}


/* Constructor of user workspace */
Workspace::Workspace(Environment* parentSpace) : Environment(parentSpace), typesInitialized(false) {
    
}


/* Constructor of user workspace */
Workspace::Workspace(Workspace* parentSpace) : Environment(parentSpace), typesInitialized(false) {

}

/* Constructor of user workspace */
Workspace::Workspace(const Workspace& x) : Environment(x), typesInitialized(x.typesInitialized) {
    
}



/* Assignment operator */
Workspace& Workspace::operator=(const Workspace& x) {

    if (this != &x) 
    {
        // first we need to delegate to the base class assignment operator
        Environment::operator=(x);
    }

    return (*this);
}


/* Add distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, Distribution *dist) {

#ifdef DEBUG_WORKSPACE
    printf("Adding distribution %s to workspace\n", name.c_str());
#endif

    if ( typeTable.find(name) != typeTable.end() )
        throw RbException("There is already a type named '" + dist->getTypeSpec() + "' in the workspace");

#ifdef DEBUG_WORKSPACE
    printf("Adding type %s to workspace\n", dist->getTypeSpec().getType().c_str());
#endif

//    typeTable.insert(std::pair<std::string, RbLanguageObject* >(dist->getTypeSpec(),dist->clone()));

    functionTable.addFunction(name, new ConstructorFunction( dist ) );

    return true;
}


/** Add type to the workspace */
bool Workspace::addType(RbLanguageObject *exampleObj) {

    std::string name = exampleObj->getTypeSpec();
    
#ifdef DEBUG_WORKSPACE
    printf("Adding type %s to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbLanguageObject*>(name, exampleObj));

    return true;
}


/** Add abstract type to the workspace */
bool Workspace::addType(const std::string& name, RbLanguageObject *exampleObj) {
    
#ifdef DEBUG_WORKSPACE
    printf("Adding special abstract type %s to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbLanguageObject*>( name, exampleObj));

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, RbLanguageObject *templ) {
    
#ifdef DEBUG_WORKSPACE
    printf("Adding type %s with constructor to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbLanguageObject*>(templ->getTypeSpec(), templ->clone()));
    
    functionTable.addFunction(name, new ConstructorFunction(templ));

    return true;
}


/** clone */
Workspace* Workspace::clone() const {
    return new Workspace(*this);
}


const TypeSpec& Workspace::getClassTypeSpecOfType(std::string const &type) const {
    
    std::map<std::string, RbLanguageObject*>::const_iterator it = typeTable.find( type );
    if ( it == typeTable.end() ) 
    {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->getClassTypeSpecOfType( type );
        else
            throw RbException( "Type '" + type + "' does not exist in environment" );;
    }
    else
        return it->second->getTypeSpec();
}



/* Is the type added to the workspace? */
bool Workspace::existsType( const TypeSpec& name ) const {

    std::map<std::string, RbLanguageObject *>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) 
    {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->existsType( name );
        else
            return false;
    }
    else
        return true;
}



/** Print the frame content, not the entire environment. */
void Workspace::printValue(std::ostream& o) const {

    if ( variableTable.size() > 0 )
    {
        o << "Variable table:" << std::endl;
        VariableTable::const_iterator it;
        for ( it = variableTable.begin(); it != variableTable.end(); it++)
        {
            o << (*it).first << " = ";
            (*it).second->printValue( o );
            o << std::endl;
        }
        o << std::endl;
    }

    std::stringstream s;
    functionTable.printValue(s, false);

    if (s.str().size() > 0 )
    {
        o << "Function table:" << std::endl;
        o << s.str() << std::endl;
    }

    if ( typeTable.size() > 0 )
    {
        o << "Type table:" << std::endl;
        std::map<std::string, RbLanguageObject *>::const_iterator i;
        for (i=typeTable.begin(); i!=typeTable.end(); i++)
        {
            if ( (*i).second != NULL )
                o << (*i).first << " = " << (*i).second->getTypeSpec() << std::endl;
            else
                o << (*i).first << " = " << "unknown class vector" << std::endl;
        }
    }
}



