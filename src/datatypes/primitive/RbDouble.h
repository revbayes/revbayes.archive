/*
 * RbDouble.h
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#ifndef RBDOUBLE_H_
#define RBDOUBLE_H_

#include "../RbDataType.h"
#include "../../main/RbObject.h"

class RbDouble: public RbDataType {
public:
    RbDouble(const double v);       //!< Constructor from double
    RbDouble(const RbDouble& s);    //!< Copy constructor
	//virtual ~RbDouble();
	RbObject*  clone(void);     //!< clone this object
	void    print(void) const;    //!< Print the value to cout
	void    print(ostream &c) const;    //!< Print the value to ostream c
	double   getValue(void) {return value;}

	bool    store(void) const { return true; }  //!< What does this do?
	bool    restore(void) { return true; }      //!< What does this do?

private:
	double     value;      //!< value member
};

#endif /* RBDOUBLE_H_ */
