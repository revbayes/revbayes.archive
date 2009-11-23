#include "StringVector.h"
#include "RbObject.h"
#include "RbDouble.h"
#include "RbComplex.h"
#include <iostream>

const StringVector StringVector::rbClass = StringVector("StringVector");

StringVector::StringVector(int n) {

    value.resize(n);
}

StringVector::StringVector(std::string s) {

    value.push_back( s );
}

StringVector::StringVector(int n, std::string v) {

    for (int i=0; i<n; i++)
    value.push_back( v );
}

StringVector::StringVector(const StringVector& v) {

    value = v.value;
}

StringVector::StringVector(std::vector<std::string> &v) {

    value = v;
}

void StringVector::print(std::ostream& o) const {

    for (int i=0; i<value.size(); i++)
    o << value[i] << " ";
    o << std::endl;
}

RbObject* StringVector::clone(void) const {

    StringVector* x = new StringVector(*this);
    return (RbObject*)x;
}
