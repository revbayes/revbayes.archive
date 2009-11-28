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
class RandomNumberGenerator;

class RbModel : public RbComplex {
    public:

        static const StringVector   rbClass;            //!< Static class attribute

        RbModel(std::vector<DAGNode*>& s);
        RbModel(const RbModel& m);
        virtual ~RbModel();

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

    protected:
        std::vector<DAGNode*>      sinkDags;
        RandomNumberGenerator* 		rng; 
};

#endif /* RBMODEL_H_ */
