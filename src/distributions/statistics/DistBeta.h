/*
 * DistBeta.h
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#ifndef DISTBETA_H
#define DISTBETA_H

#include "Distribution.h"
#include "RbDouble.h"

class DAGNode;

class DistBeta : public Distribution {
public:
        static const StringVector   rbClass;            //!< Static class attribute

    DistBeta(DAGNode* a, DAGNode* b);
    DistBeta(const DistBeta& d);
    virtual ~DistBeta();
        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&           operator=(const RbObject& o);
        DistBeta&    operator=(const DistBeta& o);

    double lnPdf(RbObject* o); //!< Ln probability density
    double pdf(RbObject* o); //!< Probability density
    RbObject* rv(RandomNumberGenerator* r);

private:
    DAGNode* alpha;
    DAGNode* beta;
};

#endif /* DISTBETA_H_ */
