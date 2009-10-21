/*
 * RbTypeInfo.h
 *
 *  Created on: 29 sep 2009
 *      Author: Sebastian
 */

#ifndef RBTYPEINFO_H_
#define RBTYPEINFO_H_

#include "RbObject.h"
#include <typeinfo>

class RbTypeInfo {
    public:
        RbTypeInfo(std::type_info& ot);
        RbTypeInfo(std::type_info& ot, std::type_info& et, int* l);
        virtual ~RbTypeInfo();

        std::type_info&     getObjectType() const;
        std::type_info&     getElementType() const;
        int*                getLength() const;

    private:
        std::type_info&     objectType;                         // the type of this object, e.g. RbInt
        std::type_info&     elementType;                        // the type of the elements of this object if this is a container object (e.g. a RbVector)
        int*                length;                             // the length/number of elements in this object
};

#endif /* RBTYPEINFO_H_ */
