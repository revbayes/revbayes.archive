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

    std::string         getName(void);              //!< Get name of object
    void                setName(std::string n);     //!< Set name of object
	virtual bool        operator==(RbObject o)=0;   //!< Comparison
    virtual std::string toString(void);             //!< Information about object for debugging, printing etc
    virtual void        print(std::ostream& c);     //!< Print this object for debugging, printing etc
	virtual void        dump(std::ostream& c)=0;    //!< Dump to ostream c
	virtual void        resurrect(const RbDumpState& x)=0;  //!< Resurrect from dumped state

protected:

private:
	std::string name;

};
#endif
