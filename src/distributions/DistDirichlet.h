#ifndef DistDirichlet_H
#define DistDirichlet_H

#include "Distribution.h"

#ifdef DISTDIRICHLET_PLUGIN_EXPORTS
  #define DISTDIRICHLET_PLUGIN_API __declspec(dllexport)
#else
  #define DISTDIRICHLET_PLUGIN_API __declspec(dllimport)
#endif

class RbDataType;
class Vector;
class DistDirichlet : Distribution {

public:
	RbDataType* rv(void);
	double pdf(void);
	double lnPdf(void);

private:
	Vector *value;
	Vector *alpha;
};

#endif;
