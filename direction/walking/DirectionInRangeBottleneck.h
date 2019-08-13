//
// Created by Tobias Schrödter on 2019-04-14.
//
#pragma  once

#include "DirectionStrategy.h"

class DirectionInRangeBottleneck : public DirectionStrategy {
public:
    virtual Point GetTarget(Room* room, Pedestrian* ped) const;
};