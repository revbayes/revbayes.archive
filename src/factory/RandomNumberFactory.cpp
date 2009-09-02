#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include <limits>



RandomNumberFactory::RandomNumberFactory(void) {

	setSeed();
	seedGenerator = new RandomNumberGenerator(I1, I2);
}

RandomNumberFactory::~RandomNumberFactory(void) {

	delete seedGenerator;
	for (std::set<RandomNumberGenerator*>::iterator p=allocatedRandomNumbers.begin(); p != allocatedRandomNumbers.end(); p++)
		delete (*p);
}

void RandomNumberFactory::deleteRandomNumberGenerator(RandomNumberGenerator* r) {

	allocatedRandomNumbers.erase( r );
	delete r;
}

RandomNumberGenerator* RandomNumberFactory::getRandomNumberGenerator(void) {

	unsigned int s1 = seedGenerator->nextUnsignedInt( UINT_MAX );
	unsigned int s2 = seedGenerator->nextUnsignedInt( UINT_MAX );
	RandomNumberGenerator *r = new RandomNumberGenerator( s1, s2 );
	allocatedRandomNumbers.insert( r );
	return r;
}

RandomNumberGenerator* RandomNumberFactory::getRandomNumberGenerator(unsigned int s) {

	RandomNumberGenerator *r = new RandomNumberGenerator( s );
	allocatedRandomNumbers.insert( r );
	return r;
}

void RandomNumberFactory::setSeed(void) {

	unsigned int x = (unsigned int)( time( 0 ) );
	I1 = x & 0xFFFF;
	I2 = x >> 16;
}

