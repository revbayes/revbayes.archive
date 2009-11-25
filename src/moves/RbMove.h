#ifndef Move_H
#define Move_H

#include <string>
#include <vector>

#include "RbComplex.h"
#include "StringVector.h"

class DAGNode;

class RbMove : public RbComplex {

public:

        static const StringVector   rbClass;            //!< Static class attribute

        virtual ~RbMove(void);

        void                        acceptMove(void);				// for statistic purposes
        double                      getAcceptanceProbability(void);
        std::vector<ArgumentRule>   getArgumentRules() const { return argRules; }   //!< Get argument rules
        double                      performMove(void);
        void                        rejectMove(void);

protected:
        RbMove(DAGNode* n);

        // virtual functions
        virtual double              perform(void) = 0;
        virtual void                accept(void) = 0;                // for statistic purposes
        virtual void                reject(void) = 0;


        int nAcceptances;
        int nTries;
        DAGNode* node;
};

#endif
