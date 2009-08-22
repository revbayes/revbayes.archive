#ifndef DistDirichlet_H
#define DistDirichlet_H

#include "Distribution.h"

class Vector;
class DistDirichlet : Distribution {

public:
	double drawRandomValue();
	/*DistDirichlet(Vector* a);
	RbDataType expectedValue(void);
	RbDataType drawRandomValue(void);
	double pdf(RbDataType& value);
	double logpdf(RbDataType& value);
	double cdf(RbDataType& value);
	double quantile(RbDataType& value);*/

private:
	Vector *value;
	Vector *alpha;
};

#endif;