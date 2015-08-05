#ifndef RlDistanceMatrix_H
#define RlDistanceMatrix_H

#include "ModelObject.h"
#include "DistanceMatrix.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RlDistanceMatrix : public ModelObject<RevBayesCore::DistanceMatrix> {
        
    public:
        
        RlDistanceMatrix(void);                                                                                                              //!< Default constructor
        RlDistanceMatrix(RevBayesCore::DistanceMatrix *m);                                                                                        //!< Default constructor
        RlDistanceMatrix(RevBayesCore::TypedDagNode<RevBayesCore::DistanceMatrix> *d);                                                            //!< Default constructor
        
        // Basic utility functions
        RlDistanceMatrix*                            clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    private:
        RevBayesCore::DistanceMatrix* distanceMatrix;
    };
    
}
#endif /* defined(RlDistanceMatrix_H) */
