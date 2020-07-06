#pragma once

#include "general/Filesystem.h"
#include "pedestrian/AgentsSource.h"
#include "pedestrian/AgentsSourcesManager.h"

#include <optional>
#include <tinyxml.h>

namespace SourceParser
{
/**
 *
 * @param eventManager
 * @param building
 * @param file
 */
void ParseSources(AgentsSourcesManager & eventManager, Building * building, const fs::path & file);

std::optional<AgentsSource> ParseSource(TiXmlElement * node);
}; // namespace SourceParser
