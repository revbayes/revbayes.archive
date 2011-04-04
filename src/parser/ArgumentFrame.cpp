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
#include "VariableSlot.h"

#include <sstream>


/** Construct empty argument frame with NULL parent */
ArgumentFrame::ArgumentFrame( void ) : Frame(), arguments() {
}


/** Construct empty argument frame with parent */
ArgumentFrame::ArgumentFrame( Frame* parentFr ) : Frame( parentFr ), arguments() {
}


/** Copy constructor. We need to set the frame of the variable slots. */
ArgumentFrame::ArgumentFrame( const ArgumentFrame& x ) :  Frame( x ), arguments( x.arguments ) {


    for ( std::vector<std::pair<std::string, VariableSlot> >::iterator i = arguments.begin(); i ! = arguments.end(); i++ )
       (*i).second.setFrame( this );
}


/** Index operator with bound checking */
VariableSlot& ArgumentFrame::operator[]( size_t i ) {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    return arguments[i].second;
}


/** Index operator (const) with bound checking */
const VariableSlot& ArgumentFrame::operator[]( size_t i ) const {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    return arguments[i].second;
}


/** String index operator with bound checking */
VariableSlot& ArgumentFrame::operator[]( const std::string& name ) {

    std::vector<std::pair<std::string, VariableSlot> >::iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return (*it).second;
}


/** String index operator (const) with bound checking */
const VariableSlot& ArgumentFrame::operator[]( const std::string& name ) const {

    std::vector<std::pair<std::string, VariableSlot> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return (*it).second;
}


/** Add argument slot without name to frame */
void ArgumentFrame::addArgumentSlot( VariableSlot& slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot>( "", slot ) );
}


/** Add argument slot with name to frame */
void ArgumentFrame::addArgumentSlot( const std::string& name, VariableSlot& slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot>( name, slot ) );
}


/** Clear arguments */
void ArgumentFrame::clear( void ) {

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


/** Get index of named argument */
size_t ArgumentFrame::getIndex( const std::string& name ) const {

    std::vector<std::pair<std::string, VariableSlot> >::const_iterator it;
    for ( it = arguments.begin(); it != arguments.end(); it++ ) {
        if ( (*it).first == name )
            break;
    }

    if ( it == arguments.end() )
        throw RbException( "Argument " + name + " does not exist" );

    return it - arguments.begin();
}


/** Push back argument slot without name onto frame */
void ArgumentFrame::push_back( VariableSlot& slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot>( "", slot ) );
}


/** Push back argument slot with name onto frame */
void ArgumentFrame::push_back( const std::string& name, VariableSlot& slot ) {

    arguments.push_back( std::pair<std::string, VariableSlot>( name, slot ) );
}


/** Set label of argument i */
void ArgumentFrame::setArgumentLabel( size_t i, const std::string& name ) {

    if ( i > arguments.size() )
        throw RbException( "Argument index out of range" );

    arguments[i].first = name;
}


/** Print value for user */
void ArgumentFrame::printValue(std::ostream& o) const {

    std::vector<std::pair<std::string, VariableSlot> >::const_iterator i;
    for ( i = arguments.begin(); i != arguments.end(); i++ ) {
        o << (*i).first << " = ";
        (*i).second.printValue(o);
        o << std::endl;
    }
}


/** Complete info about object to string */
std::string ArgumentFrame::richInfo( void ) const {

    std::ostringstream o;
    o << "Frame:" << std::endl;
    printValue( o );

    return o.str();
}

