#ifndef Move_H
#define Move_H

#include <string>
#include <vector>

class Move : public RbObject {

public:

	double performMove(void);
	bool accept(void);				// for statistic purposes
	bool reject(void);
	double getAcceptanceProb(void) { return nAcceptances/(double)nTries; }

private:
	int nAcceptances;
	int nTries;
	DAGNode* node;
};

#endif
