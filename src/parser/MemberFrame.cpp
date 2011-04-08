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
MemberFrame::MemberFrame( void ) : VariableFrame( NULL ), members() {
}


/** Construct frame with parent */
MemberFrame::MemberFrame( Frame* parentFr ) : VariableFrame( parentFr ), members() {
}


/** Copy constructor. We need to set the frame of the variable slots. */
MemberFrame::MemberFrame( const MemberFrame& x ) :  VariableFrame( x ), members( x.members ) {

    for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ )
        (*i).second.setFrame( this );
}


/** Index access to variable slot based on adding sequence */
VariableSlot& MemberFrame::operator[]( const size_t i ) {

    if ( i < 0 || i >= members.size() )
        throw RbException( "Index out of range" );

    return VariableFrame::operator[]( members[i] );
}


/** Index access to variable slot based on adding sequence (const) */
const VariableSlot& MemberFrame::operator[]( const size_t i ) const {

    if ( i < 0 || i >= members.size() )
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


/** Push back new member based on argument rule */
void MemberFrame::push_back( const ArgumentRule& rule ) {

    std::string name = (*i)->getArgLabel();

    if ( (*i)->isConstant() ) {
        
        addConstant( name, (*i)->getArgTypeSpec(), (*i)->getVariable() );
    }
    else if ( (*i)->hasDefault() ) {
    
        addVariable( name, (*i)->getArgTypeSpec(), (*i)->getDefault() );
    }
    else {

        addVariableSlot( name, (*i)->getArgTypeSpec() );
    }
}


/** Complete info about object to string */
std::string MemberFrame::richInfo( void ) const {

    std::ostringstream o;
    o << "MemberFrame:" << std::endl;
    printValue( o );

    return o.str();
}

