#ifndef Container_H
#define Container_H

#include "RevObject.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {

    /**
     * @brief Generic container base class
     *
     * Container is the base class for all generic Rev containers, that is, Rev objects with a type of the
     * form element type followed by one or more pairs of square brackets marking the dimension of the
     * container. For instance, a generic Rev vector of Real elements will have type "Real[]", and a
     * vector of probabilities will have type "Probability[][]". The actual generic vector types used
     * in Rev are classes deriving directly from either ModelContainer (for ModelObject elements) or
     * from WorkspaceContainer (for WorkspaceObject elements).
     *
     * More specialized containers derive from the generic container classes, and thus inherit all
     * of the container properties.
     *
     * Generic containers support assignment to individual elements by default, but derived classes
     * defining specialized classes typically override this behavior by appropriate modification of
     * the element access functions. Specifically, to protect the parser from assigning to container
     * elements, simply throw an appropriate error in the findOrCreateElement function.
     */
    class Container : public RevObject {
    
    public:
    
        // Basic utility functions you have to or might want to override
        virtual Container*                          clone(void) const = 0;                                              //!< Clone object
        static const std::string&                   getClassType(void);                                                 //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const = 0;                                        //!< Get the object type spec of the instance
        virtual void                                printStructure(std::ostream& o) const;                              //!< Print structure of language object for user
        virtual void                                printValue(std::ostream& o) const = 0;                              //!< Print value for user
    
        // Container functions you have to override
        virtual RevPtr<Variable>                    findOrCreateElement(const std::vector<size_t>& oneOffsetIndices) = 0;   //!< Find or create element (for direct element assignment)
        virtual size_t                              getDim(void) const = 0;                                                 //!< Get the dimension (1 for vector, 2 for matrix etc)
        virtual RevPtr<Variable>                    getElement(const std::vector<size_t>& oneOffsetIndices) = 0;            //!< Get element variable (vector of indices)
        virtual void                                setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths) = 0; //!< Set elements from Rev objects
        virtual size_t                              size(void) const = 0;                                                   //!< Get number of elements in container
    
        // Member method functions
        virtual const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        virtual RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions

        // Container function you do not have to override
        virtual RevPtr<Variable>                    getElement(size_t oneOffsetIndex);                                  //!< Get element variable (single index)
        
    protected:
        Container(void);                                                                                                //!< Default constructor
        Container(const MemberRules& memberRules);                                                                      //!< Constructor taking member rules

        Container&                                  operator=(const Container& x);                                      //!< Assignment operator

    };

}

#endif

