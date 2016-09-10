#include <iomanip>
#include <iostream>
#include "NodeVal.h"




NodeVals::NodeVals(boost::dynamic_bitset<>& bf) {

	partDescription = boost::dynamic_bitset<>( bf.size() );
    partDescription = bf;
	count = 0;
}

NodeVals::NodeVals(void) {

	count = 0;
}

NodeVals::~NodeVals(void) {

}

bool NodeVals::operator==(const NodeVals& a) const {

    if ( count == a.count)
        return true;
	return false;
}

bool NodeVals::operator<(const NodeVals& a) const {

    if ( a.count < count)
        return true;
	return false;
}

void NodeVals::addSample(void) {

	count++;
}

bool NodeVals::isSingleton(void) {

    if (partDescription.count() == 1)
        return true;
    else if (partDescription.size() - partDescription.count() == 1)
        return true;
    return false;
}

void NodeVals::normalizeBitVector(void) {

    if (partDescription[0] == true)
        partDescription.flip();
}


