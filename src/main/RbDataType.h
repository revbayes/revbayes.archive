#ifndef RbDataType_H
#define RbDataType_H

#include <string>
#include "RbObject.h"

class RbDataType {

public:
	virtual RbDataType* copy(void)=0;
	virtual void print(void)=0;
	virtual bool store(void)=0;
	virtual bool restore(void)=0;

protected:
	std::string typeName;
};

#endif