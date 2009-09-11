#ifndef RbObject_H
#define RbObject_H

#include <string>
#include <ostream>
#include "../utils/RbDumpState.h"

class RbObject{
public:
	RbObject(void);  //!< default constructor
    virtual ~RbObject();  //! Virtual destructor because of virtual functions

    virtual RbObject* clone(void) = 0;    //!< create a cope of this object. Used to call the copy constructor of the derived class

    std::string getName(void);
    void setName(std::string n);
//	virtual bool equals(RbObject o)=0;
    virtual std::string toString(void);
    virtual void print(std::ostream &c); //!< Print this object
//	virtual void dump(void)=0;
//	virtual void restore(RbDumpState state)=0;

protected:

private:
	std::string name;

};
#endif
