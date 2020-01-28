/**
 * Copyright (c) 2020 Forschungszentrum Jülich GmbH. All rights reserved.
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
 **/
#pragma once

#include "general/Logger.h"

#include <string>
#include <tinyxml.h>

namespace XmlUtil
{
int StringToInt(const std::string & input)
{
    std::size_t pos;
    int ret = std::stoi(input, &pos);
    if(pos == input.length()) {
        return ret;
    } else {
        throw std::invalid_argument("");
    }
}

double StringToDouble(const std::string & input)
{
    std::size_t pos;

    double ret = std::stod(input, &pos);
    if(pos == input.length()) {
        return ret;
    } else {
        throw std::invalid_argument("");
    }
}

bool StringToBool(const std::string & input)
{
    std::string inputLower = input;
    std::transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
    if(inputLower == "true") {
        return true;
    } else if(inputLower == "false") {
        return false;
    } else {
        throw std::invalid_argument("");
    }
}
} // namespace XmlUtil


template <typename T>
class Validator
{
public:
    static T GetValue(const TiXmlNode & node);
};

template <>
class Validator<int>
{
public:
    static int GetValue(const TiXmlNode & node)
    {
        try {
            return XmlUtil::StringToInt(node.Value());
        } catch(const std::invalid_argument & ia) {
            auto message{fmt::format(
                FMT_STRING("In <{}> expected integer input but received: {}"),
                node.Parent()->Value(),
                node.Value())};
            throw std::invalid_argument(message);
        }

        catch(const std::out_of_range & oor) {
            auto message{fmt::format(
                FMT_STRING("In <{}> value not in integer range: {}"),
                node.Parent()->Value(),
                node.Value())};
            throw std::out_of_range(message);
        }
    }
};

template <>
class Validator<double>
{
public:
    static double GetValue(const TiXmlNode & node)
    {
        try {
            return XmlUtil::StringToDouble(node.Value());
        } catch(const std::invalid_argument & ia) {
            auto message{fmt::format(
                FMT_STRING("In <{}> expected double input but received: {}"),
                node.Parent()->Value(),
                node.Value())};
            throw std::invalid_argument(message);
        }

        catch(const std::out_of_range & oor) {
            auto message{fmt::format(
                FMT_STRING("In <{}> value not in double range: {}"),
                node.Parent()->Value(),
                node.Value())};
            throw std::out_of_range(message);
        }
    }
};

template <>
class Validator<bool>
{
public:
    static bool GetValue(const TiXmlNode & node)
    {
        try {
            return XmlUtil::StringToBool(node.Value());
        } catch(const std::invalid_argument & ia) {
            auto message{fmt::format(
                FMT_STRING("In <{}> expected 'true' or 'false' as input but received: {}"),
                node.Parent()->Value(),
                node.Value())};
            throw std::invalid_argument(message);
        }
    }
};
