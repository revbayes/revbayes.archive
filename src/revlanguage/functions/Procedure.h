#ifndef Procedure_H
#define Procedure_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Procedure : public Function {
        
    public:
        virtual                                         ~Procedure(void);                                                                   //!< Destructor
        
        void bogusFoo() {}
        
        virtual RevPtr<RevVariable>                        execute(void) = 0;                                                                  //!< Create a random variable from this distribution
        
        // Basic utility functions you have to override
        virtual Procedure*                              clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        
        
    protected:
        Procedure(void);                                                                                                                    //!< Basic constructor
        
    };
    
}


#endif


