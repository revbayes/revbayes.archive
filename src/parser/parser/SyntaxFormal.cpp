/**
 * @file
 * This file contains the implementation of SyntaxFormal, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxFormal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "ReferenceRule.h"
#include "SyntaxFormal.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VariableFrame.h"
#include "Workspace.h"

#include <sstream>


/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(RbString* id, SyntaxElement* defaultVal)
    : SyntaxElement(), type(new TypeSpec(RbObject_name)), label(id), defaultExpr(defaultVal) {
}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal)
: SyntaxElement(), type(NULL), label(id), defaultExpr(defaultVal) {

    // Convert to standard string
    const std::string typeString = *typeSpec;

    // Count dimensions and check if reference
    int         nDim        = 0;
    bool        isRef       = false;
    std::string tpName      = std::string();
    for (std::string::const_iterator i=typeString.begin(); i!=typeString.end(); i++) {
        if ((*i) == '[')
            nDim++;
        else if ((*i) == '&')
            isRef = true;
        else if ((*i) != ']')
            tpName += (*i);
    }

    // Create the type specification
    type = new TypeSpec(Workspace::userWorkspace().getTypeSpec(tpName), nDim, isRef);
    
    // Avoid memory leak
    delete typeSpec;
}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x)
    : SyntaxElement(x) {

    type        = new TypeSpec(x.type->getType(), x.type->getDim(), x.type->isReference());
    label       = new RbString(*(x.label));
    defaultExpr = x.defaultExpr->clone();
}


/** Destructor deletes pointer members */
SyntaxFormal::~SyntaxFormal() {
    
    delete type;
    delete label;
    delete defaultExpr;
}


/** Assignment operator */
SyntaxFormal& SyntaxFormal::operator=(const SyntaxFormal& x) {

    if (&x != this) {

        delete type;
        delete label;
        delete defaultExpr;

        SyntaxElement::operator=(x);

        type        = new TypeSpec(x.type->getType(), x.type->getDim(), x.type->isReference());
        label       = new RbString(*(x.label));
        defaultExpr = x.defaultExpr->clone();
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxFormal::briefInfo () const {

    std::ostringstream   o;

    if (defaultExpr == NULL)
        o << "SyntaxFormal:  '" << type->toString() << " " << std::string(*label);
    else {
        o << "SyntaxFormal:  '" << type->toString() << " " << std::string(*label) << " = ";
        o << defaultExpr->briefInfo();
    }

    return o.str();
}


/** Clone syntax element */
SyntaxFormal* SyntaxFormal::clone () const {

    return new SyntaxFormal(*this);
}


/** Make argument rule from element */
ArgumentRule* SyntaxFormal::getArgumentRule(VariableFrame* frame) const {

    if ( type->isReference() ) {
    
        if ( defaultExpr == NULL )
            return new ReferenceRule( *label, type->getType(), type->getDim() );
        else
            return new ReferenceRule( *label, type->getType(), defaultExpr->getValue( frame ) );
    }
    else {

        if (defaultExpr == NULL)
            return new ValueRule(*label, *type);
        else
            return new ValueRule(*label, *type, defaultExpr->getValue(frame));
    }
}


/** Convert element to DAG node (not applicable so return NULL) */
DAGNode* SyntaxFormal::getDAGNodeExpr(VariableFrame* frame) const {

    return NULL;
}


/** Get semantic value (not applicable so return NULL) */
DAGNode* SyntaxFormal::getValue(VariableFrame* frame) const {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxFormal::print(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << type->toString() << std::endl;
    o << "label       = " << *label << std::endl;
    o << "defaultExpr = " << defaultExpr->briefInfo() << std::endl;
}


