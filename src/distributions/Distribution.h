/**
 * @file
 * This file contains the interface for distributions.
 * A distribution is taken in the statistical sense.
 * This interface specifies the pdf, cdf, quantile and rv function with its parameters which need to be provided by any distribution.
 *
 * @brief Declaration of Interface Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Distribution
 * @extends RbObject
 * @package distributions
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include <list>
#include "../main/RbObject.h"
#include "../datatypes/RbDataType.h"
//#include "Argument.h"
//#include "ArgumentRule.h"
#include "../modelLanguage/parser/SyntaxLabeledExpr.h"

class Distribution: public RbObject {

public:
	enum functionT {D, P, Q, R, TILDE};

	virtual ~Distribution() { }                        //!< Destructor does nothing

//	PLUGIN_API virtual Distribution* createInstance(void)=0;

	virtual RbDataType* execute(RbDataType* value);    //!< Call appropriate function based on functionType
	virtual RbDataType* getDataType() = 0;              //!< Get data type for type checking
	virtual double lnPdf(RbDataType* value) = 0;       //!< Ln probability density function
	virtual double pdf(RbDataType* value) = 0;         //!< Probability density function
	virtual RbDataType* rv() = 0;                      //!< Draw random variables
	virtual double cdf(RbDataType* variable) = 0;      //!< The cummulative density function until variable
	virtual double quantile(RbDataType* variable) = 0; //!< The quantile function


	//bool setArguments(std::list<SyntaxLabeledExpr*> args); //!< Set arguments based on function type
	void setFunctionType(functionT type) {
		functionType = type;
	} //!< Set function type
	std::string getDistributionName(void) { return getName(); } //!< Returns the name of the distribution

protected:
	Distribution() :
		RbObject(), functionType(Distribution::D) {
	} //!< Constructor sets type to "d"
	functionT functionType; //!< Holds function type
	//bool lnValue; //!< Calculate ln prob instead of prob
private:
//	std::vector<Argument*> argument;
//	std::vector<ArgumentRule*> argumentRule;
};

#endif
