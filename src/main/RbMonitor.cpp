

#include "DAGNode.h"
#include "RbException.h"
#include "RbMonitor.h"


RbMonitor::RbMonitor(std::ostream& o, DAGNode* n, RbInt& f) {

}

RbMonitor::RbMonitor(const RbMonitor& m) {

}

RbMonitor::~RbMonitor(void) {

}

RbObject* RbMonitor::clone() const {
    //!< Clone object
}

bool RbMonitor::equals(const RbObject* obj) const
{             //!< Equals comparison

void RbMonitor::print(std::ostream& o) const {
    //!< Print complete object info
}

void RbMonitor::printValue(std::ostream& o) const {
    //!< Print value (for user)
}

std::string RbMonitor::toString(void) const {
    //!< General info on object
}

RbObject& RbMonitor::operator=(const RbObject& o) {

}

RbMonitor& RbMonitor::operator=(const RbMonitor& o) {

}

void RbMonitor::monitor(int i) {

}


