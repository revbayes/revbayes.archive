#ifndef Move_SwitchRateJump_H
#define Move_SwitchRateJump_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a switch move between a jump and no jump of a rate-jump process.
     *
     * This class is the RevLanguage wrapper of SwitchRateJumpMove.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2014-04-08, version 1.0
     */
    class Move_SwitchRateJump : public Move {
        
    public:
        
        Move_SwitchRateJump(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_SwitchRateJump*                 clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      v;
        
    };
    
}

#endif
