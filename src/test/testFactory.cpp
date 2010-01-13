/*
 * testFactory.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include "../main/Distribution.h"
#include "../factory/Factory.h"
#include "../factory/DistributionFactory.h"

testFactory::testFactory() {
	// TODO Auto-generated constructor stub

}

testFactory::~testFactory() {
	// TODO Auto-generated destructor stub
}

/// Test which tries to open a zip archive
void tryCreateDistribution(DistributionFactory &distFactory) {
  try {
	  distFactory.getDistribution("MyDistribution");

    cout << "\tDistribution successfully created" << endl;
  }
  catch(const exception &Ex) {
    cout << "\tError creating distribution: " << Ex.what() << endl;
  }

  cout << endl;
}

int main(int argc, char **argv) {
	// load MyDistribution
	std::string filename = "MyDistribution";

	Factory fact;

    // Now load the plugins
    cout << "Loading plugins..." << endl;
	fact.loadPlugin(filename);
    cout << endl;

	tryCreateDistribution(fact.getDistributionFactory());
}
