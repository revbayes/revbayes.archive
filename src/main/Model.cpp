#include "Model.h"



Model::Model(void) {

}

Model::Model(std::set<DAGNode*> &sn) {

	//sinkNodes = (*sn);
}

Model::Model(Model &m) {

}

bool Model::completeModel(void) {

	return true;
}
