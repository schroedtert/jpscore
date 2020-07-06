/**
 * \file        AgentSource.cpp
 * \date        Apr 04, 2015
 * \version     v0.7
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 * This class is responsible for materialising agent in a given location at a given frequency up to a maximum number.
 * The optimal position where to put the agents is given by various algorithms, for instance
 * the Voronoi algorithm or the Mitchell Best candidate algorithm.
 *
 **/
#include "AgentsSource.h"

#include "Pedestrian.h"

#include <Logger.h>
#include <utility>

AgentsSource::AgentsSource(
    int id,
    std::string caption,
    int roomID,
    int subRoomID,
    int maxAgents,
    std::vector<AgentGroupSourceInfo> groupInfo,
    int frequency,
    bool greedy,
    double time,
    int agentID,
    float startX,
    float startY,
    float percent,
    float rate,
    int chunkAgents,
    std::vector<float> boundaries,
    std::vector<int> lifeSpan) :
    _id(id),
    _roomID(roomID),
    _subRoomID(subRoomID),
    _frequency(frequency),
    _maxAgents(maxAgents),
    _groupInfo(groupInfo),
    _caption(std::move(caption)),
    _greedy(greedy),
    _agentID(agentID),
    _time(time),
    _startX(startX),
    _startY(startY),
    _chunkAgents(chunkAgents),
    _percent(percent),
    _rate(rate)
{
    _remainingAgents = _chunkAgents;
    _agentsGenerated = 0;
    _boundaries      = std::move(boundaries);
    _lifeSpan        = std::move(lifeSpan);
}
int AgentsSource::GetRoomID() const
{
    return _roomID;
}
int AgentsSource::GetSubRoomID() const
{
    return _subRoomID;
}

void AgentsSource::GenerateAgentsAndAddToPool(int count, Building * building)
{
    std::vector<Pedestrian *> peds = GenerateAgents(count, building);

    _agents.insert(_agents.begin(), peds.begin(), peds.end());
    _agentsGenerated += count;
}

void AgentsSource::RemoveAgentsFromPool(std::vector<Pedestrian *> & ped, size_t count)
{
    if(_agents.size() >= count) {
        ped.insert(ped.begin(), _agents.begin(), _agents.begin() + count);
        _agents.erase(_agents.begin(), _agents.begin() + count);
    } else {
        ped.insert(ped.begin(), _agents.begin(), _agents.end());
        _agents.clear();
    }
}

void AgentsSource::AddAgentsToPool(std::vector<Pedestrian *> & peds)
{
    _agents.insert(_agents.begin(), peds.begin(), peds.end());
}

bool AgentsSource::IsGreedy() const
{
    return _greedy;
}
size_t AgentsSource::GetPoolSize() const
{
    return _agents.size();
}

int AgentsSource::GetAgentsGenerated() const
{
    return _agentsGenerated;
}

void AgentsSource::SetAgentsGenerated(int agentsGenerated)
{
    _agentsGenerated = agentsGenerated;
}

std::vector<float> AgentsSource::GetBoundaries() const
{
    return _boundaries;
}

void AgentsSource::SetBoundaries(std::vector<float> bounds)
{
    _boundaries = std::move(bounds);
}

const std::string & AgentsSource::GetCaption() const
{
    return _caption;
}

int AgentsSource::GetFrequency() const
{
    return _frequency;
}

int AgentsSource::GetGroupId() const
{
    //TODO
    return -1;
}

int AgentsSource::GetID() const
{
    return _id;
}

int AgentsSource::GetAgentID() const
{
    return _agentID;
}

double AgentsSource::GetPlanTime() const
{
    return _time;
}


int AgentsSource::GetMaxAgents() const
{
    return _maxAgents;
}

int AgentsSource::GetChunkAgents() const
{
    return _chunkAgents;
}

int AgentsSource::GetRemainingAgents() const
{
    return _remainingAgents;
}

void AgentsSource::ResetRemainingAgents()
{
    _remainingAgents = _chunkAgents;
}

void AgentsSource::UpdateRemainingAgents(int remaining)
{
    _remainingAgents = (remaining < _remainingAgents) ? _remainingAgents - remaining : 0;
}
float AgentsSource::GetPercent() const
{
    return _percent;
}
float AgentsSource::GetRate() const
{
    return _rate;
}
std::vector<int> AgentsSource::GetLifeSpan() const
{
    return _lifeSpan;
}

float AgentsSource::GetStartX() const
{
    return _startX;
}

float AgentsSource::GetStartY() const
{
    return _startY;
}

void AgentsSource::AddStartDistribution(
    int groupID,
    std::shared_ptr<StartDistribution> startDistribution)
{
    _startDistributions.emplace(groupID, std::move(startDistribution));
}

std::shared_ptr<StartDistribution> AgentsSource::GetStartDistribution(int groupID) const
{
    return _startDistributions.at(groupID);
}

std::vector<Pedestrian *> AgentsSource::GenerateAgents(int count, Building * building) const
{
    std::vector<Point> emptyPositions;
    std::vector<Pedestrian *> peds;
    // TODO: this vector is empty with purpose!
    //       1. Source-Pedestrians are created without setting their positions.
    //       2. In a second step (in AgentsSourcesManager::ProcessAllSources())
    //          the positions will be calculated and initializes.
    // TODO: We should reverse the order of points 1 and 2.
    int pid = (this->GetAgentID() >= 0) ?
                  this->GetAgentID() :
                  Pedestrian::GetAgentsCreated() + building->GetAllPedestrians().size();

    for(auto groupInfo : _groupInfo) {
        //        for (int i=0; i<groupInfo.numAgents; ++i){
        //            auto * ped =
        //        }
    }
    //    for(int i = 0; i < count; i++) {
    //        if(GetStartDistribution()) {
    //            auto * ped = GetStartDistribution()->GenerateAgent(building, &pid, emptyPositions);
    //            peds.push_back(ped);
    //        } else {
    //            std::string message =
    //                "Source: StartDistribution is null!\n"
    //                "This happens when group_id in <source> does not much any group_id in <agents>. "
    //                "Check your inifile.";
    //            throw std::runtime_error(message);
    //        }
    //    }
    return peds;
}


void AgentsSource::Dump() const
{
    //    auto tmpB = this->GetBoundaries();
    //    auto tmpL = this->GetLifeSpan();
    //    LOG_DEBUG(
    //        "Dumping Source: Caption={} SourceID={:d}, GroupID={:d}, Frequency={:d}, AgentsMax={:d}, "
    //        "AgentsPool={:d}, AgentID={:d}, Time={:.2f}, Pos=({:.2f},{:.2f}), Percent={:.2f}, "
    //        "Rate={:.2f}, N_create={:d}, Boundaries=(XAxis=({:.4f}, {:.4f}), YAxis=({:.4f},{:.4f})), "
    //        "LifeSpan=({:d},{:d})",
    //        this->GetCaption(),
    //        _id,
    //        _groupIDs,
    //        _frequency,
    //        _maxAgents,
    //        _agents.size(),
    //        this->GetAgentID(),
    //        this->GetPlanTime(),
    //        this->GetStartX(),
    //        this->GetStartY(),
    //        this->GetPercent(),
    //        this->GetRate(),
    //        this->GetChunkAgents(),
    //        tmpB[0],
    //        tmpB[1],
    //        tmpB[2],
    //        tmpB[3],
    //        tmpL[0],
    //        tmpL[1]);
}
