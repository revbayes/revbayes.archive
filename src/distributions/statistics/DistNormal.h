/*
 * DistNormal.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef DISTNORMAL_H
#define DISTNORMAL_H

#include "Distribution.h"
#include "RbDouble.h"
#include "RbObject.h"

class DAGNode;

class DistNormal : public Distribution {
public:
        static const StringVector   rbClass;            //!< Static class attribute

	DistNormal(DAGNode* m, DAGNode* s);
	DistNormal(const DistNormal& d);
	virtual ~DistNormal();
        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
       void                print(std::ostream& o) const;                         //!< Print complete object info
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&			operator=(const RbObject& o);
        DistNormal&         operator=(const DistNormal& o);

	double lnPdf(RbObject* obs); //!< Ln probability density
	double pdf(RbObject* obs); //!< Probability density
	RbObject* rv(RandomNumberGenerator* r);

private:
	DAGNode* sigma;
	DAGNode* mu;
};

#endif /* DISTNORMAL_H_ */
