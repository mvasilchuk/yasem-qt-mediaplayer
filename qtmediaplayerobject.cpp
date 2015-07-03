#include "qtmediaplayerobject.h"

#include "qtcustomvideowidget.h"
#include <QVariant>
#include <QPaintEngine>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGraphicsProxyWidget>
#include <QMediaPlaylist>

using namespace yasem;

QtMediaPlayerObject::QtMediaPlayerObject(SDK::Plugin* plugin):
    SDK::MediaPlayerPluginObject(plugin),
    mediaPlayer(NULL)
{
    m_support_opengl = false;
}

QtMediaPlayerObject::~QtMediaPlayerObject()
{
    deinit();
    if(mediaPlayer)
    {
        mediaPlayer->setParent(NULL);
        mediaPlayer->deleteLater();
    }
}

void QtMediaPlayerObject::parent(QWidget *parent)
{
    m_video_widget->setParent(parent);
    //mediaPlayer->setParent(parent);
}

QWidget *QtMediaPlayerObject::parent()
{
    return m_graphics_view->parentWidget();
}

void QtMediaPlayerObject::widget(QWidget *videoWidget)
{
    //this->m_video_widget = qobject_cast<QtCustomVideoWidget*>(videoWidget);
    //Q_ASSERT(this->m_video_widget != 0);
}

QWidget *QtMediaPlayerObject::widget() const
{
    //Q_ASSERT(this->m_video_widget);
    return m_graphics_view;
}

bool QtMediaPlayerObject::mediaPlay(const QString &url)
{
    STUB();
    LOG() << url;

    QString newUrl(url);
    if(newUrl.startsWith("/"))
        newUrl = QString("file://").append(newUrl);

    m_graphics_view->setAutoFillBackground(true);

    playList = new QMediaPlaylist();
    playList->addMedia(QMediaContent(QUrl(newUrl)));
    mediaPlayer->setPlaylist(playList);
    mediaPlayer->setVolume(50);

    return mediaContinue();
}

bool QtMediaPlayerObject::mediaContinue()
{
    STUB();
    mediaPlayer->play();
    state(SDK::PlayingState);
    return true;
}

bool QtMediaPlayerObject::mediaPause()
{
    STUB();
    mediaPlayer->pause();
    state(SDK::PausedState);
    return true;
}

bool QtMediaPlayerObject::mediaStop()
{
    STUB();
    mediaPlayer->stop();
    state(SDK::StoppedState);
    return true;
}

bool QtMediaPlayerObject::mediaReset()
{
    return true;
}

void QtMediaPlayerObject::show()
{
    STUB();
    //videoWidget->setVisible(true);
    widget()->show();
}

void QtMediaPlayerObject::hide()
{
    STUB();
    widget()->setVisible(false);
}

void QtMediaPlayerObject::rect(const QRect &rect)
{
    STUB() << rect;
    m_graphics_view->setGeometry(rect);
    m_graphics_view->setSceneRect(0, 0, rect.width(), rect.height());
    m_video_widget->setSize(QSize(rect.width(), rect.height()));
}

QRect QtMediaPlayerObject::rect() const
{
    return m_graphics_view->geometry();
}

bool QtMediaPlayerObject::isVisible()
{
    STUB();
    return widget()->isVisible();
}

SDK::MediaPlayingState QtMediaPlayerObject::state()
{
    return playerState;
}

bool QtMediaPlayerObject::state(SDK::MediaPlayingState state)
{
    this->playerState = state;
    switch(state)
    {
        case SDK::PlayingState:
        {
            emit started();
            break;
        }
        case SDK::PausedState:
        {
            emit paused(true);
            break;
        }
        case SDK::StoppedState:
        {
            //emit mediaSignalSender.stopped();
            break;
        }
    }

    return true;
}


void QtMediaPlayerObject::move(int x, int y)
{
    widget()->move(x, y);
}

void QtMediaPlayerObject::raise()
{
    widget()->raise();
}

void QtMediaPlayerObject::durationChanged(qint64 duration)
{
    STUB() << "Duration changed:" << duration;
}

void QtMediaPlayerObject::positionChanged(qint64 position)
{
    STUB() << "Position changed:" << position;
}

void QtMediaPlayerObject::metaDataChanged(const QString &key, const QVariant &value)
{
    STUB() << "Metadata changed:" << key << "=" << value;
}

void QtMediaPlayerObject::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    STUB() << "Status changed:" << status;

    switch(status)
    {
        case QMediaPlayer::BufferingMedia:
        {
            emit statusChanged(SDK::BufferingMedia);
            break;
        }
        case QMediaPlayer::BufferedMedia:
        {
            emit statusChanged(SDK::BufferedMedia);
            break;
        }
        case QMediaPlayer::LoadedMedia:
        {
            emit started();
            break;
        }
        case QMediaPlayer::StalledMedia:
        {
            emit paused(true);
            break;
        }

        case QMediaPlayer::NoMedia: {
            emit statusChanged(SDK::NoMedia);
            break;
        }
        case QMediaPlayer::EndOfMedia:
        {
            emit statusChanged(SDK::EndOfMedia);
            break;
        }
        case QMediaPlayer::InvalidMedia:
        {
            emit statusChanged(SDK::InvalidMedia);
            break;
        }
        case QMediaPlayer::UnknownMediaStatus:
        default: {
            break;
        }
    }


}

void QtMediaPlayerObject::bufferingProgress(int bufferingProgress)
{
    STUB() << "Buffering" << bufferingProgress << "%...";
}

void QtMediaPlayerObject::videoAvailableChanged(bool available)
{
    STUB() << "Video available:" << available;
}

void QtMediaPlayerObject::displayErrorMessage(QMediaPlayer::Error error)
{
    STUB() << "Error" << error << ":" << mediaPlayer->errorString();
    emit stopped();
}

void QtMediaPlayerObject::setViewport(const QRect &requestedRect)
{
    MediaPlayerPluginObject::setViewport(requestedRect);
    m_graphics_view->scene()->update(m_graphics_view->rect());
}

void QtMediaPlayerObject::setViewport(const QRect &containerRect, const qreal containerScale, const QRect &requestedRect)
{
    MediaPlayerPluginObject::setViewport(containerRect, containerScale, requestedRect);
    m_graphics_view->scene()->update(m_graphics_view->rect());
}

SDK::PluginObjectResult yasem::QtMediaPlayerObject::init()
{
    STUB();
    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    m_video_widget = new QGraphicsVideoItem();
    m_video_widget->setAspectRatioMode(Qt::KeepAspectRatio);
    mediaPlayer->setVideoOutput(m_video_widget);

    m_graphics_view = new QGraphicsView();
    m_graphics_view->setStyleSheet("background: black");
    m_graphics_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_graphics_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphics_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphics_view->setScene(new QGraphicsScene());
    m_graphics_view->scene()->addItem(m_video_widget);

    hide();

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &QtMediaPlayerObject::durationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &QtMediaPlayerObject::positionChanged);
    connect(mediaPlayer, SIGNAL(metaDataChanged(const QString &, const QVariant &)), this, SLOT(metaDataChanged(const QString &, const QVariant &)));
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &QtMediaPlayerObject::onMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::bufferStatusChanged, this, &QtMediaPlayerObject::bufferingProgress);
    connect(mediaPlayer, &QMediaPlayer::videoAvailableChanged, this, &QtMediaPlayerObject::videoAvailableChanged);
    connect(mediaPlayer, SIGNAL(error(QMediaPlayer::Error error)), SLOT(displayErrorMessage(QMediaPlayer::Error error)));
    return SDK::PLUGIN_OBJECT_RESULT_OK;
}

SDK::PluginObjectResult yasem::QtMediaPlayerObject::deinit()
{
    return SDK::PLUGIN_OBJECT_RESULT_OK;
}

int yasem::QtMediaPlayerObject::getAudioPID() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setAudioPID(int pid)
{
    STUB();
}

int yasem::QtMediaPlayerObject::getLoop() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setLoop(int loop)
{
    STUB();
}

bool yasem::QtMediaPlayerObject::isMute() const
{
    STUB();
    return mediaPlayer->isMuted();
}

void yasem::QtMediaPlayerObject::setMute(bool value)
{
    STUB();
    mediaPlayer->setMuted(value);
}

int yasem::QtMediaPlayerObject::bufferLoad() const
{
    STUB();
    return mediaPlayer->bufferStatus();
}

qint64 yasem::QtMediaPlayerObject::getPosition() const
{
    STUB();
    return mediaPlayer->position();
}

void yasem::QtMediaPlayerObject::setPosition(qint64 pos)
{
    STUB();
    mediaPlayer->setPosition(pos);
}

qint64 yasem::QtMediaPlayerObject::getDuration() const
{
    STUB();
    return mediaPlayer->duration();
}

int yasem::QtMediaPlayerObject::getVolume() const
{
    STUB();
    return mediaPlayer->volume();
}

void yasem::QtMediaPlayerObject::setVolume(int vol)
{
    STUB();
    mediaPlayer->setVolume(vol);
}

bool yasem::QtMediaPlayerObject::isVisible() const
{
    STUB();
    return true;
}

void yasem::QtMediaPlayerObject::setAspectRatio(SDK::AspectRatio mode)
{
    STUB();
}

SDK::AspectRatio yasem::QtMediaPlayerObject::getAspectRatio()
{
    STUB();
    return SDK::ASPECT_RATIO_AUTO;
}

QList<AudioLangInfo> yasem::QtMediaPlayerObject::getAudioLanguages()
{
    STUB();
    return QList<AudioLangInfo>();
}

void yasem::QtMediaPlayerObject::setAudioLanguage(int index)
{
    STUB();
}

void yasem::QtMediaPlayerObject::setBrightness(int brightness)
{
    STUB();
}

int yasem::QtMediaPlayerObject::getBrightness() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setContrast(int contrast)
{
    STUB();
}

int yasem::QtMediaPlayerObject::getContrast() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setSaturation(int saturation)
{
    STUB();
}

int yasem::QtMediaPlayerObject::getSaturation() const
{
    STUB();
    return 0;
}

MediaMetadata yasem::QtMediaPlayerObject::getMediaMetadata()
{
    return MediaMetadata();
}

QPoint yasem::QtMediaPlayerObject::getWidgetPos() const
{
    return QPoint();
}
