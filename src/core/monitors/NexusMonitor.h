/*
 * NexusMonitor.h
 *
 *  Created on: May 7, 2019
 *      Author: joellebs
 */

#ifndef SRC_CORE_MONITORS_NEXUSMONITOR_H_
#define SRC_CORE_MONITORS_NEXUSMONITOR_H_

#include "AbstractFileMonitor.h"

#include <TypedDagNode.h>

namespace RevBayesCore {

class NexusMonitor: public AbstractFileMonitor {
public:
    NexusMonitor(TypedDagNode<Tree> *t, const std::vector<DagNode*> &n, bool np, unsigned long g, const std::string &fname,
                 bool ap = false, bool taxa = true);
    NexusMonitor* clone(void) const;

    virtual void printHeader();
    virtual void monitor(unsigned long gen);
    virtual void closeStream();

protected:
    bool isNodeParameter;
    bool writeTaxa;
    TypedDagNode<Tree>* tree;
    std::vector<DagNode*> nodeVariables;
};

} /* namespace RevBayesCore */

#endif /* SRC_CORE_MONITORS_NEXUSMONITOR_H_ */
