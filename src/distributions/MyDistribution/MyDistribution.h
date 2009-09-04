#ifndef MyDistribution_H
#define MyDistribution_H

#include "../../main/Distribution.h"
#include "../../main/Plugin.h"
#include "../../main/RbDataType.h"


class MyDistribution : Distribution {

public:
	MyDistribution(void);

	PLUGIN_API virtual Distribution* createInstance(void);

	virtual RbDataType* execute(); //!< Call appropriate function based on functionType
	virtual RbDataType getDataType(); //!< Get data type for type checking
	virtual RbDataType* lnPdf(); //!< Ln probability density
	virtual RbDataType* pdf(); //!< Probability density
	virtual RbDataType* rv(); //!< Draw random variables

private:
};

#endif
