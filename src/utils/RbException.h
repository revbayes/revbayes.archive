#ifndef RbException_H
#define RbException_H

#include <string>

#include "../main/RbObject.h"
#include "RbDumpState.h"
#include <iostream>

class RbException : public RbObject {

public:
	// constructors
	RbException(void);                                    // default contructor
	RbException(const RbException& e);                    // copy contructor
	virtual ~RbException(void);                           // destructor

	// implemented abstract/virtual functions from base classes
	RbObject*           clone(void) const;                                  //!< clone this object
	void                print(std::ostream &c) const;                       //!< Print the value to ostream c
	void                dump(std::ostream& c);                              //!< Dump to ostream c
	void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state

	//overloaded operators
	bool                operator==(const RbObject& o) const;                //!< Comparison


	// member functions
	void setMessage(std::string m);                       // setter for message of the exception
	std::string getMessage(void);                         // getter for message of the exception

private:
	std::string message;
};

#endif
