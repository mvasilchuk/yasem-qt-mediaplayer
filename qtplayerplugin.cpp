#include "qtplayerplugin.h"
#include "qtmediaplayerobject.h"

using namespace yasem;

QtPlayerPlugin::QtPlayerPlugin(QObject* parent):
    Plugin(parent)
{

}

QtPlayerPlugin::~QtPlayerPlugin()
{

}

void QtPlayerPlugin::register_dependencies()
{

}

void QtPlayerPlugin::register_roles()
{
    register_role(ROLE_MEDIA, new QtMediaPlayerObject(this));
}
