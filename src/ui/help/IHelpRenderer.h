/* 
 * File:   IHelpRenderer.h
 * Author: johadunf
 *
 * Created on May 12, 2014, 2:40 PM
 */

#ifndef IHELPRENDERER_H
#define	IHELPRENDERER_H

#include <string>
#include "RbHelpEntry.h"

class IHelpRenderer{
public:
    virtual std::string renderTypeHelp(TypeHelpEntry typeHelp) = 0;
    virtual std::string renderFunctionHelp( FunctionHelpEntry functionHelp) = 0;
};

#endif	/* IHELPRENDERER_H */

