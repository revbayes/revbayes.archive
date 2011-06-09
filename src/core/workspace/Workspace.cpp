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

    if (typeTable.find(name) != typeTable.end())
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

    PRINTF("Adding type %s to workspace\n", exampleObj->getTypeSpec().toString().c_str());

    std::string name = exampleObj->getType();

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, RbObject*>(exampleObj->getType(), exampleObj));

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

    if ( typesInitialized == false )
        return true;    // Cannot provide this service if type table is not filled
    
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

    if ( typesInitialized == false )
        return TypeSpec( name, 0 );    // Cannot provide this service if type table is not filled

    //! @todo We probably need to catch the abstract types in a more general way
    if ( name == RbObject_name )
        return TypeSpec( RbObject_name, 0 );
    else if ( name == RbNULL_name )
        return TypeSpec( RbNULL_name, 0 );
    else if ( name == RbVoid_name )
        return TypeSpec( RbVoid_name, 0 );

    if ( name == ValueContainer_name || name == VariableContainer_name ) {
        
        // We cannot rely on the type spec provided by generic container type dummies in the type table
        // because they do not know the relevant element dimensions. There should not be such dummies
        // in the type table anyway, but we add an extra check here just to make sure
        throw RbException( "Invalid attempt to convert a generic container type to valid type specification" );
    }

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) {
        if ( parentFrame != NULL )
            return static_cast<Workspace*>( parentFrame )->getTypeSpec( name );
        else
            throw RbException( "No object class with name " + name );
    }
    return (*it).second->getTypeSpec();
}


/** Check and get type specification for a named object type */
TypeSpec Workspace::getTypeSpec( const TypeSpec& typeSp ) const {

    if ( typesInitialized == false )
        return TypeSpec( typeSp );    // Cannot provide this service if type table is not filled

    //! @todo We probably need to catch the abstract types in a more general way
    if ( typeSp.getType() == RbObject_name && typeSp.getDim() == 0 )
        return TypeSpec( typeSp );

    if ( typeSp.getType() == ValueContainer_name || typeSp.getType() == VariableContainer_name ) {
        
        // We cannot rely on the type spec provided by generic container type dummies in the type table
        // because they do not know the relevant element dimensions. There should not be such dummies
        // in the type table anyway, but we add an extra check here just to make sure
        throw RbException( "Invalid attempt to convert a generic container type to valid type specification" );
    }

    // Other than generic containers, we can trust container type specifications
    if ( typeSp.getDim() > 0 )
        return typeSp;

    // We can also trust all non-container type specifications
    if ( !isXOfTypeY( typeSp.getType(), Container_name ) )
        return typeSp;

    std::map<std::string, RbObject*>::const_iterator it = typeTable.find( typeSp.getType() );
    if ( it == typeTable.end() ) {
        if ( parentFrame != NULL )
            return static_cast<Workspace*>( parentFrame )->getTypeSpec( typeSp );
        else
            throw RbException( "No object class with name " + typeSp.getType() );
    }
    return (*it).second->getTypeSpec();
}


/** Convenient access to default random number generator */
RandomNumberGenerator* Workspace::get_rng(void) {

    return (RandomNumberGenerator*) getValue("_rng");
}


/** Type checking using type table and full type spec */
bool Workspace::isXOfTypeY( const TypeSpec& xTypeSp, const TypeSpec& yTypeSp ) const {

    if ( xTypeSp.getDim() != yTypeSp.getDim() )
        return false;

    return isXOfTypeY( xTypeSp.getType(), yTypeSp.getType() );
}


/** Type checking using type table and type names, assuming same dim */
bool Workspace::isXOfTypeY( const std::string& xType, const std::string& yType ) const {

    if ( typesInitialized == false )
        return true;    // Cannot provide this service if type table is not filled

    //! @todo We probably need to handle the abstract types in a more general way
    if ( yType == RbObject_name && (existsType( xType ) || xType == RbNULL_name || xType == RbVoid_name) )
        return true;
    if ( ( xType == RbObject_name || xType == RbNULL_name || xType == RbVoid_name) && yType != RbObject_name )
        return false;

    /* Simplest case first */
    if ( xType == yType )
        return true;

    if ( typeTable.find( xType ) == typeTable.end() ) {
        if ( parentFrame == NULL )
            throw RbException( "Unknown type named '" + xType + "'" );
        else
            return ( static_cast<Workspace*>( parentFrame ) )->isXOfTypeY( xType, yType );
    }

    const VectorString& xTypeVec = typeTable.find( xType )->second->getClass();
    size_t i;
    for ( i = 0; i < xTypeVec.size(); i++) {
        if ( xTypeVec[i] == yType )
            break;
    }

    if ( i == xTypeVec.size() )
        return false;
    else
        return true;
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
    int                 xDim    = xTypeSp.getDim();
    int                 yDim    = yTypeSp.getDim();

    //! @todo We probably need to handle the abstract types in a more general way
    if ( yType == RbObject_name && existsType( xType ) && xDim == yDim )
        return true;

    bool retVal = false;
    if ( xDim > 0 ) {
    
        VariableContainer* dummy    = new VariableContainer( xType );
        ValueContainer*    dummyVal = dummy->getConstValue();

        retVal = dummy->isConvertibleTo( yType, yDim, false );

        delete dummyVal;
        delete dummy;
    }
    else if (xDim == 0) {

        TypeTable::const_iterator i = typeTable.find( xType );
        if ( i == typeTable.end() )
            throw RbException("Unknown type named '" + xType + "'");
        
        retVal = (*i).second->isConvertibleTo( yType, yDim, false );
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
        o << (*i).first << " = " << (*i).second->getClass() << std::endl;
    }
}


