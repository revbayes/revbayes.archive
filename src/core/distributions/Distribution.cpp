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

#include "ConstantNode.h"
#include "Distribution.h"
#include "Environment.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"


/** Constructor with inheritance for member rules */
Distribution::Distribution( const MemberRules& memberRules ) : ConstantMemberObject( memberRules ) {
}


/** Map direct method calls to internal class methods. */
const RbLanguageObject& Distribution::executeOperationSimple( const std::string& name, Environment& args ) {

    if ( name == "lnPdf" ) {
        
        functionValueLnPdf = Real( lnPdf( args[1].getValue() ) );
        return functionValueLnPdf;
    }
    else if ( name == "pdf" ) {

        functionValuePdf = RealPos( pdf( args[1].getValue() ) );
        return functionValuePdf;
    }
    else if ( name == "rv" ) {

        return rv();
    }
    else
        return MemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& Distribution::getClass( void ) const {

    static VectorString rbClass = VectorString( Distribution_name ) + MemberObject::getClass();
    return rbClass;
}


/** Get methods */
const MethodTable& Distribution::getMethods( void ) const {

    static MethodTable methods = MethodTable();
    static ArgumentRules*    lnPdfArgRules = new ArgumentRules();
    static ArgumentRules*    pdfArgRules = new ArgumentRules();
    static ArgumentRules*    rvArgRules = new ArgumentRules();
    static bool             methodsSet = false;

    if ( !methodsSet ) {

        lnPdfArgRules->push_back( new ValueRule( "x", RbObject_name ) );

        pdfArgRules->push_back( new ValueRule( "x", RbObject_name ) );

        methods.addFunction( "lnPdf", new MemberFunction( Real_name    , lnPdfArgRules ) );
        methods.addFunction( "pdf",   new MemberFunction( Real_name    , pdfArgRules   ) );
        methods.addFunction( "rv",    new MemberFunction( RbObject_name, rvArgRules    ) );

        methods.setParentTable( &MemberObject::getMethods() );

        methodsSet = true;
    }

    return methods;
}

