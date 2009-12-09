#ifndef RbException_H
#define RbException_H

#include <string>

#include "RbObject.h"
#include "RbDumpState.h"
#include <iostream>

class RbException : public RbObject {

public:
	// constructors
	RbException(void);                                      // default contructor
    RbException(const std::string& msg) : message(msg) {}   //!< Default message constructor
	RbException(const RbException& e);                      // copy contructor
	virtual ~RbException(void);                             // destructor

	// implemented abstract/virtual functions from base classes
	RbObject*           clone(void) const;                                  //!< clone this object
    bool                equals(const RbObject* obj) const;                  //!< Equals comparison
    const StringVector& getClass() const;                                   //!< Get class
    void                print(std::ostream &c) const;                       //!< Print the value to ostream c
    void                printValue(std::ostream& o) const;              //!< Print value (for user)
    std::string         toString(void) const;                           //!< General info on object
    void                dump(std::ostream& c);                              //!< Dump to ostream c
	void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state

    // overloaded operators
    RbObject&           operator=(const RbObject& o);
    RbException&        operator=(const RbException& ar);

	// member functions
	void setMessage(std::string m);                       // setter for message of the exception
	std::string getMessage(void);                         // getter for message of the exception

private:
	std::string message;
};

#endif
