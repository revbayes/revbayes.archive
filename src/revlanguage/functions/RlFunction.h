/**
 * @file
 * This file contains the declaration of Function, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of Function
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Function
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef RlFunction_H
#define RlFunction_H

#include <string>
#include <vector>

#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Environment.h"
#include "RevPtr.h"

/**
 * This is the interface and abstract base class for functions in
 * RevBayes. Function instances are put in the function table in the
 * relevant Environment (user workspace or base environment) if they are
 * global. If they are member functions of user-defined type, they
 * are instead associated with the function table of the approprioate
 * class in the class table of the user workspace.
 * 
 * A function instance knows its argument rules and can process a
 * vector of labeled argument values according to these rules to pro-
 * duce a ready-to-use list of arguments. The processing involves label
 * matching as well as filling of missing values with default values.
 * The processing of labeled argument values is done in the function
 * processArguments, which will throw an error if the provided
 * arguments do not match.
 *
 */

namespace RevLanguage {

    class Function : public RevObject {
    public:
        virtual ~Function(void); //!< Destructor
        Function(const Function &x); //!< Copy constuctor

        // Basic utility functions you have to override
        virtual Function* clone(void) const = 0; //!< Clone object
        static const std::string& getClassName(void); //!< Get class name
        static const TypeSpec& getClassTypeSpec(void); //!< Get class type spec

        // Basic utility functions you may want to override
        virtual std::string                             callSignature(void) const;                                                          //!< Return call signature

        // Basic utility functions you should not have to override
        const std::string&                              getName(void) const;                                                                //!< Get the name of the function
        std::string                                     getRevDeclaration(void) const;                                                      //!< Get Rev declaration of the function
        void                                            printStructure(std::ostream& o) const;                                              //!< Print the structure of language object for user
        void                                            printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')
        void                                            setExecutionEnviroment(Environment *e);                                             //!< Set the environment from which the function was executed.
        void                                            setName(const std::string& nm);                                                     //!< Name the function
    
        // Function functions you have to override
        virtual RevObject*                              execute(void) = 0;                                                                  //!< Execute function
        virtual const ArgumentRules&                    getArgumentRules(void) const = 0;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const = 0;                                                      //!< Get type of return value

        // Function function you may want to override
        virtual bool                                    checkArguments(const std::vector<Argument>& passedArgs, std::vector<unsigned int>* matchScore); //!< Process args, return a match score if pointer is not null
        virtual void                                    processArguments(const std::vector<Argument>& passedArgs);                          //!< Process args, return a match score if pointer is not null

        virtual bool throws(void) const {
            return false;
        } //!< Does the function throw exceptions?


        // Function functions you should not override
        void                                            clear(void);                                                                        //!< Clear argument Environment "args"
        const std::vector<Argument>&                    getArguments(void) const;                                                           //!< Get processed arguments in argument Environment "args"
        std::vector<Argument>&                          getArguments(void);                                                                 //!< Get processed arguments in argument Environment "args"
        void                                            setArgument(const std::string& name, Argument& arg, bool c);                        //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
	protected:
                                                        Function(void);                                                                     //!< Basic constructor
    
        // function you may want to override
        virtual void                                    clearArguments(void); //!< Clear argument Environment "args"

        virtual void                                    setArgumentVariable(const std::string& name, const RevPtr<const Variable> &var) {}  //!< Set the private member variable here (for derived classes)!

        // Member variables
        bool                                            argsProcessed;                                                                      //!< Are arguments processed?
        std::vector<Argument>                           args;
        Environment*                                    env;
        std::string                                     name;                                                                               //!< The name of the function in the environment

    private:
        int                                             computeMatchScore(const Variable* arg, const ArgumentRule& rule);
};
    
}

#endif
