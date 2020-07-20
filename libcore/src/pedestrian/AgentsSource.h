/**
 * \file        AgentSource.h
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
 * This class is responsible for materialising agent in a given location at a given frequency.
 * The optimal position where to put the agents is given by various algorithms, for instance
 * the Voronoi algorithm or the Mitchell Best candidate algorithm.
 *
 **/
#pragma once

#include "pedestrian/PedDistributor.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class Pedestrian;
class OutputHandler;
class StartDistribution;
class Building;

struct AgentGroupSourceInfo {
    int groupID;
    int agentParameterID;
    int numAgents;
};

class AgentsSource
{
public:
    /**
      * Constructor
      */
    AgentsSource(
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
        std::vector<int> lifeSpan);

    /**
      * Destructor
      */
    virtual ~AgentsSource() = default;

    /**
      * Generate a number of agents, based on the frequency given in the constructor.
      * No agents are generated if the maximum (_maxAgents) is reached.
      * @see _maxAgents
      * @param ped
      */
    void
    GenerateAgentsAndAddToPool(int count, Building * building, const PedDistributor & distributor);

    /**
      * Generate agents, but do not add them to the pool.
      *
      * @attention Positions and velocities of pedestrians are not set within this function.
      *
      * @see AgentsSourcesManager::ProcessAllSources()
      * @param ped: the container for the agents
      * @param count: the number of agents to generate
      * @param building: a pointer to the building object
      */
    std::vector<Pedestrian *>
    GenerateAgents(int count, Building * building, const PedDistributor & distributor) const;

    /**
      * Generate count agents and save them in the vector
      * @param ped: the container for the agents
      * @param count: the number of agents to generate
      */
    void RemoveAgentsFromPool(std::vector<Pedestrian *> & peds, size_t count);

    /**
      * Add the agents to the pool. This might be important in the case the removed agents could not
      * be placed correctly. They can be requeued using this function.
      * @param peds
      */
    void AddAgentsToPool(std::vector<Pedestrian *> & peds);

    /**
      * @return the number of agents remaining
      */
    size_t GetPoolSize() const;

    /**
      * Print relevant information
      */
    void Dump() const;

    int GetAgentsGenerated() const;
    void SetAgentsGenerated(int agentsGenerated);
    std::vector<float> GetBoundaries() const;
    void SetBoundaries(std::vector<float> bounds);
    const std::string & GetCaption() const;
    int GetFrequency() const;
    int GetGroupId() const;
    int GetID() const;
    int GetAgentID() const;
    float GetStartX() const;
    float GetStartY() const;
    double GetPlanTime() const;
    int GetMaxAgents() const;
    int GetChunkAgents() const;
    int GetRemainingAgents() const;
    void ResetRemainingAgents();
    void UpdateRemainingAgents(int remaining);
    float GetPercent() const;
    float GetRate() const;
    std::vector<int> GetLifeSpan() const;
    bool IsGreedy() const;

    void AddStartDistribution(int groupID, std::shared_ptr<StartDistribution>);
    std::shared_ptr<StartDistribution> GetStartDistribution(int groupID) const;
    int GetRoomID() const;
    int GetSubRoomID() const;

private:
    int _id        = -1;
    int _frequency = 1; /// create \var _chunkAgents every \var _frequency seconds
    int _maxAgents = 0;
    int _roomID    = -1;
    int _subRoomID = -1;
    std::vector<AgentGroupSourceInfo> _groupInfo;

    std::string _caption = "no caption";
    bool _greedy         = false;
    int _agentsGenerated = 0;
    std::vector<float> _boundaries;
    int _agentID;
    double _time;  /// planned generation time. here \var _maxAgents = 1
    float _startX; /// \var _maxAgents = 1
    float _startY; /// \var _maxAgents = 1
    std::vector<int> _lifeSpan;

    int _chunkAgents;     /// generate \var chunk_agents per \var frequency seconds
    int _remainingAgents; /// After generating \var chunk_agents \time \var
                          /// _percent per \var frequency seconds, this is the
                          /// remaining of agents still to be produced
    float _percent = 1.0; /// generate \var _percent * \var _chunkAgents
    float _rate    = 1.0;

    std::vector<Pedestrian *> _agents;
    std::map<int, std::shared_ptr<StartDistribution>> _startDistributions;
};
