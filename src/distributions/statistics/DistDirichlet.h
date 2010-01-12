#ifndef DistDirichlet_H
#define DistDirichlet_H

#include "Distribution.h"

class DAGNode;
class Vector;
class DistDirichlet: public Distribution {

    public:
        static const StringVector rbClass; //!< Static class attribute

        DistDirichlet(DAGNode* a, RandomNumberGenerator* r);
        DistDirichlet(const DistDirichlet& d);
        virtual ~DistDirichlet();


        RbObject*           clone(void) const;                                      //!< Clone object
        bool                equals(const RbObject* o) const;                        //!< Equals comparison
        const StringVector& getClass(void) const;                                   //!< Get class
        void                print(std::ostream& o) const;                           //!< Print complete object info
        void                printValue(std::ostream& o) const;                      //!< Print value (for user)
        std::string         toString(void) const;                                   //!< General info on object

        // overloaded operators
        DistDirichlet&      operator=(const DistDirichlet& o);

        // overwritten from MemberObject
        const RbObject*         executeOperation(const std::string& name, std::vector<DAGNode*>& args);      //!< Execute method

        // overwritten from Distribution
        const ArgumentRule**    getMemberRules(void);                   //!< Get member rules
        const std::string       getReturnType() const;                  //!< Get variable type


        // overloaded functions
        double              lnPdfRatio(const RbObject* newVal, const RbObject* oldVal);  //!< Calculate ln prior ratio
        double              lnPdf(const RbObject* obs);                             //!< Ln probability density
        double              pdf(const RbObject* obs);                               //!< Probability density
        RbObject*           rv();


    private:
        Vector *value;
        DAGNode *alpha;
};

#endif
