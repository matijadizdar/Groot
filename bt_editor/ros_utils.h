#ifndef ROS_UTILS_H
#define ROS_UTILS_H

#include <string>
#include <vector>
#include <utility>

#include <QStringList>

using ROSPluginInfo  = std::pair<std::string, std::string>;
using ROSPluginsInfo = std::vector<std::pair<std::string, std::string>>;

ROSPluginsInfo getPluginsInfo(const std::string& _package, const std::string& _plugin);

QStringList parsePalettePlugin(const ROSPluginInfo& _plugin);
QStringList parseTreePlugin(const ROSPluginInfo& _plugin);

#endif // NODE_UTILS_H
