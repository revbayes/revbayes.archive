#include "Distribution.h"
#include "DistDirichlet.h"
#include "RbDataType.h"
#include "RbMath.h"
#include <cmath>
#include <iostream>



RbDataType* DistDirichlet::rv(void) {

	return NULL; // TEMP: Eventually this should return a 
}

double DistDirichlet::pdf(void) {
	
	return 0.0; // TEMP: Eventually this should return the dirichlet probability
}

double DistDirichlet::lnPdf(void) {

	return 0.0; // TEMP: Eventually this should return the dirichlet log probability
}

/// Retrieve the engine version we're going to expect
extern "C" DISTDIRICHLET_PLUGIN_API int getEngineVersion() {
  return 1;
}

/// Tells us to register our functionality to an engine kernel
extern "C" DISTDIRICHLET_PLUGIN_API void registerPlugin(RevBayesMain &rb) {
  rb.getDistributionContainer().addDistribution(
    auto_ptr<DistributionContainer::Distribution>(new DistDirichlet())
  );
}
