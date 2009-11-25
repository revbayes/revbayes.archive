/*
 * RbModel.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef RBMODEL_H_
#define RBMODEL_H_

#include <iostream>
#include <string>
#include <vector>

#include "RbComplex.h"

class DAGNode;

class RbModel : public RbComplex{
    public:

        static const StringVector   rbClass;            //!< Static class attribute

        RbModel(DAGNode* s);
        RbModel(const RbModel& m);
        virtual ~RbModel();

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object
        bool                       isType(std::string t) const { return rbClass[0] == t; }
        StringVector&              operator+(const StringVector& sv) const;

    protected:
        DAGNode*                   sink;
};

#endif /* RBMODEL_H_ */
