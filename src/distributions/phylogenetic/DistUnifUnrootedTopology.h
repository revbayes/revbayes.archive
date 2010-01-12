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

class DistUnifUnrootedTopology: public Distribution {
    public:

        DistUnifUnrootedTopology(DAGNode* n, RandomNumberGenerator* r);
        DistUnifUnrootedTopology(const DistUnifUnrootedTopology& d);
        virtual ~DistUnifUnrootedTopology();
        RbObject* clone(void) const; //!< Clone object
        bool equals(const RbObject* o) const; //!< Equals comparison
        const StringVector& getClass(void) const; //!< Get class
        void printValue(std::ostream& o) const; //!< Print value (for user)
        std::string toString(void) const; //!< General info on object

        // overloaded operators
        RbObject& operator=(const RbObject& o);
        DistUnifUnrootedTopology& operator=(const DistUnifUnrootedTopology& o);

        // overwritten from MemberObject
        const RbObject*         executeOperation(const std::string& name, std::vector<DAGNode*>& args);      //!< Execute method


        // overwritten from Distribution
        const ArgumentRule**    getMemberRules(void);                   //!< Get member rules
        const std::string       getReturnType() const;                  //!< Get variable type

        double                  lnPdfRatio(const RbObject* newVal, const RbObject* oldVal);  //!< Calculate ln prior ratio
        double                  lnPdf(const RbObject* value);           //!< Ln probability density function
//        double                  lnLikelihoodRatio(const RbObject* value);    //!< Calculate ln likelihood ratio
        double                  pdf(const RbObject* value);             //!< Probability density function
        RbObject*               rv();                                  //!< Generate a random draw

    private:
        DAGNode* numTaxa;
};

#endif /* DistUnifUnrootedTopology_H */
