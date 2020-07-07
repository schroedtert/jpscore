//
// Created by laemmel on 31.03.16.
//
#pragma once

#include "PedDistributor.h"

class PedDistributionLoader
{
public:
    virtual bool LoadPedDistribution(
        std::vector<StartDistribution> & startDis,
        std::vector<StartDistribution> & startDisSub) = 0;
    virtual ~PedDistributionLoader(){};
};
