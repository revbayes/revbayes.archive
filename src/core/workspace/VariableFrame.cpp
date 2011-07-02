/**
 * @file
 * This file contains the implementation of VariableFrame, which
 * is used to hold information about an evaluation or
 * execution frame.
 *
 * @brief Implementation of VariableFrame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ContainerNode.h"
#include "VariableFrame.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"      // For PRINTF
#include "VariableContainer.h"
#include "Workspace.h"

#include <sstream>


/** Construct frame with NULL parent */
VariableFrame::VariableFrame( void ) : Frame( NULL ), variableTable() {
}


/** Construct frame with parent */
VariableFrame::VariableFrame( Frame* parentFr ) : Frame( parentFr ), variableTable() {
}


/** Copy constructor. We need to clone the slots and set the frame of the clones. */
VariableFrame::VariableFrame( const VariableFrame& x ) :  Frame( x ), variableTable( x.variableTable ) {

    for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ ) {
        (*i).second = (*i).second->clone();
        (*i).second->setFrame( this );
    }
}


/** Destructor */
VariableFrame::~VariableFrame( void ) {

    for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ )
        delete (*i).second;
}


/** Assignment operator */
VariableFrame& VariableFrame::operator=( const VariableFrame& x ) {

    if ( this != &x ) {
    
        for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ )
            delete (*i).second;
        
        Frame::operator=( x );
        variableTable = x.variableTable;

        for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ ) {
            (*i).second = (*i).second->clone();
            (*i).second->setFrame( this );
        }
        
    }

    return (*this);
}


/** Index operator to variable slot from string */
VariableSlot& VariableFrame::operator[]( const std::string& name ) {

    std::map<std::string, VariableSlot*>::iterator it = variableTable.find(name);
    if ( variableTable.find(name) == variableTable.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->operator []( name );
        else
            throw RbException( RbException::MISSING_VARIABLE, "Variable " + name + " does not exist" );
    }

    PRINTF( "Retrieving %s %s from frame\n", it->second->getTypeSpec().toString().c_str(), name.c_str() );

    return *(*it).second;
}


/** Index operator (const) to variable slot from string */
const VariableSlot& VariableFrame::operator[]( const std::string& name ) const {

    std::map<std::string, VariableSlot*>::const_iterator it = variableTable.find( name );
    if ( variableTable.find(name) == variableTable.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->operator []( name );
        else
            throw RbException( RbException::MISSING_VARIABLE, "Variable " + name + " does not exist" );
    }

    PRINTF( "Retrieving %s %s from frame\n", it->second->getTypeSpec().toString().c_str(), name.c_str() );

    return *(*it).second;
}


/** Add variable to frame */
void VariableFrame::addVariable( const std::string& name, const TypeSpec& typeSp, DAGNode* variable ) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if ( variableTable.find( name ) != variableTable.end() )
        throw RbException( "Variable " + name + " already exists in frame" );

    /* Create the slot */
    VariableSlot* theSlot = new VariableSlot( typeSp, variable );
    theSlot->setFrame( this );

    /* Insert new slot in variable table */
    variableTable.insert( std::pair<std::string, VariableSlot*>( name, theSlot ) );

    PRINTF( "Inserted %s %s in frame\n", theSlot->getTypeSpec().toString().c_str(), name.c_str() );
}


/** Add variable slot to frame */
void VariableFrame::addVariableSlot( const std::string& name, const TypeSpec& typeSp ) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if ( variableTable.find( name ) != variableTable.end() )
        throw RbException( "Variable " + name + " already exists in frame" );

    /* Create the slot */
    VariableSlot* theSlot = new VariableSlot( typeSp );
    theSlot->setFrame( this );

    /* Insert new slot in variable table */
    variableTable.insert( std::pair<std::string, VariableSlot*>( name, theSlot ) );

    PRINTF( "Inserted %s %s in frame\n", theSlot->getTypeSpec().toString().c_str(), name.c_str() );
}


/** Clone entire environment, except base frame (it always stays the same) */
VariableFrame* VariableFrame::cloneEnvironment( void ) const {

    VariableFrame* newEnv = clone();
    if ( newEnv->parentFrame != NULL && newEnv->parentFrame->getParentFrame() != NULL )
        newEnv->parentFrame = newEnv->parentFrame->cloneEnvironment();

    return newEnv;
}


/** Erase variable */
void VariableFrame::eraseVariable( const std::string& name ) {

    std::map<std::string, VariableSlot*>::iterator it = variableTable.find( name );
    if ( it == variableTable.end() )
        throw RbException( RbException::MISSING_VARIABLE, "Variable " + name + " does not exist in frame" );

    PRINTF( "Erasing %s %s from frame\n", name.c_str(), it->second->getTypeSpec().toString().c_str() );

    delete ( (*it).second );
    variableTable.erase( it );
}


/** Does variable exist in the environment (current frame and enclosing frames)? */
bool VariableFrame::existsVariable( const std::string& name ) const {

    if ( variableTable.find(name) == variableTable.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->existsVariable( name );
        else
            return false;
    }

    return true;
}


/** Get slot name */
const std::string& VariableFrame::getSlotName( const VariableSlot* theSlot ) const {

    for ( VariableTable::const_iterator i = variableTable.begin(); i != variableTable.end(); i++) {
    
        if ( (*i).second == theSlot )
            return (*i).first;
    }

    throw RbException( "No name available: Specified variable slot does not exist in frame" );
}


/** Print value for user */
void VariableFrame::printValue( std::ostream& o ) const {

    VariableTable::const_iterator it;
    for ( it = variableTable.begin(); it != variableTable.end(); it++) {
        o << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }
}


/** Complete info about object to string */
std::string VariableFrame::richInfo( void ) const {

    std::ostringstream o;
    o << "VariableFrame:" << std::endl;
    printValue( o );

    return o.str();
}

