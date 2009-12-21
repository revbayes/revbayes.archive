/*
 * DistNormal.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef DistUniform_H
#define DistUniform_H

#include "Distribution.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "StringVector.h"

class DAGNode;

class DistUniform : public Distribution {
public:
        static const StringVector   rbClass;            //!< Static class attribute

        DistUniform(DAGNode* l, DAGNode* u, RandomNumberGenerator* r);
        DistUniform(const DistUniform& d);
        virtual ~DistUniform();

        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
        void                print(std::ostream& o) const;                         //!< Print complete object info
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&			operator=(const RbObject& o);
        DistUniform&        operator=(const DistUniform& o);
		RbObject* rv();

	double lnPdf(const RbObject* obs); //!< Ln probability density
	double pdf(const RbObject* obs); //!< Probability density

private:
	DAGNode* lower;
	DAGNode* upper;
};

#endif /* DISTNORMAL_H_ */
