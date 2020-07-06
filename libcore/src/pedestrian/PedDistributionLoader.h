//
// Created by laemmel on 31.03.16.
//
#pragma once

#include "PedDistributor.h"

class PedDistributionLoader
{
public:
    virtual bool LoadPedDistribution(
        std::vector<std::shared_ptr<StartDistribution>> & startDis,
        std::vector<std::shared_ptr<StartDistribution>> & startDisSub) = 0;
    virtual ~PedDistributionLoader(){};
};
