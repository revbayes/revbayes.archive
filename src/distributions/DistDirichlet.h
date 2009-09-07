#ifndef DistDirichlet_H
#define DistDirichlet_H

#include "Distribution.h"

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
