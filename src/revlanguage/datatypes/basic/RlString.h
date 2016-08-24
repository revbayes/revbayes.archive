#ifndef RlString_H
#define RlString_H

#include "ModelObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"

#include <fstream>
#include <string>

namespace RevLanguage {

    class RlString : public ModelObject<std::string> {

        public:
            RlString(void);                                                                         //!< Default: empty RlString
            RlString(const std::string& v);                                                         //!< Constructor from RlString
            RlString(RevBayesCore::TypedDagNode<std::string> *x);                                   //!< Construct from DAG node
        
            // Basic operator functions
            RevObject*                      add(const RevObject &rhs) const;                        //!< Addition operator used for example in '+=' statements
            RlString*                       add(const RlString &rhs) const;                         //!< Addition operator used for example in '+=' statements

            // Basic utility functions
            RlString*                       clone(void) const;                                      //!< Copy
            static const std::string&       getClassType(void);                                     //!< Get Rev type
            static const TypeSpec&          getClassTypeSpec(void);                                 //!< Get class type spec
            const TypeSpec&                 getTypeSpec(void) const;                                //!< Get language type of the object
        
        protected:

        private:
            void                            parseValue(void);
    };
    
}

#endif

