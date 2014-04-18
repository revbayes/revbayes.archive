#ifndef RlSwitchRateJumpMove_H
#define RlSwitchRateJumpMove_H

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
    class SwitchRateJumpMove : public Move {
        
    public:
        
        SwitchRateJumpMove(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual SwitchRateJumpMove*                 clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable>                       v;
        
    };
    
}

#endif
