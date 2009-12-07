#ifndef Move_H
#define Move_H

#include <string>
#include <vector>

#include "RbComplex.h"

class DAGNode;
class RandomNumberGenerator;
class StringVector;

class RbMove : public RbComplex {

public:

        virtual ~RbMove(void);

       void                        acceptMove(void);				// for statistic purposes
        double                      getAcceptanceProbability(void);
        double                      performMove(void);
        void                        rejectMove(void);

        // Basic utility functions
        const StringVector& getClass(void) const;               //!< Get class vector

        // overloaded operators
        virtual RbMove&             operator=(const RbMove& o) = 0;

protected:
        RbMove(DAGNode* n, RandomNumberGenerator* r);

        // virtual functions
        virtual double              perform(void) = 0;
        virtual void                accept(void) = 0;                // for statistic purposes
        virtual void                reject(void) = 0;

		RandomNumberGenerator*		rng;
        int nAcceptances;
        int nTries;
        DAGNode* node;
};

#endif
