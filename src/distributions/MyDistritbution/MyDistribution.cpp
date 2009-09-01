#include "../../main/Distribution.h"
#include "MyDistribution.h"
#include "../../main/RbDataType.h"


MyDistribution::MyDistribution(void) {
	name = "MyDistribution";
}

PLUGIN_API Distribution* MyDistribution::createInstance(){
	return new MyDistribution();
}

PLUGIN_API RbDataType* MyDistribution::rv(void) {

	return NULL; // TEMP: Eventually this should return a random value of "MyDistribution"
}

PLUGIN_API double MyDistribution::pdf(void) {

	return 0.0; // TEMP: Eventually this should return the probability for "MyDistribution"
}

PLUGIN_API double MyDistribution::lnPdf(void) {

	return 0.0; // TEMP: Eventually this should return the log probability for "MyDistribution"
}

/// Retrieve the engine version we're going to expect
extern "C" PLUGIN_API int getEngineVersion() {
  return 1;
}

/// Tells us to register our distribution to a factory
extern "C" PLUGIN_API void registerPlugin(Factory &fac) {
  fac.getDistributionContainer().addDistribution(
    auto_ptr<DistributionContainer::Distribution>(new MyDistribution())
  );

}
