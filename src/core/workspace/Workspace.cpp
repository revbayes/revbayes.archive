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
#include "ConstructorFunctionForSimpleObjects.h"
#include "Distribution.h"
#include "DistributionFunction.h"
#include "DistributionContinuous.h"
#include "FunctionTable.h"
#include "ParserDistributionContinuous.h"
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


/** Constructor of global workspace */
Workspace::Workspace() : Environment(), functionTable(new FunctionTable()), typesInitialized(false) {

}


/** Constructor of user workspace */
Workspace::Workspace(Environment* parentSpace) : Environment(parentSpace), functionTable(new FunctionTable(globalWorkspace().getFunctionTable())), typesInitialized(false) {
    
}


/** Constructor of user workspace */
Workspace::Workspace(Workspace* parentSpace) : Environment(parentSpace), functionTable(new FunctionTable(&parentSpace->getFunctionTable())), typesInitialized(false) {

}

/** Constructor of user workspace */
Workspace::Workspace(const Workspace& x) : Environment(x), functionTable(x.functionTable->clone()), typesInitialized(x.typesInitialized) {
    
}


/** Destructor */
Workspace::~Workspace() {

    delete functionTable;
    
    for (TypeTable::iterator it = typeTable.begin(); it != typeTable.end(); it++) {
        RbObject* type = it->second;
        delete type;
    }
}


/** Assignment operator */
Workspace& Workspace::operator=(const Workspace& x) {

    if (this != &x) {
        // first we need to delegate to the base class assignment operator
        Environment::operator=(x);
        
        functionTable = x.functionTable->clone();
    }

    return (*this);
}


///** Add distribution to the workspace */
//bool Workspace::addDistribution(const std::string& name, ParserDistribution* dist) {
//
//    PRINTF("Adding distribution %s to workspace\n", name.c_str());
//
//    if ( typeTable.find(name) != typeTable.end())
//        throw RbException("There is already a type named '" + dist->getTypeSpec() + "' in the workspace");
//
//    PRINTF("Adding type %s to workspace\n", dist->getType().c_str());
//    typeTable.insert(std::pair<std::string, RbObject*>(dist->getTypeSpec(),dist->clone()));
//
//    functionTable->addFunction(name, new ConstructorFunction( dist ) );
//    functionTable->addFunction("d" + name, new DistributionFunction(dist->clone(), DistributionFunction::DENSITY));
//    functionTable->addFunction("r" + name, new DistributionFunction((dist->clone()), DistributionFunction::RVALUE));
//
//    return true;
//}


/** Add real-valued distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, ParserDistributionContinuous* dist) {
    
    PRINTF("Adding real-valued distribution %s to workspace\n", name.c_str());
    
    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");
    
    typeTable.insert(std::pair<std::string, RbObject*>(name, dist->clone()));
    
    functionTable->addFunction(name      , new ConstructorFunction ( dist ));
    functionTable->addFunction("d" + name, new DistributionFunction( dist->clone() , DistributionFunction::DENSITY));
    functionTable->addFunction("r" + name, new DistributionFunction(dist->clone(), DistributionFunction::RVALUE));
    functionTable->addFunction("p" + name, new DistributionFunction(dist->clone(), DistributionFunction::PROB));
    functionTable->addFunction("q" + name, new DistributionFunction(dist->clone(), DistributionFunction::QUANTILE));
    
    return true;
}


///** Add real-valued distribution to the workspace */
//bool Workspace::addDistribution(const std::string& name, DistributionContinuous* dist) {
//
//    PRINTF("Adding real-valued distribution %s to workspace\n", name.c_str());
//
//    if (typeTable.find(name) != typeTable.end())
//        throw RbException("There is already a type named '" + name + "' in the workspace");
//
//    typeTable.insert(std::pair<std::string, RbObject*>(name, dist->clone()));
//
//    functionTable->addFunction(name      , new ConstructorFunction ( dist ));
//    functionTable->addFunction("d" + name, new DistributionFunction( dist->clone() , DistributionFunction::DENSITY));
//    functionTable->addFunction("r" + name, new DistributionFunction(dist->clone(), DistributionFunction::RVALUE));
//    functionTable->addFunction("p" + name, new DistributionFunction(dist->clone(), DistributionFunction::PROB));
//    functionTable->addFunction("q" + name, new DistributionFunction(dist->clone(), DistributionFunction::QUANTILE));
//
//    return true;
//}


/** Add function to the workspace */
bool Workspace::addFunction(const std::string& name, RbFunction* func) {

    PRINTF( "Adding function %s = %s to workspace\n", name.c_str(), func->debugInfo().c_str() );
    
    if (existsVariable(name))
        throw RbException("There is already a variable named '" + name + "' in the workspace");

    functionTable->addFunction(name, func);
    
    return true;
}


/** Add type to the workspace */
bool Workspace::addType(RbObject* exampleObj) {

    std::string name = exampleObj->getTypeSpec();

    PRINTF("Adding type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(name, exampleObj));

    return true;
}


/** Add abstract type to the workspace */
bool Workspace::addType(const std::string& name, RbObject* exampleObj) {

    PRINTF("Adding special abstract type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>( name, exampleObj));

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, MemberObject* templ) {

    PRINTF("Adding type %s with constructor to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(templ->getTypeSpec(), templ->clone()));

    functionTable->addFunction(name, new ConstructorFunction(templ));

    return true;
}

/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, RbLanguageObject* templ) {
    
    PRINTF("Adding simple type %s with constructor to workspace\n", name.c_str());
    
    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");
    
    typeTable.insert(std::pair<std::string, RbObject*>(templ->getTypeSpec(), templ->clone()));
    
    functionTable->addFunction(name, new ConstructorFunctionForSimpleObjects(templ));
    
    return true;
}


/** clone */
Workspace* Workspace::clone() const {
    return new Workspace(*this);
}


/** Get class name of object */
const std::string& Workspace::getClassName(void) { 
    
    static std::string rbClassName = "Workspace";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Workspace::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Environment::getClassTypeSpec() ) );
    
	return rbClass; 
}

const TypeSpec& Workspace::getClassTypeSpecOfType(std::string const &type) const {
    
    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( type );
    if ( it == typeTable.end() ) {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->getClassTypeSpecOfType( type );
        else
            throw RbException( "Type '" + type + "' does not exist in environment" );;
    }
    else
        return it->second->getTypeSpec();
}

/** Get type spec */
const TypeSpec& Workspace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Execute function to get its value (workspaces only evaluate functions once) */
const RbLanguageObject& Workspace::executeFunction(const std::string& name, const std::vector<Argument>& args) {

    /* Using this calling convention indicates that we are only interested in
       evaluating the function once */
    return functionTable->executeFunction(name, args);
}


/** Is the type added to the workspace? */
bool Workspace::existsType( const TypeSpec& name ) const {

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( (Environment*)parentEnvironment )->existsType( name );
        else
            return false;
    }
    else
        return true;
}


///** Find type template object */
//RbObject* Workspace::findType( const TypeSpec& name ) const {
//
//    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
//    if ( it == typeTable.end() ) {
//        if ( parentEnvironment != NULL )
//            return static_cast<Workspace*>( parentEnvironment )->findType( name );
//        else
//            throw RbException( "Type '" + name + "' does not exist in environment" );
//    }
//    else
//        return it->second;
//}


/** Get function */
const RbFunction& Workspace::getFunction(const std::string& name, const std::vector<Argument>& args) {
    
    return functionTable->getFunction(name, args);
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



