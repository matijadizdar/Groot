#include "ros_utils.h"

#include <ros/ros.h>
#include <ros/package.h>
#include <boost/filesystem.hpp>
#include <tinyxml2.h>

#include <QFile>

ROSPluginsInfo getPluginsInfo(const std::string& _package, const std::string& _plugin)
{
    ROSPluginsInfo exported_plugins;
    ros::package::getPlugins(_package, _plugin, exported_plugins);

    return exported_plugins;
}

QStringList parsePalettePlugin(const ROSPluginInfo& _plugin)
{
    QStringList exported_palettes;

    using namespace tinyxml2;
    XMLDocument template_description;

    template_description.LoadFile(_plugin.second.c_str());

    if(template_description.Error())
    {
        std::string error_msg;
        #ifdef MELODIC
        error_msg = std::string { template_description.ErrorName() } + ": " + std::string { template_description.ErrorStr() };
        #endif

        #ifndef MELODIC
        error_msg =  std::string { template_description.ErrorName() } + ": " + std::string { template_description.GetErrorStr1() };
        #endif 
        throw std::runtime_error { error_msg };
    }

    XMLElement* root_entry = template_description.RootElement();
    if (!root_entry)
    {
        throw std::runtime_error { "No root element was found in XML file" };
    }

    XMLElement* template_entry = root_entry->FirstChildElement("palette");

    while(template_entry)
    {
        std::string template_lib = template_entry->Attribute("path");
        if (template_lib.empty())
        {
            throw std::runtime_error { std::string("Missing path attribute in palette description in file ")
                                        + _plugin.second };
        }

        template_entry = template_entry->NextSiblingElement("palette");

        const std::string& xml_path = _plugin.second;

        std::string package_path  = xml_path.substr(0, xml_path.find("palette"));
        std::string lib_full_path = package_path + template_lib;

        QString fileName = QString::fromStdString(template_lib);
        QString filePath = QString::fromStdString(lib_full_path);
        QFile file(filePath);

        if (!file.exists())
        {
            throw std::runtime_error { std::string("Plugin containted non-existing file: ")
                                        + lib_full_path.c_str() };

        }

        exported_palettes.push_back(filePath);
    }

    return exported_palettes;
}

QStringList parseTreePlugin(const ROSPluginInfo& _plugin)
{
    QStringList exported_plugins;

    using namespace tinyxml2;
    XMLDocument plugin_description;

    plugin_description.LoadFile(_plugin.second.c_str());

    if(plugin_description.Error())
    {
        std::string error_msg;
        #ifdef MELODIC
        error_msg = std::string { plugin_description.ErrorName() } + ": " + std::string { plugin_description.ErrorStr() };
        #endif

        #ifndef MELODIC
        error_msg = std::string { plugin_description.ErrorName() } + ": " + std::string { plugin_description.GetErrorStr1() };
        #endif 
        throw std::runtime_error { error_msg };
    }

    XMLElement* root_entry = plugin_description.RootElement(); 

    if(!root_entry)
    {
        throw std::runtime_error { "No root element was found in XML file" };
    }

    XMLElement* plugin_entry = root_entry->FirstChildElement("plugin");

    // This loop abort the parsing on first error. This could be a problem if there are multiple plugins
    // defined in the same file.
    while(plugin_entry)
    {
        std::string plugin_lib = plugin_entry->Attribute("path");

        if(plugin_lib.empty())
        {
            throw std::runtime_error { "Missing path attribute in plugin element" };
        }

        std::string devel_path;
        const std::string& xml_path = _plugin.second;

        // Is there a better way to do this?
        // Check if the path contains a src folder (then we assume is a local workspace)
        // or if it contains a share folder (then we assume we are in the system path)
        if(xml_path.find("/src/") != std::string::npos)
        {
            devel_path = xml_path.substr(0, xml_path.find("/src/")) + "/devel/";
        }
        else if(xml_path.find("/share/") != std::string::npos)
        {
            devel_path = xml_path.substr(0, xml_path.find("/share/")) + "/";
        }

        if(devel_path.empty())
        {
            throw std::runtime_error { "Cannot find devel path to plugin" };
        }

        std::string lib_full_path = devel_path + plugin_lib + ".so";

        QString filePath = QString::fromStdString(lib_full_path);

        QFile file(filePath);

        if (!file.exists())
        {
            throw std::runtime_error { std::string("Plugin containted non-existing file: ")
                                        + lib_full_path.c_str() };

        }

        exported_plugins.push_back(filePath);

        plugin_entry = plugin_entry->NextSiblingElement("plugin");
    }

    return exported_plugins;
}

