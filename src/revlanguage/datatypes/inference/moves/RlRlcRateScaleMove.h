#ifndef RlRlcRateScaleMove_H
#define RlRlcRateScaleMove_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a rate scale move of a random local clock object.
     *
     * This class is the RevLanguage wrapper of RlcRateScaleMove.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2014-04-08, version 1.0
     */
    class RlcRateScaleMove : public Move {
        
    public:
        
        RlcRateScaleMove(void);                                                                                             //!< Default constructor
        
        // Basic utility functions
        virtual RlcRateScaleMove*                   clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      v;
        RevPtr<const Variable>                      lambda;
        RevPtr<const Variable>                      tune;
        
    };
    
}

#endif
