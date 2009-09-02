#ifndef Distribution_H
#define Distribution_H

#include <vector>
#include "RbObject.h"
#include "Plugin.h"

class Argument;
class ArgumentRule;
class RbDataType;
class Distribution: public RbObject {

public:
	enum functionT {D, P, Q, R, TILDE};

	Distribution() :
		RbObject(), functionType(Distribution::D) {
	} //!< Constructor sets type to "d"
	virtual ~Distribution() {
	} //!< Destructor does nothing

	PLUGIN_API virtual Distribution* createInstance(void)=0;

	virtual RbDataType* execute(); //!< Call appropriate function based on functionType
	virtual RbDataType getDataType() = 0; //!< Get data type for type checking
	virtual RbVector* lnPdf() = 0; //!< Ln probability density
	virtual RbVector* pdf() = 0; //!< Probability density
	virtual RbDataType* rv() = 0; //!< Draw random variables


	bool setArguments(list<SyntaxLabeledExpr*> args); //!< Set arguments based on function type
	void setType(functionT type) {
		functionType = type;
	} //!< Set function type

protected:
	functionT functionType; //!< Holds function type
	//bool lnValue; //!< Calculate ln prob instead of prob
private:
	std::vector<Argument*> argument;
	std::vector<ArgumentRule*> argumentRule;
};

#endif;
