#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>

namespace gui
{

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    SettingsWidget(QWidget* parent = 0);
    ~SettingsWidget();
};

} // namespace gui

#endif // SETTINGS_WIDGET_H
