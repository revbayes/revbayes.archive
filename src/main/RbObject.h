#ifndef RbObject_H
#define RbObject_H

#include <string>
#include "RbDumpState.h"

class RbObject{

public:
	std::string getName(void) { return name; }
	void setName(std::string n) { name = n; }
//	virtual void dump(void)=0;
//	virtual void restore(RbDumpState state)=0;

private:
	std::string name;

};
#endif
