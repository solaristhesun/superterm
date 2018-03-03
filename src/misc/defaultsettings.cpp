#include <QSettings>
#include <QColor>
#include <QFont>

#include "defaultsettings.h"

void DefaultSettings::writeDefaults()
{
    QSettings settings;

    if (!settings.contains("foreground"))
    {
        settings.setValue("foreground", QColor("white").name());
    }

    if (!settings.contains("background"))
    {
        settings.setValue("background", QColor("#142462").name());
    }

    if (!settings.contains("font"))
    {
#if defined(Q_OS_WIN)
        settings.setValue("font", QFont("Consolas", 11).toString());
#else
        settings.setValue("font", QFont("Liberation Mono", 11).toString());
#endif
    }
}

// EOF <stefan@scheler.com>
