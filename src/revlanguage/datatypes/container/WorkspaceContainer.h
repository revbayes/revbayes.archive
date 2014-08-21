#ifndef WorkspaceContainer_H
#define WorkspaceContainer_H

#include "Container.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    
    /**
     * @brief Base class for workspace containers holding Rev objects
     *
     * In this class we keep a vector of variables containing the elements of
     * the container. Because we always have access to the Rev language elements,
     * the implementation is more straightforward than for ModelContainer. The
     * base class need not be templated, and we do not need to worry about DAG 
     * nodes controlling our elements.
     */
    class WorkspaceContainer : public Container {
        
    public:
        
        virtual                                     ~WorkspaceContainer(void);                                          //!< Virtual destructor
        
        // STL-like container functions provided here
        void                                        clear(void);                                                        //!< Clear
        size_t                                      size(void) const;                                                   //!< Get number of elements

        // Basic utility functions you have to override
        virtual Container*                          clone(void) const = 0;                                              //!< Clone object
        static const std::string&                   getClassType(void);                                                 //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                     getTypeSpec(void) const = 0;                                        //!< Get the object type spec of the instance
        virtual void                                printValue(std::ostream& o) const = 0;                              //!< Print value for user
        
        // Basic utility function you may want to override
        virtual void                                printStructure(std::ostream& o, bool verbose=false) const;          //!< Print structure of language object for user

        // Container functions you have to override
        RevPtr<Variable>                            findOrCreateElement(const std::vector<size_t>& oneOffsetIndices) = 0;   //!< Find or create element variable
        size_t                                      getDim(void) const = 0;                                                 //!< Get the dimensions
        RevPtr<Variable>                            getElement(const std::vector<size_t>& oneOffsetIndices) = 0;            //!< Get element variable
        virtual void                                setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths) = 0; //!< Set elements from Rev objects

    protected:
                                                    WorkspaceContainer(void);                                           //!< Construct empty container
                                                    WorkspaceContainer(const WorkspaceContainer& c);                    //!< Copy constructor
        
        // Assignment operator
        WorkspaceContainer&                         operator=(const WorkspaceContainer& x);                             //!< Assignment operator

        // Member variable
        std::vector< RevPtr<Variable> >             elements;                                                           //!< The container elements
    };
}


#endif
