#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include <list>
#include "../main/RbObject.h"
#include "../datatypes/RbDataType.h"
//#include "Argument.h"
//#include "ArgumentRule.h"
#include "../modelLanguage/parser/SyntaxLabeledExpr.h"
//#include "Plugin.h"

class Distribution: public RbObject {

public:
	enum functionT {D, P, Q, R, TILDE};

	Distribution() :
		RbObject(), functionType(Distribution::D) {
	} //!< Constructor sets type to "d"
	virtual ~Distribution() {
	} //!< Destructor does nothing

//	PLUGIN_API virtual Distribution* createInstance(void)=0;

	virtual RbDataType* execute(); //!< Call appropriate function based on functionType
	virtual RbDataType getDataType() = 0; //!< Get data type for type checking
	virtual double lnPdf(RbDataType* value, std::vector<RbDataType*> arguments) = 0; //!< Ln probability density
	virtual double pdf(RbDataType* value, std::vector<RbDataType*> arguments) = 0; //!< Probability density
	virtual RbDataType* rv() = 0; //!< Draw random variables
	virtual double cdf(RbDataType* variable, std::vector<RbDataType*> arguments) = 0;
	virtual double quantile(std::vector<RbDataType*> arguments) = 0;


	//bool setArguments(std::list<SyntaxLabeledExpr*> args); //!< Set arguments based on function type
	void setFunctionType(functionT type) {
		functionType = type;
	} //!< Set function type
	std::string getDistributionName(void) { return getName(); } //!< Returns the name of the distribution

protected:
	functionT functionType; //!< Holds function type
	//bool lnValue; //!< Calculate ln prob instead of prob
private:
//	std::vector<Argument*> argument;
//	std::vector<ArgumentRule*> argumentRule;
};

#endif
