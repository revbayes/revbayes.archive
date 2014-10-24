#ifndef RlAtlas_H
#define RlAtlas_H

#include "ModelObject.h"
#include "RlAtlas.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RlAtlas : public ModelObject<RevBayesCore::TimeAtlas> {
        
    public:
        
        RlAtlas(void);                                                                                                              //!< Default constructor
        RlAtlas(RevBayesCore::TimeAtlas *m);                                                                                        //!< Default constructor
        RlAtlas(RevBayesCore::TypedDagNode<RevBayesCore::TimeAtlas> *d);                                                            //!< Default constructor
        
        // Basic utility functions
        RlAtlas*                            clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
    };
    
}
#endif /* defined(RlAtlas_H) */
