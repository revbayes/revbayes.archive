#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include <string>
#include <vector>
#include "RbObject.h"
#include "RbComplex.h"
#include "StringVector.h"

class RandomNumberGenerator : public RbComplex {

    public:

		RandomNumberGenerator(void);
		RandomNumberGenerator(std::vector<unsigned int> s);
        RandomNumberGenerator(std::string n, std::vector<unsigned int> s);
        RandomNumberGenerator(const RandomNumberGenerator& m);
        virtual ~RandomNumberGenerator(void);

        // Basic utility functions
        RbObject*					clone() const;                                  //!< Clone object
        bool						equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&			getClass(void) const;                           //!< Get class
        void						print(std::ostream& o) const;                   //!< Print complete object info
        void						printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string					toString(void) const;                           //!< General info on object

        RbObject&					operator=(const RbObject& obj);
        RandomNumberGenerator&		operator=(const RandomNumberGenerator& obj);

        void						setSeed(std::vector<unsigned int> s);
		int							nextInt(int max);
		unsigned int				nextUnsignedInt(unsigned int max);
		double						rv01(void) { return nextDouble(); }             //!< Get a random [0,1) var
		double						nextDouble(void);                             
		double						nextDouble(double max);

	protected:
        RbObject*                   convertTo(const std::string& type) const;       //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const; //!< Is convertible to type and dim?

	private:
	
		double uniform01(void);
		std::string					name;											// name of selected random number generator
		std::vector<unsigned int>	seed;											// list of seed values
};

#endif

