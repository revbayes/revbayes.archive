/*
 * MoveScale.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef MOVESCALE_H_
#define MOVESCALE_H_

#include <iostream>
#include <string>
#include <vector>

#include "RbMove.h"

class RbObject;

class MoveScale : public RbMove {
    public:

        static const StringVector   rbClass;            //!< Static class attribute

        MoveScale(DAGNode* n, RbDouble* tn, RandomNumberGenerator* r);
        MoveScale(const MoveScale& m);
        virtual ~MoveScale();

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object
        RbMove&                    operator=(const RbMove& o);
        RbObject&                  operator=(const RbObject& o);
        MoveScale&                 operator=(const MoveScale& o);



    protected:
        double         perform(RbObject* obj);
        void           accept(void);                // for statistic purposes
        void           reject(void);
        
        RbDouble*				tuningParm;
};

#endif /* MOVESCALE_H_ */
