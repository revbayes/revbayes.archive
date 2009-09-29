/*
 * RbTypeInfo.cpp
 *
 *  Created on: 29 sep 2009
 *      Author: Sebastian
 */

#include "RbTypeInfo.h"
#include "RbDataType.h"
#include <typeinfo>


RbTypeInfo::RbTypeInfo(std::type_info& info) : id(info) {

}

RbTypeInfo::~RbTypeInfo() {
    // TODO Auto-generated destructor stub
}


std::type_info& RbTypeInfo::getTypeid() const{
    return id;
}
