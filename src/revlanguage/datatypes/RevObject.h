/**
 * @file
 * This file contains the declaration of RevObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RevObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-07-27
 *
 * $Id$
 */

#ifndef RevObject_H
#define RevObject_H

#include "Cloneable.h"
#include "DagNode.h"
#include "MethodTable.h"
#include "RevPtr.h"

#include <ostream>
#include <string>
#include <vector>


namespace RevLanguage {

    class Argument;
    class ArgumentRule;
    class ArgumentRules;
    class TypeSpec;
    class UserFunction;
    class UserFunctionCall;
    class Variable;

    typedef ArgumentRules MemberRules;                                                                                                  //!< Member rules type def, for convenience

    class RevObject : public RevBayesCore::Cloneable {
    
    public:
        virtual                            ~RevObject(void);                                                                                //!< Virtual destructor
        
        // Overloaded C++ operators
        bool                                operator==(const RevObject& x) const { return &x == this; }
        bool                                operator!=(const RevObject& x) const { return !operator==(x); }
        bool                                operator<(const RevObject& x) const { return false; }
    
        // Basic operator functions
        virtual RevObject*                  add(const RevObject &rhs) const;                                                                //!< Addition operator used for example in '+=' statements
        virtual void                        decrement(void);                                                                                //!< Decrement operator used for example in 'a--' statements
        virtual RevObject*                  divide(const RevObject &rhs) const;                                                             //!< Division operator used for example in '/=' statements
        virtual void                        increment(void);                                                                                //!< Increment operator used for example in 'a++' statements
        virtual RevObject*                  multiply(const RevObject &rhs) const;                                                           //!< Multiplication operator used for example in '-=' statements
        virtual RevObject*                  subtract(const RevObject &rhs) const;                                                           //!< Subtraction operator used for example in '*=' statements
    
        // Basic utility functions you have to override (also getClassTypeSpec()!)
        virtual RevObject*                  clone(void) const = 0;                                                                          //!< Clone object
        static const std::string&           getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                         //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const = 0;                                                                    //!< Get the object type spec of the instance
        virtual void                        printStructure(std::ostream& o, bool verbose=false) const = 0;                                  //!< Print structure of language object for user
        virtual void                        printValue(std::ostream& o) const = 0;                                                          //!< Print value for user
        virtual std::string                 toString(void) const;                                                                           //!< Get this object as a string, i.e., get some info about it.
    
        // Basic utility functions you may want to override
        virtual RevObject*                  cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const;      //!< Clone the model DAG connected to this node
        virtual void                        constructInternalObject(void);                                                                  //!< Objects using the ConstructorFunction should overwrite this function for proper initializiation of the internal objects.
        virtual RevObject*                  convertTo(const TypeSpec& type) const;                                                          //!< Convert to type
        virtual RevPtr<Variable>            executeMethod(const std::string& name, const std::vector<Argument>& args);                      //!< Override to map member methods to internal functions
        virtual RevBayesCore::DagNode*      getDagNode(void) const;                                                                         //!< Get my internal value node (if applicable)
        virtual const MemberRules&          getParameterRules(void) const;                                                                     //!< Get member rules
        virtual void                        initializeMethods(void) const;                                                                  //!< Initialize member methods
        virtual bool                        isAbstract(void) const;                                                                         //!< Is this an abstract type/object?
        virtual bool                        isAssignable(void) const;                                                                       //!< Is object or upstream members assignable?
        virtual bool                        isConstant(void) const;                                                                         //!< Is this variable and the internally stored deterministic node constant?
        virtual bool                        isConvertibleTo(const TypeSpec& type, bool once) const;                                         //!< Is convertible to type?
        virtual bool                        isModelObject(void) const;                                                                      //!< Is this a model object (compared with a workspace object)
        virtual void                        makeConstantValue(void);                                                                        //!< Convert the stored variable to a constant variable (if applicable)
        virtual void                        makeConversionValue(RevPtr<Variable> var);                                                      //!< Convert the object to a conversion object
        virtual RevObject*                  makeIndirectReference(void);                                                                    //!< Make an object referencing the dag node of this object
        virtual void                        makeUserFunctionValue(UserFunction* fxn);                                                       //!< Convert to deterministic object with a userdefined Rev function
        virtual void                        replaceVariable(RevObject *newObj);                                                             //!< Replace the internal DAG node and prepare to replace me
        virtual void                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);                  //!< Set member variable
        virtual void                        setParameter(const std::string& name, const RevPtr<Variable> &var);                             //!< Set member variable
        virtual void                        setName(const std::string &n);                                                                  //!< Set the name of the variable (if applicable)

        // Basic utility functions you should not have to override
        const std::string&                  getType(void) const;                                                                            //!< Get the type as a string
        bool                                isTypeSpec(const TypeSpec& typeSpec) const;                                                     //!< Does the language type of the object fit type specification typeSpec?
        const MethodTable&                  getMethods(void) const;                                                                         //!< Get member methods
    
    protected:
        
        RevObject(void) { }                                                                                                                  //!< No objects of this class
    
        // members
        mutable MethodTable                 methods;
        mutable bool                        methodsInitialized;
    };
    
    // Global functions using the class
    std::ostream&                           operator<<(std::ostream& o, const RevLanguage::RevObject& x);                                   //!< Overloaded output operator

}




#endif
