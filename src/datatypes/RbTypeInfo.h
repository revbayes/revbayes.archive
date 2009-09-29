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
        RbTypeInfo(std::type_info& info);
        virtual ~RbTypeInfo();
        std::type_info& getTypeid() const;

    private:
        std::type_info& id;
};

#endif /* RBTYPEINFO_H_ */
