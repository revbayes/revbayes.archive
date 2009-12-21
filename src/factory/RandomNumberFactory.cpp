#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include <limits>



RandomNumberFactory::RandomNumberFactory(void) {

	setSeed();
	// TODO hack, need to be cleaned up
	std::vector<unsigned int> s;
	s.push_back(I1);
	s.push_back(I2);
	seedGenerator = new RandomNumberGenerator(s);
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
    // TODO hack, need to be cleaned up
    std::vector<unsigned int> s;
    s.push_back(s1);
    s.push_back(s2);
	RandomNumberGenerator *r = new RandomNumberGenerator( s );
	allocatedRandomNumbers.insert( r );
	return r;
}

RandomNumberGenerator* RandomNumberFactory::getRandomNumberGenerator(unsigned int s1) {

    // TODO hack, need to be cleaned up
    std::vector<unsigned int> s;
    s.push_back(s1);
	RandomNumberGenerator *r = new RandomNumberGenerator( s );
	allocatedRandomNumbers.insert( r );
	return r;
}

void RandomNumberFactory::setSeed(void) {

	unsigned int x = (unsigned int)( time( 0 ) );
	I1 = x & 0xFFFF;
	I2 = x >> 16;
}

