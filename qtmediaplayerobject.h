#ifndef QTMEDIAPLAYER_H
#define QTMEDIAPLAYER_H

#include "macros.h"
#include "mediaplayerpluginobject.h"
#include "qtcustomvideowidget.h"

#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace yasem
{

class QtMediaPlayerObject: public MediaPlayerPluginObject
{
    Q_OBJECT
public:
    explicit QtMediaPlayerObject(Plugin* plugin);
    virtual ~QtMediaPlayerObject();

    // MediaPlayerPlugin interface
public:

    virtual void parent(QWidget* parent);
    virtual QWidget* parent();

    virtual void widget(QWidget* m_video_widget);
    virtual QWidget* widget() const;
    bool mediaPlay(const QString &url);
    bool mediaContinue();
    bool mediaPause();
    bool mediaStop();
    bool mediaReset();

    virtual void show();
    virtual void hide();
    virtual void rect(const QRect &rect);
    virtual QRect rect() const;
    virtual bool isVisible();

    MediaPlayingState state();
    bool state(MediaPlayingState state);

    virtual void move(int x, int y);
    virtual void raise();

protected:
    QMediaPlaylist *playList;
    QGraphicsVideoItem* m_video_widget;
    QMediaPlayer *mediaPlayer;
    MediaPlayingState playerState;
    QGraphicsView *m_graphics_view;

public slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void metaDataChanged(const QString &key, const QVariant &value);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int bufferingProgress);
    void videoAvailableChanged(bool available);
    void displayErrorMessage(QMediaPlayer::Error error);

    virtual void setViewport(const QRect &requestedRect);
    virtual void setViewport(const QRect &containerRect, const qreal containerScale, const QRect &requestedRect);

    // MediaPlayerPlugin interface
public:
    void setOverlayWidget(QWidget *widget);


    // AbstractPluginObject interface
public:
    PluginObjectResult init();
    PluginObjectResult deinit();

    // MediaPlayerPluginObject interface
public:
    int getAudioPID() const;
    void setAudioPID(int pid);
    int getLoop() const;
    void setLoop(int loop);
    bool isMute() const;
    void setMute(bool value);
    int bufferLoad() const;
    qint64 getPosition() const;
    void setPosition(qint64 pos);
    qint64 getDuration() const;
    int getVolume() const;
    void setVolume(int vol);
    bool isVisible() const;
    void setAspectRatio(AspectRatio mode);
    AspectRatio getAspectRatio();
    QList<AudioLangInfo> getAudioLanguages();
    void setAudioLanguage(int index);
    void setBrightness(int brightness);
    int getBrightness() const;
    void setContrast(int contrast);
    int getContrast() const;
    void setSaturation(int saturation);
    int getSaturation() const;
    MediaMetadata getMediaMetadata();
    QPoint getWidgetPos() const;
    QPixmap &render();
};

}

#endif // QTMEDIAPLAYER_H
