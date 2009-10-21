/*
 * Argument.cpp
 *
 *  Created on: 21 okt 2009
 *      Author: Sebastian
 */

#include <string>

#include "Argument.h"
#include "RbObject.h"
#include "DAGNode.h"

Argument::Argument(std::string& l, DAGNode* n) : RbObject(), label(l) {
    value = n;
}

Argument::~Argument() {
    // TODO Auto-generated destructor stub
}
