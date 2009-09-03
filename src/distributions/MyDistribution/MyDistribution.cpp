#include "../../main/Distribution.h"
#include "../../factory/DistributionFactory.h"
#include "MyDistribution.h"
#include "../../main/RbDataType.h"
#include "../../main/RbInt.h"


MyDistribution::MyDistribution(void) {
	setName("MyDistribution");
}

PLUGIN_API Distribution* MyDistribution::createInstance(void){
	MyDistribution* dist = new MyDistribution();
	return dist;
}

RbDataType* MyDistribution::execute(void) {

	return NULL; // TEMP: Eventually this should call the apropriate function (pdf,cdf,...)  of "MyDistribution"
}

RbDataType MyDistribution::getDataType(void) {

	return RbInt(42); // TEMP: Eventually this should return the data type of "MyDistribution"
}

RbDataType* MyDistribution::rv(void) {

	return NULL; // TEMP: Eventually this should return a random value of "MyDistribution"
}

RbDataType* MyDistribution::pdf(void) {

	return NULL; // TEMP: Eventually this should return the probability for "MyDistribution"
}

RbDataType* MyDistribution::lnPdf(void) {

	return NULL; // TEMP: Eventually this should return the log probability for "MyDistribution"
}

/// Retrieve the engine version we're going to expect
extern "C" PLUGIN_API int getEngineVersion() {
  return 1;
}

/// Tells us to register our distribution to a factory
extern "C" PLUGIN_API void registerPlugin(Factory &fact) {
  fact.getDistributionFactory().addDistribution(new MyDistribution());

}
