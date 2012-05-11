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
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"
#include "DistributionContinuous.h"
#include "FunctionTable.h"
#include "ParserDistribution.h"
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

    if (this != &x) {
        // first we need to delegate to the base class assignment operator
        Environment::operator=(x);
    }

    return (*this);
}


/* Add distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, const RbPtr<ParserDistribution> &dist) {

    PRINTF("Adding distribution %s to workspace\n", name.c_str());

    if ( typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + dist->getTypeSpec() + "' in the workspace");

    PRINTF("Adding type %s to workspace\n", dist->getType().c_str());
    typeTable.insert(std::pair<std::string, RbPtr<RbObject> >(dist->getTypeSpec(),dist->clone()));

    functionTable.addFunction(name, new ConstructorFunction( dist ) );
    functionTable.addFunction("d" + name, new DistributionFunctionPdf( dist->clone() ) );
    functionTable.addFunction("r" + name, new DistributionFunctionRv( dist->clone() ) );

    return true;
}


/** Add real-valued distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, const RbPtr<ParserDistributionContinuous> &dist) {
    
    PRINTF("Adding real-valued distribution %s to workspace\n", name.c_str());
    
    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");
    
    typeTable.insert(std::pair<std::string, RbObject*>(name, dist->clone()));
    
    functionTable.addFunction(name      , new ConstructorFunction ( dist ));
    functionTable.addFunction("d" + name, new DistributionFunctionPdf( dist->clone() ));
    functionTable.addFunction("r" + name, new DistributionFunctionRv(dist->clone() ));
    functionTable.addFunction("p" + name, new DistributionFunctionCdf(dist->clone() ));
    functionTable.addFunction("q" + name, new DistributionFunctionQuantile(dist->clone() ));
    
    return true;
}


/** Add type to the workspace */
bool Workspace::addType(const RbPtr<RbObject> &exampleObj) {

    std::string name = exampleObj->getTypeSpec();

    PRINTF("Adding type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(name, exampleObj));

    return true;
}


/** Add abstract type to the workspace */
bool Workspace::addType(const std::string& name, const RbPtr<RbObject> &exampleObj) {

    PRINTF("Adding special abstract type %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>( name, exampleObj));

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, const RbPtr<MemberObject> &templ) {

    PRINTF("Adding type %s with constructor to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(templ->getTypeSpec(), templ->clone()));

    functionTable.addFunction(name, new ConstructorFunction(templ));

    return true;
}

/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, const RbPtr<RbLanguageObject> &templ) {
    
    PRINTF("Adding simple type %s with constructor to workspace\n", name.c_str());
    
    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");
    
    typeTable.insert(std::pair<std::string, RbObject*>(templ->getTypeSpec(), templ->clone()));
    
    functionTable.addFunction(name, new ConstructorFunctionForSimpleObjects(templ));
    
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
    
    std::map<std::string, RbPtr<RbObject> >::const_iterator it = typeTable.find( type );
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


/* Is the type added to the workspace? */
bool Workspace::existsType( const TypeSpec& name ) const {

    std::map<std::string, RbPtr<RbObject> >::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( (Environment*)parentEnvironment )->existsType( name );
        else
            return false;
    }
    else
        return true;
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
    functionTable.printValue(o);
    o << std::endl;

    o << "Type table:" << std::endl;
    std::map<std::string, RbPtr<RbObject> >::const_iterator i;
    for (i=typeTable.begin(); i!=typeTable.end(); i++) {
        if ( (*i).second != NULL )
            o << (*i).first << " = " << (*i).second->getTypeSpec() << std::endl;
        else
            o << (*i).first << " = " << "unknown class vector" << std::endl;
    }
}



