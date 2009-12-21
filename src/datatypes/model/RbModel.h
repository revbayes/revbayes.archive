/*
 * RbModel.h
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#ifndef RBMODEL_H_
#define RBMODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "RbComplex.h"

class DAGNode;
class RandomNumberGenerator;

class RbModel : public RbComplex {
    public:

        RbModel(std::vector<DAGNode*>& s, RandomNumberGenerator* r);
        RbModel(const RbModel& m);
        virtual ~RbModel();

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const;                               //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

        // overloaded operators
        RbObject&                   operator=(const RbObject& o);
        RbModel&                    operator=(const RbModel& o);

        DAGNode*                    getDagToUpdate(void);
        void                        monitor(int i);
        
    protected:
		void						extractNodes(std::vector<DAGNode*>& dn); 
		void						extractNodes(DAGNode* dn);
		void  						initializeUpdateInfo(void);
		void 						initializeDAGs(void);
		std::vector<DAGNode*>      sinkDags;
        std::set<DAGNode*>       dagNodes;
        RandomNumberGenerator* 		rng; 
        std::vector<DAGNode*>       changeableDags;
        std::vector<double>         changeableDagUpdateProbs;
};

#endif /* RBMODEL_H_ */
