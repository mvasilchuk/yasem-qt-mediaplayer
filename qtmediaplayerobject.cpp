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
    SDK::MediaPlayer(plugin),
    mediaPlayer(NULL)
{
    m_support_opengl = false;
}

QtMediaPlayerObject::~QtMediaPlayerObject()
{
    STUB();

    if(m_graphics_view)
        m_graphics_view->deleteLater();
    if(m_video_widget)
        m_video_widget->deleteLater();
    if(mediaPlayer)
        mediaPlayer->deleteLater();
}

void QtMediaPlayerObject::parent(QWidget *parent)
{
    m_graphics_view->setParent(parent);
}

QWidget *QtMediaPlayerObject::widget() const
{
    return m_graphics_view;
}

bool QtMediaPlayerObject::mediaPlay(const QString &url)
{
    STUB();
    LOG() << url;

    m_media_info = MediaInfo();

    QString newUrl(url);
    if(newUrl.startsWith("/"))
        newUrl = QString("file://").append(newUrl);

    //m_graphics_view->setAutoFillBackground(true);

    playList = new QMediaPlaylist(mediaPlayer);
    playList->addMedia(QMediaContent(QUrl(newUrl)));
    playList->setCurrentIndex(1);
    mediaPlayer->setPlaylist(playList);
    mediaPlayer->setVolume(50);

    mediaPlayer->play();
    state(SDK::PlayingState);
    return true;
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
    STUB();
    return true;
}

void QtMediaPlayerObject::show()
{
    STUB();
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
    // FIXME: If either width or height has ever been set to 0 QGraphicsView will never show picture
    // anymore (at least on Linux).
    // I don't know why it happens and there is a chance the bug is somewhere else.
    if(rect.height() != 0 && rect.width() != 0)
    {
        m_graphics_view->setGeometry(rect);
        m_graphics_view->setSceneRect(0, 0, rect.width(), rect.height());
        m_video_widget->setSize(QSize(rect.width(), rect.height()));
    }
    else
    {
        QWidget* par = m_graphics_view->parentWidget();
        if(par)
        {
            m_graphics_view->setGeometry(par->geometry().width() / 2, par->geometry().height() / 2, 10, 10);
        }
    }
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
            emit stopped();
            break;
        }
    }

    return true;
}


void QtMediaPlayerObject::move(int x, int y)
{
    STUB() << x << y;
    widget()->move(x, y);
}

void QtMediaPlayerObject::raise()
{
    STUB();
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

    if(key == "VideoCodec")
    {
        m_media_info.video_codec = value.toString();
        emit statusChanged(SDK::VideoInfoReceived);
    }
    else if(key == "AudioCodec")
    {
        m_media_info.audio_codec = value.toString();
        if(!m_media_info.video_codec.isEmpty())
            emit statusChanged(SDK::MediaInfoReceived);
    }
    else if(key == "Language")
        m_media_info.language = value.toString();
    else if(key == "container-format")
        m_media_info.container = value.toString();
    else if(key == "PixelAspectRatio")
        m_media_info.pixel_ratio = value.toSize();
    else if(key == "Resolution")
        m_media_info.resolution = value.toSize();
    else if(key == "maximum-bitrate")
        m_media_info.max_bitrate = value.toUInt();
    else if(key == "minimum-bitrate")
        m_media_info.min_bitrate = value.toUInt();
    else if(key == "AudioBitRate")
        m_media_info.audio_bitrate = value.toUInt();
    else if(key == "ContributingArtist")
        m_media_info.contributing_artist = value.toString();
    else if(key == "Title")
        m_media_info.title = value.toString();
    else if(key == "encoder")
        m_media_info.encoder = value.toString();
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
    if(available)
        emit started();
}

void QtMediaPlayerObject::availabilityChanged(QMultimedia::AvailabilityStatus availability)
{
    STUB() << availability;
    switch(availability)
    {
        case QMultimedia::Available: {
            emit errorHappened(MediaPlayer::MEDIA_ERROR_NO_ERROR);
            break;
        }
        case QMultimedia::ServiceMissing: {
            emit errorHappened(MediaPlayer::MEDIA_ERROR_SERVICE_MISSING);
            break;
        }
        case QMultimedia::ResourceError: {
            emit errorHappened(MediaPlayer::MEDIA_ERROR_RESOURCE_ERROR);
            break;
        }
        case QMultimedia::Busy: {
            emit errorHappened(MediaPlayer::MEDIA_ERROR_RESOURCE_BUSY);
            break;
        }
        default: {
            WARN() << "Unknown availability status" << availability;
            break;
        }
    }
}

void QtMediaPlayerObject::displayErrorMessage(QMediaPlayer::Error error)
{
    STUB() << "Error" << error << ":" << mediaPlayer->errorString();
    emit stopped();
}

void QtMediaPlayerObject::setViewport(const QRect &requestedRect)
{
    STUB() << requestedRect;
    MediaPlayer::setViewport(requestedRect);
    m_graphics_view->scene()->update(m_graphics_view->rect());
}

void QtMediaPlayerObject::setViewport(const QRect &containerRect, const qreal containerScale, const QRect &requestedRect)
{
    STUB() << containerRect << containerScale << requestedRect;
    MediaPlayer::setViewport(containerRect, containerScale, requestedRect);
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

    connect(mediaPlayer, &QMediaPlayer::durationChanged,
            this, &QtMediaPlayerObject::durationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged,
            this, &QtMediaPlayerObject::positionChanged);
    connect(mediaPlayer, SIGNAL(metaDataChanged(const QString &, const QVariant &)),
            this, SLOT(metaDataChanged(const QString &, const QVariant &)));
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &QtMediaPlayerObject::onMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::bufferStatusChanged,
            this, &QtMediaPlayerObject::bufferingProgress);
    connect(mediaPlayer, &QMediaPlayer::videoAvailableChanged,
            this, &QtMediaPlayerObject::videoAvailableChanged);
    connect(mediaPlayer, SIGNAL(availabilityChanged(QMultimedia::AvailabilityStatus)),
            this, SLOT(availabilityChanged(QMultimedia::AvailabilityStatus)));
    connect(mediaPlayer, SIGNAL(error(QMediaPlayer::Error)),
            SLOT(displayErrorMessage(QMediaPlayer::Error)));

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
    STUB() << pid;
}

int yasem::QtMediaPlayerObject::getLoop() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setLoop(int loop)
{
    STUB() << loop;
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
    STUB() << mode;
}

SDK::AspectRatio yasem::QtMediaPlayerObject::getAspectRatio() const
{
    STUB();
    return SDK::ASPECT_RATIO_AUTO;
}

QList<AudioLangInfo> yasem::QtMediaPlayerObject::getAudioLanguages()
{
    STUB();
    QList<AudioLangInfo> list;
    list.append(AudioLangInfo(0, "", ""));
    return list;
}

void yasem::QtMediaPlayerObject::setAudioLanguage(int index)
{
    STUB() << index;
}

void yasem::QtMediaPlayerObject::setBrightness(int brightness)
{
    STUB() << brightness;
}

int yasem::QtMediaPlayerObject::getBrightness() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setContrast(int contrast)
{
    STUB() << contrast;
}

int yasem::QtMediaPlayerObject::getContrast() const
{
    STUB();
    return 0;
}

void yasem::QtMediaPlayerObject::setSaturation(int saturation)
{
    STUB() << saturation;
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
