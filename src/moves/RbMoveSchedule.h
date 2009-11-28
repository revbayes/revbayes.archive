/*
 * RbMoveSchedule.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef RBMOVESCHEDULE_H_
#define RBMOVESCHEDULE_H_

#include "RandomNumberGenerator.h"
#include "RbComplex.h"
#include "RbObject.h"
#include "StringVector.h"

class RbMove;

class RbMoveSchedule : public RbComplex {
    public:

        static const StringVector   rbClass;            //!< Static class attribute

        RbMoveSchedule(RandomNumberGenerator* r);
        virtual ~RbMoveSchedule();

        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object


        // overloaded operators
        RbObject&                  operator=(const RbObject& o);
        RbMoveSchedule&            operator=(const RbMoveSchedule& o);

        RbMove*                         getNext();
        void                            addMove(RbMove* m, double w);

    protected:

        RandomNumberGenerator*          rng;
        double                          sumWeights;
        std::map<double, RbMove*>       schedule;    //!< Member variables
};

#endif /* RBMOVESCHEDULE_H_ */
