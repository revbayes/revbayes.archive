#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include "RbObject.h"
#include "plugin.h"

class Argument;
class ArgumentRule;
class RbDataType;
class Distribution : public RbObject {

public:
	PLUGIN_API virtual Distribution* createInstance(void)=0;
	PLUGIN_API virtual RbDataType* rv(void)=0;
	PLUGIN_API virtual double pdf(void)=0;
	PLUGIN_API virtual double lnPdf(void)=0;
private:
	std::vector<Argument*> argument;
	std::vector<ArgumentRule*> argumentRule;
};

#endif;
