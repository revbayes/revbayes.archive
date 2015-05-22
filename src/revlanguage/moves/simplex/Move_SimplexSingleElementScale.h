#ifndef Move_SimplexSingleElementScale_H
#define Move_SimplexSingleElementScale_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_SimplexSingleElementScale : public Move {
        
    public:
        
        Move_SimplexSingleElementScale(void);                                                                               //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move_SimplexSingleElementScale*     clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      x;
        RevPtr<const RevVariable>                      alpha;
        RevPtr<const RevVariable>                      tune;
        
    };
    
}

#endif
