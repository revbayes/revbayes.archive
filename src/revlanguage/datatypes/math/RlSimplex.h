/**
 * @file
 * This file contains the declaration of TypedContainer, a container type
 * that acts as a specialized base class for all constant Containers. Additionally to the interface
 * container, this class provides the wrapper functionality for DAG node values.
 *
 * @brief Declaration of TypedContainer
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id: Container.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef Simplex_H
#define Simplex_H

#include "TypedContainer.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    class Simplex : public TypedContainer<std::vector<double> > {
        
    public:
        // the value type definition
        typedef std::vector<double> valueType;
        typedef std::vector<double>::iterator iterator;
        typedef std::vector<double>::const_iterator const_iterator;
        
        Simplex(void);                                                                                                  //!< Set type spec of container from type of elements
        Simplex(const std::vector<double>& v);                                                                          //!< Set type spec of container from type of elements
        Simplex(RevBayesCore::TypedDagNode<std::vector<double> > *c);                                                   //!< Set type spec of container from type of elements
        Simplex(const Simplex &s);                                                                                      //!< Set type spec of container from type of elements

        virtual                                    ~Simplex(void);                                                      //!< Destructor        

        Simplex&                                    operator=(const Simplex& x);                                    //!< Assignment operator

        // Basic utility functions you have to override
        virtual Simplex*                            clone(void) const;                                                  //!< Clone object
        static const std::string&                   getClassName(void);                                             //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                         //!< Get class type spec
            
        // Basic utility functions you should not have to override
        const TypeSpec&                             getTypeSpec(void) const;                                        //!< Get language type of the object
        const valueType&                            getValue(void) const;
        RevBayesCore::TypedDagNode<valueType>*      getDagNode(void) const;
        bool                                        isConstant(void) const;                                             //!< Is this variable and the internally stored deterministic node constant?
        void                                        makeConstantValue();                                                //!< Convert the stored variable to a constant variable (if applicable)
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user
        void                                        setName(const std::string &n);                                      //!< Set the name of the variable (if applicable)
        void                                        replaceVariable(RevObject *newVar);                          //!< Replace the internal DAG node
        
        // function you might want to overwrite
        virtual RevObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
        virtual RevObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        virtual const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods (const)
        
        // Container functions you should not have to override
        iterator                                    begin(void);                                                    //!< Iterator to the beginning of the Vector
        const_iterator                              begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
        void                                        clear(void);                                                    //!< Clear
        iterator                                    end(void);                                                      //!< Iterator to the end of the Vector
        const_iterator                              end(void) const;                                                //!< Const-iterator to the end of the Vector
        int                                         findIndex(const RevObject& x) const;                     //!< Find the index if the element being equal to that one
        void                                        pop_back(void);                                                 //!< Drop element at back
        void                                        pop_front(void);                                                //!< Drop element from front
        size_t                                      size(void) const;                                               //!< get the number of elements in the AbstractVector
        
        // Container functions you have to overwrite
        virtual RevObject*                   getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
        virtual void                                sort(void);                                                 //!< sort the AbstractVector
        virtual void                                unique(void);                                               //!< removes consecutive duplicates
        
    protected:
        
//        RevBayesCore::TypedDagNode<valueType>*      value;
        
    };
    
}


#endif

