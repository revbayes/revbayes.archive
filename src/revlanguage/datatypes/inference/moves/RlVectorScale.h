#ifndef RlVectorScale_H
#define RlVectorScale_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on all elements of a real valued vector.
     *
     * This class is the RevLanguage wrapper of VectorScale.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    class VectorScale : public Move {
        
    public:
        
        VectorScale(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual VectorScale*                        clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable>                       x;                                                                                              //!< The variable holding the real valued vector.
        RbPtr<const Variable>                       lambda;                                                                                         //!< The variable for the tuning parameter.
        RbPtr<const Variable>                       tune;                                                                                           //!< The variable telling if to tune or not.
        
    };
    
}

#endif
