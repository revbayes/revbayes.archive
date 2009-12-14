

#include "DAGNode.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbMonitor.h"
#include "RbNames.h"
#include "StringVector.h"

#include <iostream>



RbMonitor::RbMonitor(std::ostream& o, DAGNode* n, RbInt& f) {

	node = n;
	outStrm = &o;
	sampleFrequency = f.getValue();
}

RbMonitor::RbMonitor(const RbMonitor& m) {

	(*node) = (*m.node);
	outStrm = m.outStrm;
	sampleFrequency = m.sampleFrequency;
}

RbMonitor::~RbMonitor(void) {

}

RbObject* RbMonitor::clone() const {
    //!< Clone object
    RbMonitor* x = new RbMonitor(*this);
    return (RbObject*) x;
}

bool RbMonitor::equals(const RbObject* obj) const
{             //!< Equals comparison

	return false;
}


/** Get class vector describing type of object */
const StringVector& RbMonitor::getClass() const {

    static StringVector rbClass = StringVector(RbMonitor_name) + RbComplex::getClass();
    return rbClass;
}


void RbMonitor::print(std::ostream& o) const {
    //!< Print complete object info
}

void RbMonitor::printValue(std::ostream& o) const {
    //!< Print value (for user)
}

std::string RbMonitor::toString(void) const {
    //!< General info on object
    return "";
}


void RbMonitor::monitor(int i) {

	if (i % sampleFrequency == 0)
		{
		(*outStrm) << node->getValue()->toString() << std::endl;
		
		}
}


