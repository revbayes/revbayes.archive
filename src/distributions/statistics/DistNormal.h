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

class DAGNode;

class DistNormal : Distribution {
public:
	DistNormal(DAGNode* s, DAGNode* m, DAGNode* x);
	DistNormal(DistNormal& d);
	virtual ~DistNormal();
        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
        bool                        isType(const std::string t) const;                        //!< Is the object of type t?
       void                print(std::ostream& o) const;                         //!< Print complete object info
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&			operator=(const RbObject& o);

	double lnPdf(RbObject* o); //!< Ln probability density
	double pdf(RbObject* o); //!< Probability density

private:
	DAGNode* sigma;
	DAGNode* mu;
	DAGNode* obs;
};

#endif /* DISTNORMAL_H_ */
