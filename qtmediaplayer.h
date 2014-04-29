#ifndef QTMEDIAPLAYER_H
#define QTMEDIAPLAYER_H

#include "macros.h"
#include "mediaplayerplugin.h"
#include "plugin.h"
#include "qtmediaplayer_global.h"
#include "qtcustomvideowidget.h"

#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace yasem
{

class QTMEDIAPLAYERSHARED_EXPORT QtMediaPlayer: public QObject, public virtual Plugin, public MediaPlayerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.mvas.yasem.QtMediaPlayer/1.0" FILE "metadata.json")
    Q_INTERFACES(yasem::Plugin yasem::MediaPlayerPlugin)

    Q_CLASSINFO("author", "Maxim Vasilchuk")
    Q_CLASSINFO("description", "Qt Mediaplayer plugin for YASEM")
public:
    QtMediaPlayer();

    // MediaPlayerPlugin interface
public:

    virtual void parent(QWidget* parent);
    virtual QWidget* parent();

    virtual void widget(QWidget* videoWidget);
    virtual QWidget* widget();
    bool mediaPlay(const QString &url);
    bool mediaContinue();
    bool mediaPause();
    bool mediaStop();
    bool mediaReset();

    virtual void show();
    virtual void hide();
    virtual void rect(const QRect &rect);
    virtual QRect rect();
    virtual bool isVisible();

    MediaPlayingState state();
    bool state(MediaPlayingState state);

    virtual void aspectRatio(ASPECT_RATIO mode);
    virtual ASPECT_RATIO aspectRatio();

    virtual void move(int x, int y);
    virtual void raise();

    // Plugin interface
public:
    PLUGIN_ERROR_CODES initialize();
    PLUGIN_ERROR_CODES deinitialize();

protected:
    QMediaPlaylist *playList;
    QtCustomVideoWidget* videoWidget;
    QMediaPlayer *mediaPlayer;
    MediaPlayingState playerState;

public slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void metaDataChanged(const QString &key, const QVariant &value);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int bufferingProgress);
    void videoAvailableChanged(bool available);
    void displayErrorMessage(QMediaPlayer::Error error);

    // MediaPlayerPlugin interface
public:
    void setOverlayWidget(QWidget *widget);

    // MediaPlayerPlugin interface
public:
    int audioPID();
    int bufferLoad();
    long mediaLength();
    qint64 position();
    void position(qint64 pos);

    qint64 duration();

    void audioPID(int pid);
    int loop();
    void loop(int loop);
    bool mute();
    void mute(bool value);

    int volume();
    void volume(int vol);

};

}

#endif // QTMEDIAPLAYER_H
