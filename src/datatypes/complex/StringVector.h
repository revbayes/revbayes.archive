#ifndef StringVector_H
#define StringVector_H

#include <iostream>
#include <string>
#include <vector>

#include "RbComplex.h"

class RbObject;

class StringVector : public RbComplex {

public:
	StringVector(int n);
    StringVector(std::string s);
	StringVector(int n, std::string v);
	StringVector(const StringVector& v);
	StringVector(std::vector<std::string> &v);
	virtual ~StringVector(void) { }

    static const StringVector   rbClass;            //!< Static class attribute

    // Basic utility functions
    RbObject*                  clone() const;                                  //!< Clone object
    bool                       equals(const RbObject* obj) const;              //!< Equals comparison
    const StringVector&        getClass() const { return rbClass; }            //!< Get class
    void                       print(std::ostream& o) const;                   //!< Print complete object info
    void                       printValue(std::ostream& o) const;              //!< Print value (for user)
    std::string                toString(void) const;                           //!< General info on object
    bool                       isType(std::string t) const { return rbClass[0] == t; }
    StringVector&              operator+(const StringVector& sv) const; 

	std::string operator[](int i) const { return value[i]; }
	int size(void) { return value.size(); }
	std::string                get(int i) const { return value[i]; }

private:
	std::vector<std::string> value;
};

#endif
