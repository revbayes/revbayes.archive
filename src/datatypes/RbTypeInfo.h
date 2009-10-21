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

class RbTypeInfo : public RbObject{
    public:
        RbTypeInfo(const std::type_info& ot);
        RbTypeInfo(const std::type_info& ot, const std::type_info& et, int* l, int d);
        RbTypeInfo(const RbTypeInfo& ti);
        virtual ~RbTypeInfo();

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                operator==(const RbObject& o) const;                //!< Comparison
        bool                operator==(const RbTypeInfo& o) const;              //!< Comparison

        // getter functions
        const std::type_info&     getObjectType() const;
        const std::type_info&     getElementType() const;
        int*                getLength() const;
        int                 getDimensions() const;

    private:
        const std::type_info&   objectType;                         // the type of this object, e.g. RbInt
        const std::type_info&   elementType;                        // the type of the elements of this object if this is a container object (e.g. a RbVector)
        int*                    length;                             // the length/number of elements in this object
        int                     dimensions;                         // the number of dimension
};

#endif /* RBTYPEINFO_H_ */
