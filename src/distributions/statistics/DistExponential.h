/*
 * DistNormal.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef DistExponential_H
#define DistExponential_H

#include "Distribution.h"
#include "RbDouble.h"

class DAGNode;

class DistExponential : public Distribution {
public:
        static const StringVector   rbClass;            //!< Static class attribute

	DistExponential(DAGNode* l);
	DistExponential(const DistExponential& d);
	virtual ~DistExponential();
        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
       void                print(std::ostream& o) const;                         //!< Print complete object info
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&			operator=(const RbObject& o);
        DistExponential&    operator=(const DistExponential& o);

	double lnPdf(RbObject* o); //!< Ln probability density
	double pdf(RbObject* o); //!< Probability density

private:
	DAGNode* lambda;
};

#endif /* DISTNORMAL_H_ */
