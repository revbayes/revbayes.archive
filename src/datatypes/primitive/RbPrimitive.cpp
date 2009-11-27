#include "RbPrimitive.h"
#include "StringVector.h"



const StringVector RbPrimitive::rbClass = StringVector("Primitive") + RbObject::rbClass;


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
