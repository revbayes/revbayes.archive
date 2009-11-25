#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include <string>
#include "RbObject.h"

class RandomNumberGenerator : public RbComplex {
    public:

        static const StringVector   rbClass;            //!< Static class attribute

        RandomNumberGenerator(int s);
        RandomNumberGenerator(std::string n, int s);
        RandomNumberGenerator(const RandomNumberGenerator& m);
        virtual ~RandomNumberGenerator();

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object
        bool                       isType(std::string t) const { return rbClass[0] == t; }
        StringVector&              operator+(const StringVector& sv) const;

		int nextInt(int max);
		unsigned int nextUnsignedInt(unsigned int max);
		double nextDouble(void);
		double nextDouble(double max);

	private:
		double uniform01(void);
		int seed;
};

#endif

