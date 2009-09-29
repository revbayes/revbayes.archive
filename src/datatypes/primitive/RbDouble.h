/*
 * RbDouble.h
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#ifndef RBDOUBLE_H_
#define RBDOUBLE_H_

#include "../RbAbstractDataType.h"
#include "../../main/RbObject.h"
#include "../../utils/RbDumpState.h"

class RbDouble: public RbAbstractDataType {
public:

    RbDouble(const double v);                                      //!< Constructor from double
    RbDouble(const double v, std::string& name);                   //!< Constructor from double
    RbDouble(const RbDouble& s);                                   //!< Copy constructor
	virtual ~RbDouble();

    // implemented abstract/virtual functions from base classes
	RbObject*  clone(void);                                        //!< clone this object
	void       print(std::ostream &c) const;                            //!< Print the value to ostream c
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state

	//overloaded operators
	bool       operator==(RbObject& o) const;                      //!< Comparison

	// memeber functions
	void       setValue(double d);
	double     getValue(void) const;

private:
	double     value;      //!< value member
};

#endif /* RBDOUBLE_H_ */
