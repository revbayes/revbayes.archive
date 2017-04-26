//
//  RlCladogeneticProbabilityMatrix.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/23/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef RlCladogeneticProbabilityMatrix_h
#define RlCladogeneticProbabilityMatrix_h

#include <stdio.h>
#include "MatrixReal.h"
#include "ModelObject.h"
#include "CladogeneticProbabilityMatrix.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class CladogeneticProbabilityMatrix : public ModelObject<RevBayesCore::CladogeneticProbabilityMatrix> {
        
    public:
        
        CladogeneticProbabilityMatrix(void);                                                                                                           //!< Default constructor
        CladogeneticProbabilityMatrix(const RevBayesCore::CladogeneticProbabilityMatrix& m);                                                                              //!< Default constructor
        CladogeneticProbabilityMatrix(RevBayesCore::CladogeneticProbabilityMatrix *m);                                                                                    //!< Default constructor
        CladogeneticProbabilityMatrix(RevBayesCore::TypedDagNode<RevBayesCore::CladogeneticProbabilityMatrix> *d);                                                                                                        //!< Default constructor
        
        // Basic utility functions
        CladogeneticProbabilityMatrix*      clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method functions
        virtual RevPtr<RevVariable>         executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Map member methods to internal functions
<<<<<<< HEAD

        std::string                         getGuiName(void) { return ""; }
        std::string                         getGuiSymbol(void) { return ""; }
        std::string                         getGuiInfo(void) { return ""; }
=======
>>>>>>> c54d844d6bccb46f193d8e4b18ac6f817cb88097
        
    protected:
        void                                initMethods(void);
    };
    
}

#endif /* RlCladogeneticProbabilityMatrix_h */
