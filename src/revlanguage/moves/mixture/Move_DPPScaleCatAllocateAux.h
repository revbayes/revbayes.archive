#ifndef Move_DPPScaleCatAllocateAux_H
#define Move_DPPScaleCatAllocateAux_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on all elements of a real valued vector.
     *
     * This class is the RevLanguage wrapper of Move_DPPScaleCatAllocateAux.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    class Move_DPPScaleCatAllocateAux : public Move {
        
    public:
        
        Move_DPPScaleCatAllocateAux(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_DPPScaleCatAllocateAux*                        clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        RevPtr<const Variable>                      x;                                                                                              //!< The variable holding the real valued vector.
        RevPtr<const Variable>                      lambda;                                                                                         //!< The variable for the tuning parameter.
        RevPtr<const Variable>                      nAux;                                                                                         //!< The variable for the tuning parameter.
        
    };
    
}

#endif
