#include "ros_dialog.h"
#include "ui_ros_dialog.h"
#include "ros_utils.h"

#include <algorithm>

#include <QPushButton>
#include <QModelIndexList>

ROSDialog::ROSDialog(QWidget *parent) : QDialog(parent),
    ui_ (new Ui::ROSDialog)
{
    ui_->setupUi(this);

    setWindowTitle("ROS Resources");
    clearErrorMsg();
}

ROSDialog::~ROSDialog()
{
    delete ui_;
}

// TODO: add support for plugins
QStringList ROSDialog::getSelectedPlugins() const
{
    QStringList plugins;

    const auto& selected_items = ui_->resourcesList->selectedItems();

    std::transform(selected_items.cbegin(), selected_items.cend(), std::back_inserter(plugins),
                    [] (const QListWidgetItem* item) { return item->text(); });

    return plugins;
}

ROSDialog::ResourceType ROSDialog::getSelectedResource() const
{
    return static_cast<ResourceType>(ui_->resourceComboBox->currentIndex());
}

// Private
void ROSDialog::on_searchButton_pressed()
{
    const ResourceType resource_type = getSelectedResource();

    // TODO: handle errors
    if(resource_type == ResourceType::Palettes)
    {
        const ROSPluginsInfo& palettes_plugins = getPluginsInfo("behavior_tree_ros", "palette");

        if(palettes_plugins.empty()) { setErrorMsg("No palettes found"); }

        for(const auto& palette_plugin : palettes_plugins)
        {
            const QStringList& palettes = parsePalettePlugin(palette_plugin);
            ui_->resourcesList->addItems(palettes);
        }
    }
    else if(resource_type == ResourceType::Plugins)
    {
        const ROSPluginsInfo& tree_plugins = getPluginsInfo("behavior_tree_ros", "plugin");

        if(tree_plugins.empty()) { setErrorMsg("No plugins found"); }

        for(const auto& tree_plugin : tree_plugins)
        {
            const QStringList& plugins = parseTreePlugin(tree_plugin);
            ui_->resourcesList->addItems(plugins);
        }
    }
}

void ROSDialog::setErrorMsg(const QString& _message)
{
    ui_->labelWarning->setText(_message);
    ui_->labelWarning->setStyleSheet("color: rgb(204, 0, 0)");
}

void ROSDialog::clearErrorMsg()
{
    ui_->labelWarning->setText("OK");
    ui_->labelWarning->setStyleSheet("color: rgb(78, 154, 6)");
}
