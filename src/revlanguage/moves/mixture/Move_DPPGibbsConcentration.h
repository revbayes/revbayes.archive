#ifndef Move_DPPGibbsConcentration_H
#define Move_DPPGibbsConcentration_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on all elements of a real valued vector.
     *
     * This class is the RevLanguage wrapper of Move_DPPGibbsConcentration.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    class Move_DPPGibbsConcentration : public Move {
        
    public:
        
        Move_DPPGibbsConcentration(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_DPPGibbsConcentration*                        clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassType(void);                                                                             //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        RevPtr<const Variable>                      cp;                                                                                              //!< The variable holding the real valued vector.
        RevPtr<const Variable>                      numCats;                                                                                         //!< The variable for the tuning parameter.
        RevPtr<const Variable>                      gammaShape;
        RevPtr<const Variable>                      gammaRate;
        RevPtr<const Variable>                      numElements;
        
    };
    
}

#endif
