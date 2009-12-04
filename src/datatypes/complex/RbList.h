/*
 * RbList.h
 *
 *  Created on: 2 dec 2009
 *      Author: Sebastian
 */

#ifndef RBLIST_H
#define RBLIST_H

#include "RbComplex.h"

class RbList : public RbComplex{
    public:
        virtual ~RbList();

        virtual void                    addElement(RbObject* o) = 0;
        virtual void                    addElementAt(RbObject* o, int i) = 0;
        virtual RbObject*               getElement(int i) const = 0;
        virtual int                     getSize() const = 0;
        virtual void                    setElement(RbObject* o) = 0;


    protected:

        RbList();
};

#endif /* RBLIST_H_ */
