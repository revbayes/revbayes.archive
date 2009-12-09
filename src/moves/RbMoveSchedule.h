/*
 * RbMoveSchedule.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef RbMoveSchedule_H
#define RbMoveSchedule_H

#include "RandomNumberGenerator.h"
#include "RbComplex.h"
#include "RbObject.h"
#include "StringVector.h"

class RbMove;

#include <map>

class RbMoveSchedule : public RbComplex {
    public:

        RbMoveSchedule(RandomNumberGenerator* r, double w);
        virtual ~RbMoveSchedule();


        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
 		const StringVector&        getClass(void) const;                           //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object
        void                       setUpdateWeight(double x) { dagUpdateWeight = x; }
        double					   getUpdateWeight(void) { return dagUpdateWeight; }


        // overloaded operators
        RbObject&                  operator=(const RbObject& o);
        RbMoveSchedule&            operator=(const RbMoveSchedule& o);

        RbMove*                         getNext();
        void                            addMove(RbMove* m, double w);

    protected:

        RandomNumberGenerator*          rng;
        double                          sumWeights;
        std::map<double, RbMove*>       schedule;    //!< Member variables
        double 							dagUpdateWeight;
};

#endif
