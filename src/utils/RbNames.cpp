/*
 * RbNames.cpp
 *
 *  Created on: 30 nov 2009
 *      Author: Sebastian
 */

#include "RbNames.h"
#include <string>

    const std::string RbNames::Beta::name                   = "distBeta";
    const std::string RbNames::Beta::alpha                  = "alpha";
    const std::string RbNames::Beta::beta                   = "beta";

    const std::string RbNames::Double::name                 = "double";

    const std::string RbNames::Distribution::name           = "distribution";

    const std::string RbNames::Exponential::name            = "distExponential";
    const std::string RbNames::Exponential::lambda          = "rate";

    const std::string RbNames::MCMC::name                   = "MCMC";

    const std::string RbNames::Model::name                  = "model";

    const std::string RbNames::Normal::name                 = "distNormal";
    const std::string RbNames::Normal::mu                   = "mu";
    const std::string RbNames::Normal::sigma                = "sigma";

    const std::string RbNames::Uniform01::name              = "distUniform01";

    const std::string RbNames::Uniform::name                = "distUniform";
    const std::string RbNames::Uniform::lower               = "lower";
    const std::string RbNames::Uniform::upper               = "upper";

    const std::string RbNames::MoveScale::name              = "moveScale";

    std::string RbNames::MoveSlidingWindow::name      = "moveSlidingWindow";

