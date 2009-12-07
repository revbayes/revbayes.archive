#include "RbNames.h"
#include "RbPrimitive.h"
#include "StringVector.h"




/**
 * @brief Constructor
 *
 * The constructor uses the global workspace (the base environment)
 * to initialize the member variables and retrieve a reference to the
 * function table.
 *
 */
RbPrimitive::RbPrimitive() : RbObject() {

}


/**
 * @brief Destructor
 *
 * The destructor deletes the objects that may be stored
 * in the object slots of the members map.
 *
 */
RbPrimitive::~RbPrimitive() {

}

/** Get class vector describing type of object */
const StringVector& RbPrimitive::getClass() const {

    static StringVector rbClass = StringVector(RbNames::Primitive::name) + RbObject::getClass();
    return rbClass;
}
