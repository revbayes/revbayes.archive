/**
 * @file
 * This file contains the implementation of Frame, which
 * is the abstract base class of frames and workspaces.
 *
 * @brief Implementation of Frame
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

#include "Frame.h"
#include "VariableSlot.h"


/** Construct frame with NULL parent */
Frame::Frame(void) : parentFrame(NULL) {
}


/** Construct frame with parent */
Frame::Frame(Frame* parentFr) : parentFrame(parentFr) {
}


/** Clone entire environment, except base frame (it always stays the same) */
Frame* Frame::cloneEnvironment( void ) const {

    Frame* newEnv = clone();
    if ( newEnv->parentFrame != NULL && newEnv->parentFrame->getParentFrame() != NULL )
        newEnv->parentFrame = newEnv->parentFrame->cloneEnvironment();

    return newEnv;
}


/** Get reference, alternative method */
DAGNode* Frame::getReference( const std::string& name ) const {

    return operator[]( name ).getReference();
}


//!< Get value, alternative method */
const RbObject* Frame::getValue( const std::string& name ) const {

    return operator[]( name ).getValue();
}


//!< Get variable, alternative method */
const DAGNode* Frame::getVariable( const std::string& name ) const {

    return operator[]( name ).getVariable();
}

