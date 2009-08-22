#ifndef EvoMoDeLParser_H
#define EvoMoDeLParser_H

#include <string>
#include <vector>

class RbCommand;
class EvoMoDeLParser : public RbObject {

public:
	bool parseCommand(RbCommand* command);

private:

};

#endif