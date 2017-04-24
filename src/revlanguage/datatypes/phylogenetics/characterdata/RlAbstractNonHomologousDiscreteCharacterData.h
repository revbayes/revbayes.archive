#ifndef RlAbstractNonHomologousDiscreteCharacterData_H
#define RlAbstractNonHomologousDiscreteCharacterData_H

#include "AbstractNonHomologousDiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlNonHomologousCharacterData.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractNonHomologousDiscreteCharacterData : public NonHomologousCharacterData {
        
    public:
        AbstractNonHomologousDiscreteCharacterData(void);                                                                                                  //!< Constructor requires character type
        AbstractNonHomologousDiscreteCharacterData(const RevBayesCore::AbstractNonHomologousDiscreteCharacterData &d);                                              //!< Constructor requires character type
        AbstractNonHomologousDiscreteCharacterData(RevBayesCore::AbstractNonHomologousDiscreteCharacterData *d);                                              //!< Constructor requires character type
        AbstractNonHomologousDiscreteCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractNonHomologousDiscreteCharacterData>*d);                   //!< Constructor requires character type
        AbstractNonHomologousDiscreteCharacterData(const AbstractNonHomologousDiscreteCharacterData &d);                                                                                                //!< Constructor requires character type
        
        virtual ~AbstractNonHomologousDiscreteCharacterData();
        
        
        AbstractNonHomologousDiscreteCharacterData&            operator=(const AbstractNonHomologousDiscreteCharacterData& x);                            //!< Assignment operator
        
        
        typedef RevBayesCore::AbstractNonHomologousDiscreteCharacterData valueType;

        // Basic utility functions
        virtual AbstractNonHomologousDiscreteCharacterData*     clone(void) const;                                                                      //!< Clone object
        static const std::string&                               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>                             executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
        // Basic utility functions you should not have to override
        bool                                                    isAssignable(void) const;                                                               //!< Is object or upstream members assignable?
        bool                                                    isConstant(void) const;                                                                 //!< Is this variable and the internally stored deterministic node constant?
        bool                                                    isModelObject(void) const;                                                              //!< Is this object a model object. Yes, so we return true.
        void                                                    makeConstantValue(void);                                                                //!< Convert to constant object
        void                                                    makeConversionValue(RevPtr<RevVariable> var);                                           //!< Convert to conversion object
        AbstractNonHomologousDiscreteCharacterData*             makeIndirectReference(void);                                                            //!< Make reference to object
        void                                                    makeUserFunctionValue(UserFunction* fxn);                                               //!< Convert to user-defined Rev function object
        void                                                    setDagNode(RevBayesCore::DagNode *newNode);                                             //!< Set or replace the internal dag node (and keep me)
        void                                                    setName(const std::string &n);                                                          //!< Set the name of the variable (if applicable)
        void                                                    replaceVariable(RevObject *newVar);                                                     //!< Replace the internal DAG node (and prepare to replace me...)
        
        // Getters and setters
        RevBayesCore::TypedDagNode<valueType>*                  getDagNode(void) const;                                                                 //!< Get the internal DAG node
        virtual const valueType&                                getValue(void) const;                                                                   //!< Get the value (const)
        virtual valueType&                                      getValue(void);                                                                         //!< Get the value (non-const)
        void                                                    printValue(std::ostream& o, bool user) const;                                          //!< Print value for user
        void                                                    setValue(valueType *x);                                                                 //!< Set new constant value
        
    protected:
        
        
        RevBayesCore::TypedDagNode<valueType>*                  dag_node;
        
        
    };
    
}


#endif

