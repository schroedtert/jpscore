#include "SourceParser.h"

#include "general/Macros.h"

#include <Logger.h>
namespace SourceParser
{
void ParseSources(
    AgentsSourcesManager & agentsSourceManager,
    Building * building,
    const fs::path & file)
{
    LOG_INFO("Parsing sources from inifile");
    TiXmlDocument docEvent(file.string());
    if(!docEvent.LoadFile()) {
        LOG_ERROR("Cannot ini file: {}", docEvent.ErrorDesc());
        return;
    }

    TiXmlElement * xRootNode = docEvent.RootElement();
    if(!xRootNode) {
        LOG_ERROR("Ini file root element missing");
        return;
    }

    TiXmlNode * xAgents = xRootNode->FirstChild("agents");
    if(!xAgents) {
        LOG_ERROR("No agents found");
        return;
    }

    TiXmlNode * xSources = xAgents->FirstChild("agents_sources");
    if(!xSources) {
        LOG_ERROR("No sources found");
        return;
    }

    for(TiXmlElement * e = xSources->FirstChildElement("source"); e;
        e                = e->NextSiblingElement("source")) {
        auto source = ParseSource(e);
        if(source.has_value()) {
            agentsSourceManager.AddSource(source.value());
        }
    }
}

std::optional<AgentsSource> ParseSource(TiXmlElement * e)
{
    int id                 = xmltoi(e->Attribute("id"), -1);
    int frequency          = xmltoi(e->Attribute("frequency"), 1);
    int agents_max         = xmltoi(e->Attribute("agents_max"), 10);
    int group_id           = xmltoi(e->Attribute("group_id"), -1);
    std::string caption    = xmltoa(e->Attribute("caption"), "no caption");
    std::string str_greedy = xmltoa(e->Attribute("greedy"), "false");
    float percent          = xmltof(e->Attribute("percent"), 1);
    float rate             = xmltof(e->Attribute("rate"), -1);
    double time            = xmltof(e->Attribute("time"), 0);
    int agent_id           = xmltoi(e->Attribute("agent_id"), -1);
    float startx    = xmltof(e->Attribute("startX"), std::numeric_limits<float>::quiet_NaN());
    float starty    = xmltof(e->Attribute("startY"), std::numeric_limits<float>::quiet_NaN());
    bool greedy     = (str_greedy == "true") ? true : false;
    float xmin      = xmltof(e->Attribute("x_min"), std::numeric_limits<float>::lowest());
    float xmax      = xmltof(e->Attribute("x_max"), std::numeric_limits<float>::max());
    float ymin      = xmltof(e->Attribute("y_min"), std::numeric_limits<float>::lowest());
    float ymax      = xmltof(e->Attribute("y_max"), std::numeric_limits<float>::max());
    int chunkAgents = xmltof(e->Attribute("N_create"), 1);
    int timeMin     = xmltof(e->Attribute("time_min"), std::numeric_limits<int>::min());
    int timeMax     = xmltof(e->Attribute("time_max"), std::numeric_limits<int>::max());
    int roomID      = xmltof(e->Attribute("room_id"), std::numeric_limits<int>::min());
    int subroomID   = xmltof(e->Attribute("subroom_id"), std::numeric_limits<int>::max());

    std::vector<float> boundaries = {xmin, xmax, ymin, ymax};
    std::vector<int> lifeSpan     = {timeMin, timeMax};
    float SizeBB                  = 1;
    bool isBigEnough              = (abs(xmin - xmax) > SizeBB) && (abs(ymin - ymax) > SizeBB);
    if(!isBigEnough) {
        LOG_WARNING("Source {} got too small bounding box.", id);
        LOG_WARNING(
            "BB [Dx, Dy] should be such Dx > {:.2f} and Dy > {:.2f} (ignoring BB).",
            SizeBB,
            SizeBB);
        xmin = std::numeric_limits<float>::min();
        xmax = std::numeric_limits<float>::max();
        ymin = std::numeric_limits<float>::min();
        ymax = std::numeric_limits<float>::max();
    }
    if(timeMin > timeMax) {
        LOG_WARNING("Source {} given wrong life span. Assuming timeMin = timeMax.", id);
        timeMin = timeMax;
    }
    if(time > 0) {
        timeMin = std::numeric_limits<int>::min();
        timeMax = std::numeric_limits<int>::max();
        LOG_WARNING(
            "Source {}. Planned time {}. Ignoring timeMin and timeMax (in case they are "
            "specified)",
            id,
            time);
    }
    if(agent_id >= 0) {
        agents_max = 1;
        frequency  = 1;
    }
    if(percent > 1) {
        LOG_WARNING("Source {}. Passed erronuous percent <{:.2f}>. Set percent=1.", id, percent);
        percent = 1.0;
    } else if(percent < 0) {
        LOG_WARNING(
            "Source {}. Passed erronuous percent <{:.2f}>. Set percent=0 (this source is "
            "inactive)",
            id,
            percent);
        percent = 0.0;
    }
    if(rate < 0) {
        rate = frequency;
    }
    //    auto source = std::make_shared<AgentsSource>(
    //        id,
    //        caption,
    //        roomID,
    //        subroomID,
    //        agents_max,
    //        std::vector<AgentGroupSourceInfo>{AgentGroupSourceInfo{group_id, agents_max}},
    //        frequency,
    //        greedy,
    //        time,
    //        agent_id,
    //        startx,
    //        starty,
    //        percent,
    //        rate,
    //        chunkAgents,
    //        boundaries,
    //        lifeSpan);

    LOG_INFO("Source with id {} will be parsed (greedy = {}).", id, greedy);
    //    return source;

    return AgentsSource{
        id,
        caption,
        roomID,
        subroomID,
        agents_max,
        std::vector<AgentGroupSourceInfo>{AgentGroupSourceInfo{group_id, agents_max}},
        frequency,
        greedy,
        time,
        agent_id,
        startx,
        starty,
        percent,
        rate,
        chunkAgents,
        boundaries,
        lifeSpan};
}

} // namespace SourceParser
////Parse the sources
//TiXmlNode * xSources = xRootNode->FirstChild("agents_sources");
//if(xSources) {
//LOG_INFO("Loading sources");
//// ------ parse sources from inifile
//for(TiXmlElement * e = xSources->FirstChildElement("source"); e;
//e                = e->NextSiblingElement("source")) {
//startDisSources.push_back(parseSourceNode(e));
//} //for
//TiXmlNode * xFileNode = xSources->FirstChild("file");
////------- parse sources from external file
//if(xFileNode) {
//fs::path p(_configuration->GetProjectRootDir());
//std::string sourceFilename = xFileNode->FirstChild()->ValueStr();
//p /= sourceFilename;
//sourceFilename = p.string();
//LOG_INFO("Source file found <{}>", sourceFilename);
//TiXmlDocument docSource(sourceFilename);
//if(!docSource.LoadFile()) {
//LOG_ERROR("Could not parse the sources file.");
//LOG_ERROR("{}", docSource.ErrorDesc());
//return false;
//}
//TiXmlElement * xRootNodeSource = docSource.RootElement();
//if(!xRootNodeSource) {
//LOG_ERROR("Root element does not exist in source file.");
//return false;
//}
//
//if(xRootNodeSource->ValueStr() != "JPScore") {
//LOG_ERROR("Root element value in source file is not 'JPScore'.");
//return false;
//}
//TiXmlNode * xSourceF = xRootNodeSource->FirstChild("agents_sources");
//if(!xSourceF) {
//LOG_ERROR("No agents_sources tag in file not found.");
//return false;
//}
//LOG_INFO("Loading sources from file");
//TiXmlNode * xSourceNodeF = xSourceF->FirstChild("source");
//if(xSourceNodeF) {
//for(TiXmlElement * e = xSourceF->FirstChildElement("source"); e;
//e                = e->NextSiblingElement("source")) {
//startDisSources.push_back(parseSourceNode(e));
//} //for
//} else
//LOG_INFO("No source info found in source file");
//} // source file found
//}     //sources
