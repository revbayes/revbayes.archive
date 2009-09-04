#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include "RbObject.h"

class RandomNumberGenerator : public RbObject {

	friend class RandomNumberFactory;

public:
	int nextInt(int max);
	unsigned int nextUnsignedInt(unsigned int max);
	double nextDouble(void);
	double nextDouble(double max);
	void getSeed(unsigned int &seed1, unsigned int &seed2);
	void setSeed(unsigned int seed1, unsigned int seed2);

private:
	RandomNumberGenerator(unsigned int x);
	RandomNumberGenerator(unsigned int x, unsigned int y);
	RandomNumberGenerator(void) {}
	double uniform01(void);
	unsigned int I1, I2;
};

#endif

