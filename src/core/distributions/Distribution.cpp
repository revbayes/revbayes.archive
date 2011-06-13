/**
 * @file
 * This file contains the partial implementation of Distribution, the atbstract
 * base class for distributions in RevBayes.
 *
 * @brief Partial implementation of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface Distribution
 *
 * $Id$
 */

#include "ArgumentFrame.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "Distribution.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbNames.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"


/** Constructor with inheritance for member rules */
Distribution::Distribution( const MemberRules& memberRules ) : MemberObject( memberRules ) {
}


/** Map direct method calls to internal class methods. */
DAGNode* Distribution::executeOperation( const std::string& name, ArgumentFrame& args ) {

    if ( name == "lnPdf" ) {

        return new ConstantNode( new RealPos( lnPdf( args[0].getValue() ) ) );
    }
    else if ( name == "pdf" ) {

        return new ConstantNode( new RealPos( pdf  ( args[0].getValue() ) ) );
    }
    else if ( name == "rv" ) {

        RbObject* draw = rv();

        if ( draw->isType( Container_name ) )
            return new ContainerNode( static_cast<Container*>( draw ) );
        else if ( draw->isType( MemberObject_name ) )
            return new MemberNode( static_cast<MemberObject*>( draw ) );
        else
            return new ConstantNode( draw );
    }
    else
        return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& Distribution::getClass( void ) const {

    static VectorString rbClass = VectorString( Distribution_name ) + MemberObject::getClass();
    return rbClass;
}


/** Get methods */
const MethodTable& Distribution::getMethods( void ) const {

    static MethodTable      methods;
    static ArgumentRules    lnPdfArgRules;
    static ArgumentRules    pdfArgRules;
    static ArgumentRules    rvArgRules;
    static bool             methodsSet = false;

    if ( !methodsSet ) {

        lnPdfArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        lnPdfArgRules.push_back( new ValueRule    ( "x", RbObject_name    ) );

        pdfArgRules.push_back  ( new ReferenceRule( "",  MemberObject_name ) );
        pdfArgRules.push_back  ( new ValueRule    ( "x", RbObject_name     ) );

        rvArgRules.push_back   ( new ReferenceRule( "",  MemberObject_name ) );

        methods.addFunction( "lnPdf", new MemberFunction( Real_name    , lnPdfArgRules ) );
        methods.addFunction( "pdf",   new MemberFunction( Real_name    , pdfArgRules   ) );
        methods.addFunction( "rv",    new MemberFunction( RbObject_name, rvArgRules    ) );

        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );

        methodsSet = true;
    }

    return methods;
}

