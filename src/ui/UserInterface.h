#ifndef UserInterface_H
#define UserInterface_H

class UserInterface : public RbObject {

public:
	bool startInteraction(void);

private:
	REvBayesMain* main;
};

#endif