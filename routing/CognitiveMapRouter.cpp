/**
 * @file   CognitiveMapRouter.cpp
 * @author David Haensel (d.haensel@fz-juelich.de)
 * @date   February, 2014
 * @brief  Routing Engine for Cognitive Map
 *
 */

#include "CognitiveMapRouter.h"
#include "Router.h"

#include "cognitive_map/CognitiveMapStorage.h"
#include "cognitive_map/CognitiveMap.h"
#include "cognitive_map/NavigationGraph.h"
#include "cognitive_map/sensor/SensorManager.h"

#include "../geometry/SubRoom.h"
#include "../geometry/NavLine.h"
#include "../geometry/Building.h"

#include "../pedestrian/Pedestrian.h"
#include "../tinyxml/tinyxml.h"

CognitiveMapRouter::CognitiveMapRouter()
{
}

CognitiveMapRouter::~CognitiveMapRouter()
{
    delete cm_storage;

}

int CognitiveMapRouter::FindExit(Pedestrian * p)
{
    //check for former goal.
    if((*cm_storage)[p]->HadNoDestination()) {
        sensor_manager->execute(p, SensorManager::INIT);
    }

    //Check if the Pedestrian already has a Dest. or changed subroom and needs a new one.
    if((*cm_storage)[p]->ChangedSubRoom()) {
        //execute periodical sensors
        sensor_manager->execute(p, SensorManager::CHANGED_ROOM);

        int status = FindDestination(p);

        (*cm_storage)[p]->UpdateSubRoom();

        return status;

    }
    return 1;
}

int CognitiveMapRouter::FindDestination(Pedestrian * p)
{
        //check if there is a way to the outside the pedestrian knows (in the cognitive map)
        const GraphEdge * destination = NULL;
        destination = (*cm_storage)[p]->GetDestination();
        if(destination == NULL) {
            //no destination was found, now we could start the discovery!
            //1. run the no_way sensors for room discovery.
            sensor_manager->execute(p, SensorManager::NO_WAY);

            //check if this was enough for finding a global path to the exit
            destination = (*cm_storage)[p]->GetDestination();

            if(destination == NULL) {
                //we still do not have a way. lets take the "best" local edge
                //for this we don't calculate the cost to exit but calculte the cost for the edges at the actual vertex.
                destination = (*cm_storage)[p]->GetLocalDestination();
            }
        }

        //if we still could not found any destination we are lost! Pedestrian will be deleted
        //no destination should just appear in bug case or closed rooms.
        if(destination == NULL) {
            Log->Write("ERROR: \t One Pedestrian (ID: %i) was not able to find any destination", p->GetID());
            return -1;
        }

        (*cm_storage)[p]->AddDestination(destination);

        p->SetExitLine(destination->GetCrossing());
        // p->SetExitIndex(destination->GetCrossing()->GetUniqueID());
        return 1;
}



void CognitiveMapRouter::Init(Building * b)
{
    Log->Write("INFO:\tInit the Cognitive Map  Router Engine");
    building = b;

    //Init Cognitive Map Storage
    cm_storage = new CognitiveMapStorage(building);
    Log->Write("INFO:\tInitialized CognitiveMapStorage");
    //Init Sensor Manager
    sensor_manager = SensorManager::InitWithAllSensors(b, cm_storage);
    Log->Write("INFO:\tInitialized SensorManager");
}
