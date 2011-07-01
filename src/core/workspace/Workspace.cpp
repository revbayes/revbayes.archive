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
 * $Id:$
 */

// Regular include files
#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "Container.h"
#include "Distribution.h"
#include "DistributionFunction.h"
#include "DistributionInterval.h"
#include "FunctionTable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbOptions.h"         // For PRINTF
#include "UserInterface.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "Workspace.h"

#include <sstream>
#include <vector>


/** Constructor of global workspace */
Workspace::Workspace() : VariableFrame(), functionTable(new FunctionTable()), typesInitialized(false) {

}


/** Constructor of user workspace */
Workspace::Workspace(Workspace* parentSpace)
    : VariableFrame(parentSpace), functionTable(new FunctionTable(globalWorkspace().getFunctionTable())), typesInitialized(false) {

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
bool Workspace::addDistribution(const std::string& name, DistributionInterval* dist) {

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

    typeTable.insert(std::pair<std::string, RbObject*>(exampleObj->getType(), exampleObj));

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

    typeTable.insert(std::pair<std::string, RbObject*>(templ->getType(), templ->clone()));

    functionTable->addFunction(name, new ConstructorFunction(templ));

    return true;
}


/** Execute function to get its value (workspaces only evaluate functions once) */
DAGNode* Workspace::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    /* Using this calling convention indicates that we are only interested in
       evaluating the function once */
    return functionTable->executeFunction(name, args);
}


/** Is the type added to the workspace? */
bool Workspace::existsType( const std::string& name ) const {

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentFrame != NULL )
            return static_cast<Workspace*>( parentFrame )->existsType( name );
        else
            return false;
    }
    else
        return true;
}


/** Find type template object */
RbObject* Workspace::findType( const std::string& name ) const {

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentFrame != NULL )
            return static_cast<Workspace*>( parentFrame )->findType( name );
        else
            throw RbException( "Type '" + name + "' does not exist in environment" );
    }
    else
        return it->second;
}


/** Get function */
RbFunction* Workspace::getFunction(const std::string& name, const std::vector<Argument>& args) {

    return functionTable->getFunction(name, args);
}


/** Check and get reference to the type name of an object type */
const std::string& Workspace::getTypeNameRef( const std::string& name ) const {

    return getTypeSpec( name ).getType();
}


/** Check and get type specification for a named object type */
TypeSpec Workspace::getTypeSpec( const std::string& name ) const {

    if ( name == ValueContainer_name || name == VariableContainer_name ) {
        
        // We cannot rely on the type spec provided by generic container type dummies in the type table
        // because they do not know the relevant element dimensions. There should not be such dummies
        // in the type table anyway, but we add an extra check here just to make sure
        throw RbException( "Invalid attempt to convert a generic container type to valid type specification" );
    }

    return findType( name )->getTypeSpec();
}


/** Check and possibly correct a type specification */
TypeSpec Workspace::getTypeSpec( const TypeSpec& typeSp ) const {

    // We can trust container type specifications
    if ( typeSp.getDim() > 0 )
        return typeSp;

    // We can also trust all non-container type specifications
    if ( !isXOfTypeY( typeSp.getType(), Container_name ) )
        return typeSp;

    // The case we need to catch is a container type using a non-container type specification (dim == 0)
    if ( typeSp.getType() == ValueContainer_name || typeSp.getType() == VariableContainer_name ) {
        
        // We cannot rely on the type spec provided by generic container type dummies in the type table
        // because they do not know the relevant element dimensions. There should not be such dummies
        // in the type table anyway, but we add an extra check here just to make sure
        throw RbException( "Invalid attempt to convert a generic container type to valid type specification" );
    }

    if ( typesInitialized == false && !existsType( typeSp.getType() ) )
        return TypeSpec( typeSp );    // Cannot provide this service if type table is not filled

    // Get the dummy type object to convert into a true container type specification
    return findType( typeSp.getType() )->getTypeSpec();
}


/** Convenient access to default random number generator */
RandomNumberGenerator* Workspace::get_rng(void) {

    return (RandomNumberGenerator*) getValue("_rng");
}


/** Type checking using type table and full type spec */
bool Workspace::isXOfTypeY( const TypeSpec& xTypeSp, const TypeSpec& yTypeSp ) const {

    // If yTypeSp is dimensionless object, then all types fit
    if ( yTypeSp.isDimensionlessObject() )
        return true;

    if ( xTypeSp.getDim() != yTypeSp.getDim() )
        return false;

    return isXOfTypeY( xTypeSp.getType(), yTypeSp.getType() );
}


/** Type checking using type table and type names, assuming dim = 0 */
bool Workspace::isXOfTypeY( const std::string& xType, const std::string& yType ) const {

    // Cannot provide this service in standard way if type table is not filled with xType
    // so hard code it instead
    if ( typesInitialized == false && ! existsType( xType ) ) {
        throw RbException( "Unknown type " + xType );
    }

    if ( !existsType( xType ) )
        throw RbException( "No type named '" + xType + "'" );

    return findType( xType )->isType( yType );
}


/** Is type x convertible to type y using type names, assuming dim = 0 */
bool Workspace::isXConvertibleToY( const std::string& xType, const std::string& yType ) const {

    TypeSpec    xTypeSpec   = TypeSpec( xType, 0 );
    TypeSpec    yTypeSpec   = TypeSpec( yType, 0 );

    return isXConvertibleToY( xTypeSpec, yTypeSpec );
}


/** Is type x convertible to type y? */
bool Workspace::isXConvertibleToY( const TypeSpec& xTypeSp, const TypeSpec& yTypeSp ) const {

    if ( typesInitialized == false )
        return true;    // Cannot provide this service if type table is not filled

    const std::string&  xType   = xTypeSp.getType();
    const std::string&  yType   = yTypeSp.getType();
    size_t              xDim    = xTypeSp.getDim();
    size_t              yDim    = yTypeSp.getDim();

    bool retVal = false;
    if ( xDim > 0 ) {
    
        VariableContainer* dummy    = new VariableContainer( xType );
        ValueContainer*    dummyVal = dummy->getConstValue();

        retVal = dummy->isConvertibleTo( yType, yDim, false );

        delete dummyVal;
        delete dummy;
    }
    else if (xDim == 0) {

        retVal = findType( xType )->isConvertibleTo( yType, yDim, false );
    }

    return retVal;
}


/** Print workspace */
void Workspace::printValue(std::ostream& o) const {

    o << "Variable table:" << std::endl;
    VariableFrame::printValue(o);
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


