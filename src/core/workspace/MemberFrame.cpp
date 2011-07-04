/**
 * @file
 * This file contains the implementation of MemberFrame, which is
 * used to hold member variables in MemberObject data types.
 *
 * @brief Implementation of MemberFrame
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

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "MemberFrame.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"      // For PRINTF
#include "VariableContainer.h"
#include "Workspace.h"

#include <algorithm>
#include <sstream>


/** Construct frame with NULL parent */
MemberFrame::MemberFrame( void ) : VariableFrame( NULL ), members(), owner( NULL ) {
}


/** Construct frame with parent */
MemberFrame::MemberFrame( Frame* parentFr ) : VariableFrame( parentFr ), members(), owner( NULL ) {
}


/** Index operator to variable slot from string */
VariableSlot& MemberFrame::operator[]( const std::string& name ) {

    std::map<std::string, VariableSlot*>::iterator it = variableTable.find(name);
    if ( variableTable.find(name) == variableTable.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->operator []( name );
        else {
            if ( owner != NULL )
                throw RbException( "Member variable " + owner->getName() + "." + name + " does not exist" );
            else
                throw RbException( "Member variable <null>." + name + " does not exist" );
        }
    }

    PRINTF( "Retrieving %s %s from frame\n", it->second->getTypeSpec().toString().c_str(), name.c_str() );

    return *(*it).second;
}


/** Index operator (const) to variable slot from string */
const VariableSlot& MemberFrame::operator[]( const std::string& name ) const {

    std::map<std::string, VariableSlot*>::const_iterator it = variableTable.find( name );
    if ( variableTable.find(name) == variableTable.end() ) {
        if ( parentFrame != NULL )
            return parentFrame->operator []( name );
        else {
            if ( owner != NULL )
                throw RbException( "Member variable " + owner->getName() + "." + name + " does not exist" );
            else
                throw RbException( "Member variable <null>." + name + " does not exist" );
        }
    }

    PRINTF( "Retrieving %s %s from frame\n", it->second->getTypeSpec().toString().c_str(), name.c_str() );

    return *(*it).second;
}


/** Index access to variable slot based on adding sequence */
VariableSlot& MemberFrame::operator[]( const size_t i ) {

    if ( i >= members.size() )
        throw RbException( "Index out of range" );

    return VariableFrame::operator[]( members[i] );
}


/** Index access to variable slot based on adding sequence (const) */
const VariableSlot& MemberFrame::operator[]( const size_t i ) const {

    if ( i >= members.size() )
        throw RbException( "Index out of range" );

    return VariableFrame::operator[]( members[i] );
}


/** Add variable slot to frame */
void MemberFrame::addVariableSlot( const std::string& name, const TypeSpec& typeSp ) {

    VariableFrame::addVariableSlot( name, typeSp );

    members.push_back( name );
}


/** Erase variable */
void MemberFrame::eraseVariable( const std::string& name ) {

    VariableFrame::eraseVariable( name );
    
    members.erase( std::find(members.begin(), members.end(), name ) );

}


/** Clear member frame */
void MemberFrame::clear( void ) {

    for ( VariableTable::iterator i=variableTable.begin(); i!=variableTable.end(); i++ )
        delete ( i->second );
    variableTable.clear();
    members.clear();
}


/** Get index of named argument */
size_t MemberFrame::getIndex( const std::string& name ) const {

    std::vector<std::string>::const_iterator it;
    it = std::find( members.begin(), members.end(), name );
    
    if ( it == members.end() )
        throw RbException( "Member " + name + " does not exist" );

    return it - members.begin();
}


/** Get name of indexed argument */
std::string MemberFrame::getName( const size_t index ) const {

    if ( index > members.size() )
        throw RbException( "Index out of range" );

    return members[index];
}


/**
 * Is member frame same or parent of otherFrame? We use this function
 * to decide when a reference from a slot in otherFrame to a variable in
 * this member frame is safe, and when it is not. The only time we know for
 * sure that it is safe is when this frame is identical to, or a parent of,
 * the otherFrame. Unlike a regular frame, we need to ask the owner if
 * we are at the base of a potential stack of member frames.
 */
bool MemberFrame::isSameOrParentOf( Frame* otherFrame ) const {

    if ( this == otherFrame )
        return true;

    if ( parentFrame != NULL )
        return parentFrame->isSameOrParentOf( otherFrame );

    if ( owner != NULL )
        return owner->isPermanent( otherFrame );

    return true;
}


/** Push back new member based on argument rule */
void MemberFrame::push_back( const std::string& name, VariableSlot* slot ) {

    variableTable.insert( std::pair<std::string, VariableSlot*>( name, slot ) );
    members.push_back( name );

}


/** Complete info about object to string */
std::string MemberFrame::richInfo( void ) const {

    std::ostringstream o;
    o << "MemberFrame:" << std::endl;
    printValue( o );

    return o.str();
}

