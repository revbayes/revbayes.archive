#ifndef RlContinuousCharacterData_H
#define RlContinuousCharacterData_H

#include <iostream>


#include "ContinuousCharacterData.h"
#include "ModelObject.h"
#include "RlHomologousCharacterData.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ContinuousCharacterData : public HomologousCharacterData {
        
    public:
        ContinuousCharacterData(void);                                                                                                                      //!< Default constructor
        ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d);                                                                            //!< Constructor from new core data type pointer
        ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d);                                                                                  //!< Constructor from new core data type pointer
        ContinuousCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData>*d);                                                       //!< Constructor with DAG node
        ContinuousCharacterData(const ContinuousCharacterData &d);                                                                                          //!< Default constructor
        
        virtual ~ContinuousCharacterData();
        
        ContinuousCharacterData&                                operator=(const ContinuousCharacterData& x);                                                //!< Assignment operator

        
        typedef RevBayesCore::ContinuousCharacterData valueType;

        // Basic utility functions
        ContinuousCharacterData*                                clone(void) const;                                                                          //!< Clone object
        ContinuousCharacterData*                                concatenate(const RevObject& d, std::string type = "") const;                                                      //!< Concatenate two sequences
        ContinuousCharacterData*                                concatenate(const ContinuousCharacterData& d, std::string type = "") const;                                        //!< Concatenate two sequences
        static const std::string&                               getClassType(void);                                                                         //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                     //!< Get class type spec
        const TypeSpec&                                         getTypeSpec(void) const;                                                                    //!< Get language type of the object

        RevPtr<RevVariable>                                     executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found);     //!< Execute member method

        // Basic utility functions you should not have to override
        bool                                                    isAssignable(void) const;                                                                   //!< Is object or upstream members assignable?
        bool                                                    isConstant(void) const;                                                                     //!< Is this variable and the internally stored deterministic node constant?
        bool                                                    isModelObject(void) const;                                                                  //!< Is this object a model object. Yes, so we return true.
        void                                                    makeConstantValue(void);                                                                    //!< Convert to constant object
        void                                                    makeConversionValue(RevPtr<RevVariable> var);                                               //!< Convert to conversion object
        ContinuousCharacterData*                                makeIndirectReference(void);                                                                //!< Make reference to object
        void                                                    makeUserFunctionValue(UserFunction* fxn);                                                   //!< Convert to user-defined Rev function object
        void                                                    setDagNode(RevBayesCore::DagNode *newNode);                                                 //!< Set or replace the internal dag node (and keep me)
        void                                                    setName(const std::string &n);                                                              //!< Set the name of the variable (if applicable)
        void                                                    replaceVariable(RevObject *newVar);                                                         //!< Replace the internal DAG node (and prepare to replace me...)
        
        // Getters and setters
        RevBayesCore::TypedDagNode<valueType>*                  getDagNode(void) const;                                                                     //!< Get the internal DAG node
        virtual const valueType&                                getValue(void) const;                                                                       //!< Get the value (const)
        virtual valueType&                                      getValue(void);                                                                             //!< Get the value (non-const)
        void                                                    printValue(std::ostream& o, bool user) const;                                                          //!< Print value for user
        void                                                    setValue(valueType *x);                                                                     //!< Set new constant value
        
    protected:
        
        RevBayesCore::TypedDagNode<valueType>*                  dagNode;

    };
    
}

#endif /* defined(__revbayes__RlContinuousCharacterData__) */
