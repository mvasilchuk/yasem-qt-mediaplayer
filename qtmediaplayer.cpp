#include "qtmediaplayer.h"

/*
#include "vlc-qt/MediaPlayer.h"
#include "vlc-qt/Common.h"
#include "vlc-qt/Instance.h"
#include "vlc-qt/Media.h"
#include "vlc-qt/Video.h"
#include "vlc-qt/WidgetVideo.h"
*/

#include "qtcustomvideowidget.h"
#include <QVariant>
#include <QPaintEngine>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGraphicsProxyWidget>
#include <QMediaPlaylist>

using namespace yasem;

QtMediaPlayer::QtMediaPlayer()
{

}

void QtMediaPlayer::parent(QWidget *parent)
{
    videoWidget->setParent(parent);
    //mediaPlayer->setParent(parent);
}

QWidget *QtMediaPlayer::parent()
{
    return videoWidget->parentWidget();
}

void QtMediaPlayer::widget(QWidget *videoWidget)
{
    this->videoWidget = qobject_cast<QtCustomVideoWidget*>(videoWidget);
    Q_ASSERT(this->videoWidget != 0);
}

QWidget *QtMediaPlayer::widget()
{
    Q_ASSERT(this->videoWidget);
    return videoWidget;
}

bool QtMediaPlayer::mediaPlay(const QString &url)
{
    STUB();
    LOG(url);

    videoWidget->setAutoFillBackground(true);

    playList = new QMediaPlaylist();
    playList->addMedia(QMediaContent(QUrl(url)));
    mediaPlayer->setPlaylist(playList);

    mediaPlayer->setVolume(0);
    return mediaContinue();
}

bool QtMediaPlayer::mediaContinue()
{
    STUB();
    mediaPlayer->play();
    state(PlayingState);
    return true;
}

bool QtMediaPlayer::mediaPause()
{
    STUB();
    mediaPlayer->pause();
    state(PausedState);
    return true;
}

bool QtMediaPlayer::mediaStop()
{
    STUB();
    mediaPlayer->stop();
    state(StoppedState);
    return true;
}

bool QtMediaPlayer::mediaReset()
{
    return true;
}

void QtMediaPlayer::show()
{
    STUB();
    //videoWidget->setVisible(true);
    widget()->show();
}

void QtMediaPlayer::hide()
{
    STUB();
    widget()->setVisible(false);
}

void QtMediaPlayer::rect(const QRect &rect)
{
    STUB();
    STUB_WITH_LIST(QStringList() << QString::number(rect.x()) << QString::number(rect.y()) << QString::number(rect.width()) << QString::number(rect.height()));
    widget()->setGeometry(rect);
}

QRect QtMediaPlayer::rect()
{
    return widget()->geometry();
}

bool QtMediaPlayer::isVisible()
{
    STUB();
    return widget()->isVisible();
}

MediaPlayingState QtMediaPlayer::state()
{
    return playerState;
}

bool QtMediaPlayer::state(MediaPlayingState state)
{
    this->playerState = state;
    switch(state)
    {
        case PlayingState:
        {
            emit mediaSignalSender.started();
            break;
        }
        case PausedState:
        {
            emit mediaSignalSender.paused(true);
            break;
        }
        case StoppedState:
        {
            //emit mediaSignalSender.stopped();
            break;
        }
    }

    return true;
}

void QtMediaPlayer::aspectRatio(ASPECT_RATIO mode)
{
    Qt::AspectRatioMode arMode = Qt::KeepAspectRatioByExpanding;
    videoWidget->setAspectRatioMode(arMode);
}

ASPECT_RATIO QtMediaPlayer::aspectRatio()
{
    return ASPECT_RATIO_AUTO;
}

void QtMediaPlayer::move(int x, int y)
{
    widget()->move(x, y);
}

void QtMediaPlayer::raise()
{
    widget()->raise();
}

PLUGIN_ERROR_CODES QtMediaPlayer::initialize()
{
    STUB();
    mediaPlayer = new QMediaPlayer();
    videoWidget = new QtCustomVideoWidget();
    mediaPlayer->setVideoOutput(videoWidget);

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &QtMediaPlayer::durationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &QtMediaPlayer::positionChanged);
    //connect(mediaPlayer, &QMediaPlayer::metaDataChanged, this, &QtMediaPlayer::metaDataChanged);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &QtMediaPlayer::statusChanged);
    connect(mediaPlayer, &QMediaPlayer::bufferStatusChanged, this, &QtMediaPlayer::bufferingProgress);
    connect(mediaPlayer, &QMediaPlayer::videoAvailableChanged, this, &QtMediaPlayer::videoAvailableChanged);
    //connect(mediaPlayer, &QMediaPlayer::error, this, &QtMediaPlayer::displayErrorMessage);
    connect(mediaPlayer, SIGNAL("QMediaPlayer::error"), SLOT("QtMediaPlayer::displayErrorMessage"));

    return PLUGIN_ERROR_NO_ERROR;
}

PLUGIN_ERROR_CODES QtMediaPlayer::deinitialize()
{
    //if(videoWidget != NULL)
    //    delete videoWidget;
    //if(mediaPlayer != NULL)
    //    delete mediaPlayer;

    return PLUGIN_ERROR_NO_ERROR;
}


void QtMediaPlayer::durationChanged(qint64 duration)
{
    STUB_WITH_PARAMS(QString("Duration changed: %1").arg(duration));
}

void QtMediaPlayer::positionChanged(qint64 position)
{
    STUB_WITH_PARAMS(QString("Position changed: %1").arg(position));
}

void QtMediaPlayer::metaDataChanged(const QString &key, const QVariant &value)
{
    STUB_WITH_PARAMS(QString("Metadata changed: %1=%2").arg(key).arg(value.toString()));
}

void QtMediaPlayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    STUB_WITH_PARAMS(QString("Status changed: %1").arg(status));

    switch(status)
    {
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
        case QMediaPlayer::LoadedMedia:
        {
            emit mediaSignalSender.started();
            break;
        }
        case QMediaPlayer::StalledMedia:
        {
            emit mediaSignalSender.paused(true);
            break;
        }

        case QMediaPlayer::NoMedia:
        //case QMediaPlayer::LoadingMedia:

        case QMediaPlayer::EndOfMedia:
        case QMediaPlayer::InvalidMedia:
        {
            //emit mediaSignalSender.stopped();
            break;
        }
        case QMediaPlayer::UnknownMediaStatus:
        default: {

                break;
        }
    }


}

void QtMediaPlayer::bufferingProgress(int bufferingProgress)
{
    STUB_WITH_PARAMS(QString("Buffering %1%...").arg(bufferingProgress));
}

void QtMediaPlayer::videoAvailableChanged(bool available)
{
    STUB_WITH_PARAMS(QString("Video available: %1").arg(QVariant(available).toString()));
}

void QtMediaPlayer::displayErrorMessage(QMediaPlayer::Error error)
{
    STUB_WITH_PARAMS(QString("Error %1: %2").arg(error).arg(mediaPlayer->errorString()));
    emit mediaSignalSender.stopped();
}

int QtMediaPlayer::audioPID()
{
    STUB();
    return 0;
}

int QtMediaPlayer::bufferLoad()
{
    return mediaPlayer->bufferStatus();
}

long QtMediaPlayer::mediaLength()
{
    return mediaPlayer->duration();
}

qint64 QtMediaPlayer::position()
{
    return mediaPlayer->position();
}

void QtMediaPlayer::audioPID(int pid)
{
    //mediaPlayer->
    STUB();
}

int QtMediaPlayer::loop()
{
    STUB();
    return 0;
}

void QtMediaPlayer::loop(int loop)
{
    STUB();
}

bool QtMediaPlayer::mute()
{
   return mediaPlayer->isMuted();
}

void QtMediaPlayer::mute(bool value)
{
    mediaPlayer->setMuted(value);
}

void QtMediaPlayer::position(qint64 pos)
{
    mediaPlayer->setPosition(pos);
}

qint64 QtMediaPlayer::duration()
{
    return mediaPlayer->duration();
}

int QtMediaPlayer::volume()
{
   return mediaPlayer->volume();
}

void QtMediaPlayer::volume(int vol)
{
    mediaPlayer->setVolume(vol);
}




