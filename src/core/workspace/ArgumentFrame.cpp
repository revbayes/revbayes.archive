/**
 * @file
 * This file contains the implementation of ArgumentFrame,
 * which is used to hold arguments passed in to functions
 * after processing.
 *
 * @brief Implementation of ArgumentFrame
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

#include "ArgumentFrame.h"
#include "RbException.h"
#include "RbOptions.h"      // For PRINTF
#include "VariableSlot.h"

#include <sstream>
#include <string>
#include <utility>


/** Construct empty argument frame with NULL parent */
ArgumentFrame::ArgumentFrame( void ) : Frame(), arguments() {
}


/** Construct empty argument frame with parent */
ArgumentFrame::ArgumentFrame( Frame* parentFr ) : Frame( parentFr ), arguments() {
}


/**
 * Copy constructor. We need to copy the slots and set the frame of the copies.
 * Unlike a regular slot clone, which would make an independent copy of the variable
 * in some cases, we create an empty clone and make sure it contains the same variable
 * as the original slot. This ensures that RbFunction copies are actually passed in
 * the same variables as the template they are created from.
 */
ArgumentFrame::ArgumentFrame( const ArgumentFrame& x ) :  Frame( x ), arguments( x.arguments ) {

    std::vector<std::pair<std::string, VariableSlot*> >::iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        DAGNode* variable = const_cast<DAGNode*>( (*it).second->getVariable() );
        (*it).second = (*it).second->cloneEmpty();
        (*it).second->replaceArgumentVariable( variable );
        (*it).second->setFrame( this );
    }
}


/** Destructor */
ArgumentFrame::~ArgumentFrame( void ) {

    std::vector<std::pair<std::string, VariableSlot*> >::iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ )
        delete (*it).second;
}


/** Assignment operator. See copy constructor. */
ArgumentFrame& ArgumentFrame::operator=( const ArgumentFrame& x ) {

    if ( this != &x ) {
    
        std::vector<std::pair<std::string, VariableSlot*> >::iterator it;
        for ( it = arguments.begin(); it != arguments.end(); it++ )
            delete (*it).second;
        
        Frame::operator=( x );
        arguments = x.arguments;

        for ( it = arguments.begin(); it != arguments.end(); it++ ) {
            DAGNode* variable = const_cast<DAGNode*>( (*it).second->getVariable() );
            (*it).second = (*it).second->cloneEmpty();
            (*it).second->replaceArgumentVariable( variable );
            (*it).second->setFrame( this );
        }
    }

    return (*this);
}


/** Index operator with bound checking */
VariableSlot& ArgumentFrame::operator[]( size_t i ) {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    return *arguments[i].second;
}


/** Index operator (const) with bound checking */
const VariableSlot& ArgumentFrame::operator[]( size_t i ) const {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    return *arguments[i].second;
}


/** String index operator with bound checking */
VariableSlot& ArgumentFrame::operator[]( const std::string& name ) {

    std::vector<std::pair<std::string, VariableSlot*> >::iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return *(*it).second;
}


/** String index operator (const) with bound checking */
const VariableSlot& ArgumentFrame::operator[]( const std::string& name ) const {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return *(*it).second;
}


/** Clear arguments */
void ArgumentFrame::clear( void ) {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ )
        delete it->second;
    arguments.clear();
}


/** Clone this frame */
ArgumentFrame* ArgumentFrame::clone( void ) const {

    return new ArgumentFrame( *this );
}


/** Clone entire environment, except base frame (it always stays the same) */
ArgumentFrame* ArgumentFrame::cloneEnvironment( void ) const {

    ArgumentFrame* newEnv = clone();
    if ( newEnv->parentFrame != NULL && newEnv->parentFrame->getParentFrame() != NULL )
        newEnv->parentFrame = newEnv->parentFrame->cloneEnvironment();

    return newEnv;
}


/** Does variable exist? */
bool ArgumentFrame::existsVariable( const std::string& name ) const {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->existsVariable( name );
        else
            return false;
    }

    return true;
}


/** Get index of named argument */
size_t ArgumentFrame::getIndex( const std::string& name ) const {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return it - arguments.begin();
}


/** Find slot and return its name */
const std::string& ArgumentFrame::getSlotName(const VariableSlot* slot) const {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).second == slot )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "No name available: Slot does not exist in the argument frame" );

    return (*it).first;
}


/** Print value for user */
void ArgumentFrame::printValue(std::ostream& o) const {

    std::vector<std::pair<std::string, VariableSlot*> >::const_iterator i;
    for ( i = arguments.begin(); i != arguments.end(); i++ ) {
        o << (*i).first << " = ";
        (*i).second->printValue(o);
        o << std::endl;
    }
}


/** Push back argument slot without name onto frame */
void ArgumentFrame::push_back( VariableSlot* slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot*>( "", slot ) );
    slot->setFrame( this );
}


/** Push back argument slot with name onto frame */
void ArgumentFrame::push_back( const std::string& name, VariableSlot* slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot*>( name, slot ) );
    slot->setFrame( this );
}


/** Complete info about object to string */
std::string ArgumentFrame::richInfo( void ) const {

    std::ostringstream o;
    o << "Frame:" << std::endl;
    printValue( o );

    return o.str();
}


/** Set label of argument i */
void ArgumentFrame::setArgumentLabel( size_t i, const std::string& name ) {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    arguments[i].first = name;
}


