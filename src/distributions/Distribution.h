#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include <list>
#include "RbObject.h"
#include "RbDataType.h"
//#include "Argument.h"
//#include "ArgumentRule.h"
#include "../parser/SyntaxLabeledExpr.h"
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
	virtual RbDataType* lnPdf() = 0; //!< Ln probability density
	virtual RbDataType* pdf() = 0; //!< Probability density
	virtual RbDataType* rv() = 0; //!< Draw random variables


	bool setArguments(std::list<SyntaxLabeledExpr*> args); //!< Set arguments based on function type
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
