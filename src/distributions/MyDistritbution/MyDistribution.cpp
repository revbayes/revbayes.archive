#include "../../main/Distribution.h"
#include "MyDistribution.h"
#include "../../main/RbDataType.h"


MyDistribution::MyDistribution(void) {
	name = "MyDistribution";
}

PLUGIN_API Distribution* MyDistribution::createInstance(){
	return new MyDistribution();
}

RbDataType* MyDistribution::execute(void) {

	return NULL; // TEMP: Eventually this should call the apropriate function (pdf,cdf,...)  of "MyDistribution"
}

RbDataType MyDistribution::getDataType(void) {

	return NULL; // TEMP: Eventually this should return the data type of "MyDistribution"
}

RbDataType* MyDistribution::rv(void) {

	return NULL; // TEMP: Eventually this should return a random value of "MyDistribution"
}

RbVector* MyDistribution::pdf(void) {

	return 0.0; // TEMP: Eventually this should return the probability for "MyDistribution"
}

RbVector* MyDistribution::lnPdf(void) {

	return 0.0; // TEMP: Eventually this should return the log probability for "MyDistribution"
}

/// Retrieve the engine version we're going to expect
extern "C" PLUGIN_API int getEngineVersion() {
  return 1;
}

/// Tells us to register our distribution to a factory
extern "C" PLUGIN_API void registerPlugin(Factory &fact) {
  fact.getDistributionFactory().addDistribution(
    auto_ptr<DistributionFactory::Distribution>(new MyDistribution())
  );

}
