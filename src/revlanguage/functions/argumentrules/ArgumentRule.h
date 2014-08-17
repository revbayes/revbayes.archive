#ifndef ArgumentRule_H
#define ArgumentRule_H

#include "Environment.h"
#include "RevPtr.h"
#include "TypeSpec.h"

#include <string>

namespace RevLanguage {

    /**
     * @brief Argument rule class
     *
     * The argument rule class describes the rules for fitting an
     * argument (a variable) into an argument or a member variable
     * slot.
     */
    class ArgumentRule {

    public: 
        ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp);                                        //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp, RevObject *defVal);                     //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp);                           //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp, RevObject *defVal);        //!< Constructor of rule without default value
        virtual                            ~ArgumentRule(void) {}
        
        // Basic utility functions
        virtual ArgumentRule*               clone(void) const;                                                              //!< Clone object
        virtual void                        printValue(std::ostream& o) const;                                              //!< Print value for user

        // ArgumentRule functions
        Argument                            fitArgument( Argument& arg, bool once ) const;                                  //!< Fit a passed argument into a slot using the rule
        const std::string&                  getArgumentLabel(void) const;                                                   //!< Get label of argument
        const std::vector<TypeSpec>&        getArgumentTypeSpec(void) const;                                                //!< Get argument type spec
        virtual const Variable&             getDefaultVariable(void) const;                                                 //!< Get default argument
        bool                                hasDefault(void) const;                                                         //!< Does the rule have a default value?
        virtual bool                        isArgumentValid(const RevPtr<const Variable> &var, bool once) const;            //!< Is var a valid argument?
        bool                                isConstant(void) const;                                                         //!< Is this a 'const' rule?
        virtual bool                        isEllipsis(void) const;                                                         //!< Is this an ellipsis?
    

    protected:
        std::vector<TypeSpec>               argTypeSpecs;                                                                   //!< Type specs
        Variable*                           defaultVar;                                                                     //!< Default value
        bool                                isConst;                                                                        //!< Is rule const?
        std::string                         label;                                                                          //!< Label of argument
        bool                                hasDefaultVal;                                                                  //!< Has default

    };
    
}

#endif
