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
#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "Distribution.h"
#include "DistributionFunction.h"
#include "DistributionContinuous.h"
#include "FunctionTable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"         // For PRINTF
#include "UserInterface.h"
#include "Workspace.h"

#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Workspace::typeSpec(Workspace_name);

/** Constructor of global workspace */
Workspace::Workspace() : Environment(), functionTable(new FunctionTable()), typesInitialized(false) {
    // since we are a singleton we own ourselve
    retain();
}


/** Constructor of user workspace */
Workspace::Workspace(Workspace* parentSpace) : Environment(parentSpace), functionTable(new FunctionTable(globalWorkspace().getFunctionTable())), typesInitialized(false) {
    // since we are a singleton we own ourselve
    retain();
}


/** Destructor */
Workspace::~Workspace() {

    delete functionTable;
    for (TypeTable::iterator i=typeTable.begin(); i!=typeTable.end(); i++)
        delete (*i).second;
}


/** Assignment operator */
Workspace& Workspace::operator=(const Workspace& x) {

    if (this != &x) {

        delete functionTable;
        functionTable = new FunctionTable(x.functionTable);
    }

    return (*this);
}


/** Add distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, Distribution* dist) {

    PRINTF("Adding distribution %s to workspace\n", name.c_str());

    if ( typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + dist->getType() + "' in the workspace");

    PRINTF("Adding type %s to workspace\n", dist->getType().c_str());
    typeTable.insert(std::pair<std::string, RbObject*>(dist->getType(), dist->clone()));

    functionTable->addFunction(name, new ConstructorFunction(dist));
    functionTable->addFunction("d" + name, new DistributionFunction(dist, DistributionFunction::DENSITY));
    functionTable->addFunction("r" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::RVALUE));

    return true;
}


/** Add real-valued distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, DistributionContinuous* dist) {

    PRINTF("Adding real-valued distribution %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(name, dist->clone()));

    functionTable->addFunction(name, new ConstructorFunction(dist));
    functionTable->addFunction("d" + name, new DistributionFunction(dist, DistributionFunction::DENSITY));
    functionTable->addFunction("r" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::RVALUE));
    functionTable->addFunction("p" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::PROB));
    functionTable->addFunction("q" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::QUANTILE));

    return true;
}


/** Add function to the workspace */
bool Workspace::addFunction(const std::string& name, RbFunction* func) {

    PRINTF( "Adding function %s = %s to workspace\n", name.c_str(), func->briefInfo().c_str() );
    
    if (existsVariable(name))
        throw RbException("There is already a variable named '" + name + "' in the workspace");

    functionTable->addFunction(name, func);
    
    return true;
}


/** Add type to the workspace */
bool Workspace::addType(RbObject* exampleObj) {

    std::string name = exampleObj->getType();

    PRINTF("Adding type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(name, exampleObj));
    
    // retain the object
    exampleObj->retain();

    return true;
}


/** Add abstract type to the workspace */
bool Workspace::addType(const std::string& name, RbObject* exampleObj) {

    PRINTF("Adding special abstract type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>( name, exampleObj));
    
    // retain the object
    exampleObj->retain();

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, MemberObject* templ) {

    PRINTF("Adding type %s with constructor to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(templ->getType(), templ->clone()));

    functionTable->addFunction(name, new ConstructorFunction(templ));

    return true;
}


/** clone */
Workspace* Workspace::clone() const {
    return new Workspace(*this);
}

/** Get class vector describing type of object */
const VectorString& Workspace::getClass() const {
    
    static VectorString rbClass = VectorString(Workspace_name) + Environment::getClass();
    return rbClass;
}


/** Execute function to get its value (workspaces only evaluate functions once) */
RbLanguageObject* Workspace::executeFunction(const std::string& name, const std::vector<Argument*>& args) const {

    /* Using this calling convention indicates that we are only interested in
       evaluating the function once */
    return functionTable->executeFunction(name, args);
}


/** Is the type added to the workspace? */
bool Workspace::existsType( const TypeSpec& name ) const {

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->existsType( name );
        else
            return false;
    }
    else
        return true;
}


/** Find type template object */
RbObject* Workspace::findType( const TypeSpec& name ) const {

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->findType( name );
        else
            throw RbException( "Type '" + name + "' does not exist in environment" );
    }
    else
        return it->second;
}


/** Get function */
RbFunction* Workspace::getFunction(const std::string& name, const std::vector<Argument*>& args) {

    return functionTable->getFunction(name, args);
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Workspace::getTypeSpec(void) const {
    return typeSpec;
}


/** Type checking using type table and full type spec */
bool Workspace::isXOfTypeY( const TypeSpec& xTypeSp, const TypeSpec& yTypeSp ) const {
    
    // Cannot provide this service in standard way if type table is not filled with xType
    // so hard code it instead
    if ( typesInitialized == false && ! existsType( xTypeSp ) ) {
        throw RbException( "Unknown type " + xTypeSp.getType() );
    }
    
    if ( !existsType( xTypeSp ) )
        throw RbException( "No type named '" + xTypeSp.getType() + "'" );
    
    return findType( xTypeSp )->isTypeSpec( yTypeSp );
}


/** Is type x convertible to type y? */
bool Workspace::isXConvertibleToY( const TypeSpec& xTypeSp, const TypeSpec& yTypeSp ) const {

    if ( typesInitialized == false )
        return true;    // Cannot provide this service if type table is not filled

    bool retVal = false;
    
    retVal = findType( xTypeSp )->isConvertibleTo( yTypeSp );
    

    return retVal;
}


/** Print workspace */
void Workspace::printValue(std::ostream& o) const {

    o << "Variable table:" << std::endl;
    VariableTable::const_iterator it;
    for ( it = variableTable.begin(); it != variableTable.end(); it++) {
        o << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }
    o << std::endl;

    o << "Function table:" << std::endl;
    functionTable->printValue(o);
    o << std::endl;

    o << "Type table:" << std::endl;
    std::map<std::string, RbObject*>::const_iterator i;
    for (i=typeTable.begin(); i!=typeTable.end(); i++) {
        if ( (*i).second != NULL )
            o << (*i).first << " = " << (*i).second->getTypeSpec() << std::endl;
        else
            o << (*i).first << " = " << "unknown class vector" << std::endl;
    }
}


/** Complete info about object to string */
std::string Workspace::richInfo( void ) const {
    
    std::ostringstream o;
    o << "Workspace:" << std::endl;
    printValue( o );
    
    return o.str();
}


