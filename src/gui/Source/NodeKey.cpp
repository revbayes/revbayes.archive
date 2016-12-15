#include "NodeKey.h"
#include <iostream>



NodeKey::NodeKey(boost::dynamic_bitset<>& bf) {

	partDescription = boost::dynamic_bitset<>( bf.size() );
    partDescription = bf;
}

NodeKey::~NodeKey(void) {

}

bool NodeKey::operator<(const NodeKey &a) const {

	boost::dynamic_bitset<> myPrt = partDescription;
	boost::dynamic_bitset<> aPrt  = a.partDescription;
    return (myPrt > aPrt);
}

void NodeKey::print(void) {

	std::cout << partDescription;
}

