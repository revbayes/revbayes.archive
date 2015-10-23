#ifndef Procedure_H
#define Procedure_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Procedure : public Function {
        
    public:
        virtual                                            ~Procedure(void);                                                                   //!< Destructor
        
        virtual RevPtr<RevVariable>                         execute(void) = 0;                                                                  //!< Create a random variable from this distribution
        virtual const std::string&                          getFunctionName(void) const = 0;                                                                //!< Get the name of the function
        virtual const std::vector<std::string>&             getFunctionNameAliases(void) const = 0;                                                                //!< Get the name of the function
        
        // Basic utility functions you have to override
        virtual Procedure*                                  clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                           getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        
        
    protected:
        Procedure(void);                                                                                                                    //!< Basic constructor
        
        virtual std::string                                 getHelpAuthor(void) const = 0;
        virtual std::vector<std::string>                    getHelpDescription(void) const = 0;
        virtual std::vector<std::string>                    getHelpDetails(void) const = 0;
        virtual std::string                                 getHelpExample(void) const = 0;
        virtual std::vector<RevBayesCore::RbHelpReference>  getHelpReferences(void) const = 0;
        virtual std::vector<std::string>                    getHelpSeeAlso(void) const = 0;
        virtual std::string                                 getHelpTitle(void) const = 0;
        virtual std::string                                 getHelpUsage(void) const = 0;

    };
    
}


#endif


