#ifndef Vector_H
#define Vector_H

#include <vector>
#include "RbNumeric.h"

class RbVector : public RbNumeric {

public:
        RbVector(int n);
        RbVector(int n, double v);
        RbVector(const RbVector& v);
        RbVector(std::vector<double> &v);

	double operator[](int i) { return value[i]; }


    // Pointer-based arithmetic -- throw not supported error by default
    RbObject*           add(const RbObject* o) const;                             //!< Addition
    RbObject*           subtract(const RbObject* o) const;                        //!< Subtraction
    RbObject*           multiply(const RbObject* o) const;                        //!< Multiplication
    RbObject*           divide(const RbObject* o) const;                          //!< Division
    RbObject*           raiseTo(const RbObject* o) const;

private:
	std::vector<double> value;
};

#endif
