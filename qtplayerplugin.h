#ifndef QTPLAYERPLUGINOBJECT_H
#define QTPLAYERPLUGINOBJECT_H

#include "plugin.h"
#include "qtmediaplayer_global.h"

namespace yasem {

class QTMEDIAPLAYERSHARED_EXPORT QtPlayerPlugin: public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.mvas.yasem.QtPlayerPlugin/1.0" FILE "metadata.json")
    Q_INTERFACES(yasem::Plugin)

    Q_CLASSINFO("author", "Maxim Vasilchuk")
    Q_CLASSINFO("description", "Qt media player for YASEM")
public:
    QtPlayerPlugin(QObject* parent = NULL);
    virtual ~QtPlayerPlugin();
    void register_dependencies();
    void register_roles();
};

}

#endif // QTPLAYERPLUGINOBJECT_H
