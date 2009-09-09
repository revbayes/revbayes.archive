#ifndef RbObject_H
#define RbObject_H

#include <string>
#include <ostream>
#include "../utils/RbDumpState.h"

class RbObject{

public:
     virtual ~RbObject() {}  //! Virtual destructor because of virtual functions

	std::string getName(void) { return name; }
	void setName(std::string n) { name = n; }
//	virtual bool equals(RbObject o)=0;
	virtual std::string toString(void){ return name; }
	virtual void print(std::ostream &c) { c<<getName(); } //!< Print this object
//	virtual void dump(void)=0;
//	virtual void restore(RbDumpState state)=0;

private:
	std::string name;

};
#endif
