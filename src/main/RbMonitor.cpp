

#include "DAGNode.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbMonitor.h"
#include "RbNames.h"
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

RbObject& RbMonitor::operator=(const RbObject& obj) {
	try {
        // Use built-in fast down-casting first
        const RbMonitor& x = dynamic_cast<const RbMonitor&> (obj);

        RbMonitor& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbMonitor& x = dynamic_cast<const RbMonitor&> (*(obj.convertTo(RbNames::Monitor::name)));

            RbMonitor& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Monitor::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbMonitor& RbMonitor::operator=(const RbMonitor& o) {

	(*node) = (*o.node);
	outStrm = o.outStrm;
	sampleFrequency = o.sampleFrequency;
	return *this;
}

void RbMonitor::monitor(int i) {

	if (i % sampleFrequency == 0)
		{
		(*outStrm) << node->getValue()->toString() << std::endl;
		
		}
}


