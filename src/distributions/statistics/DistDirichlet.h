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

        // overloaded functions
        double              lnPdf(const RbObject* obs);                             //!< Ln probability density
        double              pdf(const RbObject* obs);                               //!< Probability density
        RbObject*           rv();


    private:
        Vector *value;
        DAGNode *alpha;
};

#endif
