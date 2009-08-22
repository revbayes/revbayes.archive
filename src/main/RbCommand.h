#ifndef RbCommand_H
#define RbCommand_H

#include <string>
#include <vector>

class RbCommand : public RbObject {

public:

private:
	std::string name;
	std::vector<Argument*>;
};

#endif