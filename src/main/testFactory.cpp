/*
 * testFactory.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include "testFactory.h"

testFactory::testFactory() {
	// TODO Auto-generated constructor stub

}

testFactory::~testFactory() {
	// TODO Auto-generated destructor stub
}

int main(int argc, char **argv) {
	// load DistNormal
	std::string filename = "Distribution";

	TheKernel.loadPlugin(filename);

	tryOpenArchive(TheKernel.getStorageServer());

	listGraphicsDrivers(TheKernel.getGraphicsServer());

	// create Distribution object from DistNormal
}
