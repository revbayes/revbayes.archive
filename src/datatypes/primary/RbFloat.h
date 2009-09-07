/*
 * RbFloat.h
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#ifndef RBFLOAT_H_
#define RBFLOAT_H_

#include "RbDataType.h"

class RbFloat: public RbDataType {
public:
	virtual ~RbFloat();
	RbFloat*  copy(void) const;     //!< Copy
	void    print(void) const;    //!< Print the value to cout
	void    print(ostream &c) const;    //!< Print the value to ostream c
	float   getValue(void) {return value;}

	bool    store(void) const { return true; }  //!< What does this do?
	bool    restore(void) { return true; }      //!< What does this do?

private:
	float     value;      //!< value member
};

#endif /* RBFLOAT_H_ */
