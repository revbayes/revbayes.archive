/*
 * RbParameter.h
 *
 *  Created on: 17 sep 2009
 *      Author: Sebastian
 */

#ifndef RBPARAMETER_H_
#define RBPARAMETER_H_

class RbParameter {
public:
	RbParameter();
	virtual ~RbParameter();

	RbDataType* getValue();
	DAGNode* getDAGNode();

private:
	RbDataType* parameter;                       // the parameter reference
	DAGNode* node;                               // the dag node to which this parameter is associated
};

#endif /* RBPARAMETER_H_ */
