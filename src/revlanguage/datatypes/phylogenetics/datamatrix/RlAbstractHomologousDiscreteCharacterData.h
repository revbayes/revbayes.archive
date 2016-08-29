#ifndef RlAbstractHomologousDiscreteCharacterData_H
#define RlAbstractHomologousDiscreteCharacterData_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlHomologousCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractHomologousDiscreteCharacterData : public HomologousCharacterData {
        
    public:
        AbstractHomologousDiscreteCharacterData(void);                                                                                                  //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(const RevBayesCore::AbstractHomologousDiscreteCharacterData &d);                                              //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(RevBayesCore::AbstractHomologousDiscreteCharacterData *d);                                              //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>*d);                   //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(const AbstractHomologousDiscreteCharacterData &d);                                                                                                //!< Constructor requires character type
        
        virtual ~AbstractHomologousDiscreteCharacterData();
        
        
        AbstractHomologousDiscreteCharacterData&                operator=(const AbstractHomologousDiscreteCharacterData& x);                            //!< Assignment operator
        
        
        typedef RevBayesCore::AbstractHomologousDiscreteCharacterData valueType;
        
        
        // Basic utility functions
        virtual AbstractHomologousDiscreteCharacterData*        clone(void) const;                                                                      //!< Clone object
        static const std::string&                               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>                             executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
        AbstractHomologousDiscreteCharacterData*                concatenate(const RevObject& d) const;                                                  //!< Concatenate two sequences
        AbstractHomologousDiscreteCharacterData*                concatenate(const AbstractHomologousDiscreteCharacterData& d) const;                    //!< Concatenate two sequences
        
        
        // Basic utility functions you should not have to override
        bool                                                    isAssignable(void) const;                                                               //!< Is object or upstream members assignable?
        bool                                                    isConstant(void) const;                                                                 //!< Is this variable and the internally stored deterministic node constant?
        bool                                                    isModelObject(void) const;                                                              //!< Is this object a model object. Yes, so we return true.
        void                                                    makeConstantValue(void);                                                                //!< Convert to constant object
        void                                                    makeConversionValue(RevPtr<RevVariable> var);                                           //!< Convert to conversion object
        AbstractHomologousDiscreteCharacterData*                makeIndirectReference(void);                                                            //!< Make reference to object
        void                                                    makeUserFunctionValue(UserFunction* fxn);                                               //!< Convert to user-defined Rev function object
        void                                                    setDagNode(RevBayesCore::DagNode *newNode);                                             //!< Set or replace the internal dag node (and keep me)
        void                                                    setName(const std::string &n);                                                          //!< Set the name of the variable (if applicable)
        void                                                    replaceVariable(RevObject *newVar);                                                     //!< Replace the internal DAG node (and prepare to replace me...)
        
        // Getters and setters
        RevBayesCore::TypedDagNode<valueType>*                  getDagNode(void) const;                                                                 //!< Get the internal DAG node
        virtual const valueType&                                getValue(void) const;                                                                   //!< Get the value (const)
        virtual valueType&                                      getValue(void);                                                                         //!< Get the value (non-const)
        void                                                    printValue(std::ostream& o, bool user) const;                                                      //!< Print value for user
        void                                                    setValue(valueType *x);                                                                 //!< Set new constant value
        
    protected:

        RevBayesCore::TypedDagNode<valueType>*                  dagNode;
        
    private:
        
        void                                                    initMethods(void);
        
    };
    
}


#endif

