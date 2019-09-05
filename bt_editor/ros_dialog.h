#ifndef ROS_DIALOG_H
#define ROS_DIALOG_H

#include "bt_editor_base.h"

#include <QDialog>
#include <QAbstractButton>
#include <QStringList>

namespace Ui { class ROSDialog; }

class ROSDialog : public QDialog
{
    Q_OBJECT

    public:
        enum class ResourceType
        {
            Palettes = 0,
            Plugins  = 1,
        };

    public:
        explicit ROSDialog(QWidget *parent = nullptr);
        ~ROSDialog() override;

        QStringList  getSelectedPlugins() const;
        ResourceType getSelectedResource() const;

    private slots:
        void on_searchButton_pressed();
        void on_resourceComboBox_currentIndexChanged(int index);

    private:
        void setErrorMsg(const QString& _message);
        void clearErrorMsg();

    private:
        Ui::ROSDialog *ui_;
};

#endif // ROS_NODE_DIALOG_H
