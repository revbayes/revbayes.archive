/**
 * @file
 * This file contains the implementation of SyntaxFormal, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxFormal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxFormal.h"

#include <sstream>


/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(RbString* id, SyntaxElement* defaultVal)
    : SyntaxElement(), type(new RbString(RbObject_name)), label(id), defaultExpr(defaultVal) {

    if (defaultExpr != NULL && !defaultExpr->isConstExpr())
        throw(RbException("Default value is not a constant expression"));
}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal)
    : SyntaxElement(), type(typeSpec), label(id), defaultExpr(defaultVal) {

    if (defaultExpr != NULL && !defaultExpr->isConstExpr())
        throw(RbException("Default value is not a constant expression"));
}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x)
    : SyntaxElement(x) {

    type        = new RbString(*(x.type));
    label       = new RbString(*(x.label));
    defaultExpr = x.defaultExpr->clone();
}


/** Destructor deletes members */
SyntaxFormal::~SyntaxFormal() {
    
    delete type;
    delete label;
    delete defaultExpr;
}


/** Return brief info about object */
std::string SyntaxFormal::briefInfo () const {

    std::ostringstream   o;

    if (defaultExpr == NULL)
        o << "SyntaxFormal:  '" << std::string(*type) << " " << std::string(*label);
    else if (defaultExpr->isConstExpr()) {
        o << "SyntaxFormal:  '" << std::string(*type) << " " << std::string(*label) << " = ";
        defaultExpr->getValue()->printValue(o);
    }
    else
        o << "SyntaxFormal:  '" << std::string(*type) << " " << std::string(*label) << " = <error>";

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxFormal::clone () const {

    return (SyntaxElement*)(new SyntaxFormal(*this));
}


/** Equals comparison */
bool SyntaxFormal::equals(const SyntaxElement* elem) const {

	const SyntaxFormal* p = dynamic_cast<const SyntaxFormal*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && type->equals(p->type);
    result = result && label->equals(p->label);
    result = result && defaultExpr->equals(p->defaultExpr);

    return result;
}


/** Make argument rule from element */
// TODO: Add dimensions
ArgumentRule* SyntaxFormal::getArgumentRule(Frame* frame) const {

    if (defaultExpr == NULL)
        return new ArgumentRule(*label, *type);
    else
        return new ArgumentRule(*label, *type, 0, defaultExpr->getValue(frame));
}


/** Convert element to DAG node (not applicable so return NULL) */
DAGNode* SyntaxFormal::getDAGNode(Frame* frame) const {

    return NULL;
}


/** Get semantic value (not applicable so return NULL) */
RbObject* SyntaxFormal::getValue(Frame* frame) const {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxFormal::print(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << type->toString() << std::endl;
    o << "label       = " << label->toString() << std::endl;
    o << "defaultExpr = " << defaultExpr->briefInfo() << std::endl;
}


