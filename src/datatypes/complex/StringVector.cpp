#include "StringVector.h"
#include "RbObject.h"
#include "RbComplex.h"
#include "RbDouble.h"
#include "RbException.h"
#include <iostream>

const StringVector StringVector::rbClass = StringVector("StringVector");

StringVector::StringVector(int n) {

    value.resize(n);
}

StringVector::StringVector(std::string s) {

    value.push_back(s);
}

StringVector::StringVector(int n, std::string v) {

    for (int i = 0; i < n; i++)
        value.push_back(v);
}

StringVector::StringVector(const StringVector& v) {

    value = v.value;
}

StringVector::StringVector(std::vector<std::string> &v) {

    value = v;
}

RbObject& StringVector::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const StringVector& x = dynamic_cast<const StringVector&> (obj);

        StringVector& sv = (*this);
        sv = x;
        return sv;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const StringVector& x = dynamic_cast<const StringVector&> (*(obj.convertTo("StringVector")));

            StringVector& sv = (*this);
            sv = x;
            return sv;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to StringVector");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

StringVector& StringVector::operator=(const StringVector& sv) {

    value = sv.value;
    return (*this);
}

StringVector& StringVector::operator+(const StringVector& sv) const {

    std::vector<std::string> tempVec;
    for (int i = 0; i < value.size(); i++)
        tempVec.push_back(value[i]);
    for (int i = 0; i < sv.value.size(); i++)
        tempVec.push_back(sv.value[i]);
    StringVector* newSv = new StringVector(tempVec);
    return *newSv;
}

/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool StringVector::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const StringVector* x = dynamic_cast<const StringVector*> (obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to a double
    x = dynamic_cast<const StringVector*> (obj->convertTo("StringVector"));
    if (x == NULL)
        return false;

    bool result = true;
    if (value.size() == x->value.size()) {
        for (int i = 0; i < value.size(); i++)
            result = result && (value[i] == x->value[i]);
    } else
        result = false;
    delete x;
    return result;
}

void StringVector::print(std::ostream& o) const {

    for (int i = 0; i < value.size(); i++)
        o << value[i] << " ";
    o << std::endl;
}

RbObject* StringVector::clone(void) const {

    StringVector* x = new StringVector(*this);
    return (RbObject*) x;
}

void StringVector::printValue(std::ostream& o) const {

    o << "Complex object of type " << getClass()[0] << std::endl;

    for (std::map<std::string, RbObject*>::const_iterator i = members.begin(); i
            != members.end(); i++) {
        o << "." << i->first << " = ";
        o << getMember(i->first)->briefInfo();
        o << std::endl;
    }
    o << toString() << std::endl;
}

std::string StringVector::toString(void) const {

    std::string tempStr = "";
    for (std::vector<std::string>::const_iterator i = value.begin(); i
            != value.end(); i++) {
        tempStr += (*i);
        if (i + 1 != value.end())
            tempStr += ":";
    }
    return tempStr;
}

