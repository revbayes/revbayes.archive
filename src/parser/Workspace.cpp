#include "DAGNode.h"
#include "WorkSpace.h"



WorkSpace::~WorkSpace(void) {

}

void WorkSpace::addDagNode(DAGNode *dagPtr) {

	dagNodes.insert( dagPtr );
	symbolTable.insert( std::make_pair(dagPtr->getName(), dagPtr) );
}

DAGNode* WorkSpace::retrieveDagNode(std::string &q) {

	std::map<std::string, DAGNode*>::iterator it = symbolTable.find( q );
	if ( it != symbolTable.end() )
		return it->second;
	return NULL;
}

