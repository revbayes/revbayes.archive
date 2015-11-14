#ifndef RlDistanceMatrix_H
#define RlDistanceMatrix_H

#include "ModelObject.h"
#include "DistanceMatrix.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Real;
    
    class RlDistanceMatrix : public ModelObject<RevBayesCore::DistanceMatrix>  {
        
    public:
        
        RlDistanceMatrix(void);                                                                                                              //!< Default constructor
        RlDistanceMatrix(const RevBayesCore::DistanceMatrix &m);                                                                                        //!< Default constructor
        RlDistanceMatrix(RevBayesCore::DistanceMatrix *m);                                                                                        //!< Default constructor
        RlDistanceMatrix(RevBayesCore::TypedDagNode<RevBayesCore::DistanceMatrix> *d);                                                            //!< Default constructor

        
        // Basic utility functions
        RlDistanceMatrix*                   clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
		

		// STL-like vector functions provided here
	//	virtual void                                push_back(const RevObject &x);                                  //!< Append element to end
	//	virtual RevObject*                          getElement(size_t idx) const;                                   //!< Get element variable
		virtual size_t                      size(void) const;                                               //!< Get number of elements in container

		const Real*                         getElement(size_t idx, size_t idy) const;
		
		void                                setElement(size_t idx, size_t idy, double& value) ;


        // Member method inits
        RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    private:
            
    };


	
}
#endif /* defined(RlDistanceMatrix_H) */
