/*
 * RbMoveSchedule.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef RBMOVESCHEDULE_H_
#define RBMOVESCHEDULE_H_

class RbMoveSchedule : RbComplex {
    public:
        RbMoveSchedule();
        virtual ~RbMoveSchedule();

        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object


        RbMove*                         getNext();

    protected:

        std::map<double, RbMove*>       schedule;    //!< Member variables
};

#endif /* RBMOVESCHEDULE_H_ */
