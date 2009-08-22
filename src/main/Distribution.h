#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include "RbObject.h"

class Argument;
class ArgumentRule;
class Distribution : public RbObject {

public:
	virtual RbDataType rv(void)=0;
	virtual double pdf(void)=0;
	virtual double lnPdf(void)=0;
private:
	RbDataType *
	std::vector<Argument*> argument;
	std::vector<ArgumentRule*> argumentRule;
};

#endif;