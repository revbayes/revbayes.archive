#ifndef Monitor_H
#define Monitor_H

#include <string>
#include <vector>

class Monitor : public RbObject {

public:
	bool startMonitoring(void);
	bool stopMonitoring(void);
	bool isMonitoring(void);
	bool storeValue(void);

private:
	DAGNode* node;
	std::vector<RbDataType*> value;
};

#endif