/*
 * DistUnifUnrootedTopology.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef DistUnifUnrootedTopology_H
#define DistUnifUnrootedTopology_H

#include "Distribution.h"
#include "RbDouble.h"
#include "RbObject.h"

class DAGNode;

class DistUnifUnrootedTopology : public Distribution {
public:
        static const StringVector   rbClass;            //!< Static class attribute

	DistUnifUnrootedTopology(DAGNode* n, RandomNumberGenerator* r);
	DistUnifUnrootedTopology(const DistUnifUnrootedTopology& d);
	virtual ~DistUnifUnrootedTopology();
        RbObject*           clone(void) const;                                    //!< Clone object
        bool                equals(const RbObject* o) const;                      //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
       void                printValue(std::ostream& o) const;                    //!< Print value (for user)
       std::string         toString(void) const;                                 //!< General info on object

        // overloaded operators
        RbObject&			operator=(const RbObject& o);
        DistUnifUnrootedTopology&         operator=(const DistUnifUnrootedTopology& o);

	double lnPdf(const RbObject* obs); //!< Ln probability density
	double pdf(const RbObject* obs); //!< Probability density
	RbObject* rv();

private:
	DAGNode* numTaxa;
};

#endif /* DistUnifUnrootedTopology_H */
