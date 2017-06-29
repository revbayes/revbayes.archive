#ifndef Simplex_H
#define Simplex_H


#include "ModelVector.h"
#include "RealPos.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief Rev simplex variable class
     *
     * This class is derived from ModelVector<RealPos> and inherits all
     * its indexing and type conversion functionality. The only changes
     * we need to introduce here are to override some vector functions
     * that are not applicable to simplices (sort, unique) and to override
     * the two functions providing assignment or external modification
     * abilities to the parser (findOrCreateElement, getElement).
     */
    
    class Simplex : public ModelVector<RealPos> {
        
        public:
                                                        Simplex(void);                                                              //!< Construct empty simplex
                                                        Simplex(const RevBayesCore::RbVector<double>& v);                           //!< Construct simplex from double (real) vector
                                                        Simplex(RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* c);    //!< Construct simplex from DAG node

            virtual                                    ~Simplex(void);                                                              //!< Destructor

            // Basic utility functions you have to override
            virtual Simplex*                            clone(void) const;                                                          //!< Clone object
            static const std::string&                   getClassType(void);                                                         //!< Get Rev type
            static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
            virtual const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
            
            // ModelVector functions that we override here to stop inappropriate actions
            void                                        sort(void);                                                                 //!< Sort vector
            void                                        unique(void);                                                               //!< Remove consecutive duplicates
            virtual bool                                allowsModificationToCompositeContainer(void) const { return false;}         //!< Does an object of this type allow transformation into a composite container?

            std::string                                 getGuiName(void) { return "Simplex"; }
            std::string                                 getGuiUnicodeSymbol(void) { return "\\delta"; }
            std::string                                 getGuiInfo(void) { return ""; }

        private:
            RevBayesCore::RbVector<double>*             makeNormalizedValue(const RevBayesCore::RbVector<double>& v);               //!< Help function
    };
}

#endif

