#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include <string>
#include <vector>
#include "RbObject.h"
#include "RbComplex.h"
#include "VectorString.h"

class RandomNumberGenerator : public RbComplex {

    public:

		                            RandomNumberGenerator(void);                          //!< Default constructor using time seed
    		                        RandomNumberGenerator(std::vector<unsigned int> s);   //!< Constructor explicitly setting seeds

        // Basic utility functions
        RandomNumberGenerator*      clone(void) const;                                    //!< Clone object
        bool						equals(const RbObject* obj) const;                    //!< Equals comparison
        const VectorString&			getClass(void) const;                                 //!< Get class
        void						printValue(std::ostream& o) const;                    //!< Print value (for user)
        std::string					toString(void) const;                                 //!< General info on object

        // Regular functions
        void						setSeed(std::vector<unsigned int> s);                 //!< Set the seeds of the RNG
		double                      uniform01(void);                                      //!< Get a random [0,1) var

	protected:
        // Protected conversion functions
        RbObject*                   convertTo(const std::string& type) const;             //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;       //!< Is convertible to type and dim?

	private:
		std::string					name;											      //!< Name of selected random number generator
		std::vector<unsigned int>	seed;											      //!< Vector of two seed values
};

#endif

