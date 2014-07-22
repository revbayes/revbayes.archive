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
#include "Container.h"
#include "FunctionTable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RevObject.h"
#include "RbUtil.h"
#include "RbOptions.h"         // For debug defines
#include "RlDistribution.h"
#include "Function.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <cassert>
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
        throw RbException("There is already a type named '" + dist->getType() + "' in the workspace");

#ifdef DEBUG_WORKSPACE
    printf("Adding type %s to workspace\n", dist->getTypeSpec().getType().c_str());
#endif

//    typeTable.insert(std::pair<std::string, RevObject* >(dist->getTypeSpec(),dist->clone()));

    functionTable.addFunction(name, new ConstructorFunction( dist ) );

    return true;
}


/** Add type to the workspace */
bool Workspace::addType(RevObject *exampleObj) {

    std::string name = exampleObj->getType();
    
#ifdef DEBUG_WORKSPACE
    printf("Adding type %s to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RevObject*>(name, exampleObj));

    return true;
}


/** Add abstract type to the workspace */
bool Workspace::addType(const std::string& name, RevObject *exampleObj) {
    
#ifdef DEBUG_WORKSPACE
    printf("Adding special abstract type %s to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RevObject*>( name, exampleObj));

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, RevObject *templ) {
    
#ifdef DEBUG_WORKSPACE
    printf("Adding type %s with constructor to workspace\n", name.c_str());
#endif

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RevObject*>(templ->getType(), templ->clone()));
    
    functionTable.addFunction(name, new ConstructorFunction(templ));

    return true;
}


/** clone */
Workspace* Workspace::clone() const {
    return new Workspace(*this);
}


const TypeSpec& Workspace::getClassTypeSpecOfType(std::string const &type) const {
    
    std::map<std::string, RevObject*>::const_iterator it = typeTable.find( type );
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
bool Workspace::existsType( const std::string& name ) const {

    std::map<std::string, RevObject *>::const_iterator it = typeTable.find( name );
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



const TypeTable& Workspace::getTypeTable( void ) const
{
    
    return typeTable;
}


/**
 * Use the template object in the type table to make a default instance of
 * a specified Rev type. Throw an error if the type is abstract.
 */
RevObject* Workspace::makeNewDefaultObject(const std::string& type) const {
    
    std::map<std::string, RevObject*>::const_iterator it = typeTable.find( type );
    
    if ( it == typeTable.end() )
    {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->makeNewDefaultObject( type );
        else
            throw RbException( "Type '" + type + "' does not exist in environment" );;
    }
    else
    {
        if ( it->second->isAbstract() )
            throw RbException( "No default instance of abstract type '" + type + "'" );
        
        return it->second->clone();
    }
}


/**
 * Here we use the template container type objects to make an empty container
 * of the desired type. If we cannot find the desired type, we try to make a
 * container with elements of the immediate base class of the requested element
 * type. By proceeding up the inheritance hierarchy, we are guaranteed to find
 * some container for the requested element type, given that there is at least
 * a RevObjectContainer of RevObject with the right dimension in our type
 * table.
 *
 * We do container promotion as described above only in the global workspace,
 * not in the user workspace.
 */
Container* Workspace::makeNewEmptyContainer( const std::string& elemType, size_t dim ) const
{
    std::stringstream type;
    type << elemType;
    for ( size_t i = 0; i < dim; ++i )
        type << "[]";
    
    // Try to find the container
    std::map<std::string, RevObject*>::const_iterator it = typeTable.find( type.str() );
    if ( it != typeTable.end() )
        return static_cast<Container*>( it->second->clone() );
    
    // If that fails, ask our parent
    if ( parentEnvironment != NULL )
        return static_cast<Workspace*>( parentEnvironment )->makeNewEmptyContainer( elemType, dim );
    
    // If that fails, we are the global workspace and cannot ask for help.
    // We try element type promotion to base class element types.
    // We first get the typespec from the template element type object
    // and then proceed up the type hierarchy until we find a suitable
    // container.
    
    // Find the element type in the type table.
    it = typeTable.find( elemType );
    if ( it == typeTable.end() )
        throw RbException( "Element type '" + elemType + "' missing" );
    
    // Get its type specification
    const TypeSpec& elemTypeSpec = it->second->getTypeSpec();

    // 
    const TypeSpec* parentElement = elemTypeSpec.getParentTypeSpec();
    while ( parentElement != NULL )
    {
        // Make type of promoted container
        std::stringstream promotedType;
        promotedType << parentElement->getType();
        for ( size_t i = 0; i < dim; ++i )
            promotedType << "[]";

        // Try to find it
        if ( ( it = typeTable.find( promotedType.str() ) ) != typeTable.end() )
            return static_cast<Container*>( it->second->clone() );

        // If failed, try parent element type spec
        parentElement = parentElement->getParentTypeSpec();
    }
    
    throw RbException( "Container of type '" + type.str() + "' not supported (yet)" );
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
        std::map<std::string, RevObject *>::const_iterator i;
        for (i=typeTable.begin(); i!=typeTable.end(); i++)
        {
            if ( (*i).second != NULL )
                o << (*i).first << " = " << (*i).second->getTypeSpec() << std::endl;
            else
                o << (*i).first << " = " << "unknown class vector" << std::endl;
        }
    }
}


