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
Distribution::Distribution( RbPtr<const MemberRules> memberRules ) : ConstantMemberObject( memberRules ) {
}


/** Map direct method calls to internal class methods. */
RbPtr<RbLanguageObject> Distribution::executeOperation( const std::string& name, Environment& args ) {

    if ( name == "lnPdf" ) {

        return RbPtr<RbLanguageObject>( new RealPos( lnPdf( (const RbLanguageObject*)args[1]->getValue() ) ) );
    }
    else if ( name == "pdf" ) {

        return RbPtr<RbLanguageObject>( new RealPos( pdf  ( (const RbLanguageObject*)args[1]->getValue() ) ) );
    }
    else if ( name == "rv" ) {

        RbPtr<RbLanguageObject> draw = rv();

        return draw;
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
RbPtr<const MethodTable> Distribution::getMethods( void ) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules>    lnPdfArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules>    pdfArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules>    rvArgRules( new ArgumentRules() );
    static bool             methodsSet = false;

    if ( !methodsSet ) {

        lnPdfArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "x", RbObject_name ) ) );

        pdfArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "x", RbObject_name ) ) );

        methods->addFunction( "lnPdf", RbPtr<RbFunction>( new MemberFunction( Real_name    , lnPdfArgRules ) ) );
        methods->addFunction( "pdf",   RbPtr<RbFunction>( new MemberFunction( Real_name    , pdfArgRules   ) ) );
        methods->addFunction( "rv",    RbPtr<RbFunction>( new MemberFunction( RbObject_name, rvArgRules    ) ) );

        methods->setParentTable( RbPtr<const FunctionTable>( MemberObject::getMethods() ) );

        methodsSet = true;
    }

    return RbPtr<const MethodTable>( methods );
}

