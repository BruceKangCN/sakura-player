#include "sakura-player.hpp"
#include <QTimer>
#include <QDebug>

MpvQtPlayer::MpvQtPlayer(QObject *parent)
    : QObject(parent), m_mpv(nullptr), m_duration(0), m_position(0), m_pause(true)
{
    m_mpv = mpv_create();
    if (!m_mpv) {
        qWarning() << "Failed to create mpv context";
        return;
    }

    // 设置MPV选项
    mpv_set_option_string(m_mpv, "terminal", "yes");
    mpv_set_option_string(m_mpv, "msg-level", "all=v");

    if (mpv_initialize(m_mpv) < 0) {
        qWarning() << "Failed to initialize mpv context";
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
        return;
    }

    // 监听属性变化
    mpv_observe_property(m_mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "pause", MPV_FORMAT_FLAG);

    // 设置事件回调
    mpv_set_wakeup_callback(m_mpv, on_mpv_events, this);

    // 启动事件处理定时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MpvQtPlayer::handleMpvEvents);
    timer->start(10);
}

MpvQtPlayer::~MpvQtPlayer()
{
    if (m_mpv) {
        mpv_terminate_destroy(m_mpv);
    }
}

void MpvQtPlayer::command(const QVariantList &params)
{
    if (!m_mpv) return;

    QVector<QByteArray> data;
    QVector<const char *> cparams;

    for (const QVariant &param : params) {
        QByteArray ba = param.toString().toUtf8();
        data.append(ba);
        cparams.append(data.last().constData());
    }
    cparams.append(nullptr);

    mpv_command_async(m_mpv, 0, cparams.data());
}

void MpvQtPlayer::setProperty(const QString &name, const QVariant &value)
{
    if (!m_mpv) return;

    if (value.type() == QVariant::String) {
        mpv_set_option_string(m_mpv, name.toUtf8().constData(),
                             value.toString().toUtf8().constData());
    } else if (value.type() == QVariant::Int) {
        mpv_set_option_string(m_mpv, name.toUtf8().constData(),
                             QByteArray::number(value.toInt()).constData());
    } else if (value.type() == QVariant::Double) {
        mpv_set_option_string(m_mpv, name.toUtf8().constData(),
                             QByteArray::number(value.toDouble()).constData());
    } else if (value.type() == QVariant::Bool) {
        mpv_set_option_string(m_mpv, name.toUtf8().constData(),
                             value.toBool() ? "yes" : "no");
    }
}

QVariant MpvQtPlayer::getProperty(const QString &name) const
{
    if (!m_mpv) return QVariant();

    mpv_node node;
    if (mpv_get_property(m_mpv, name.toUtf8().constData(),
                        MPV_FORMAT_NODE, &node) >= 0) {
        if (node.format == MPV_FORMAT_DOUBLE) {
            return QVariant(node.u.double_);
        } else if (node.format == MPV_FORMAT_FLAG) {
            return QVariant(static_cast<bool>(node.u.flag));
        } else if (node.format == MPV_FORMAT_STRING) {
            return QVariant(QString::fromUtf8(node.u.string));
        }
        mpv_free_node_contents(&node);
    }
    return QVariant();
}

double MpvQtPlayer::duration() const
{
    return m_duration;
}

double MpvQtPlayer::position() const
{
    return m_position;
}

void MpvQtPlayer::setPosition(double value)
{
    if (!m_mpv) return;

    command({"seek", QString::number(value), "absolute"});
}

bool MpvQtPlayer::pause() const
{
    return m_pause;
}

void MpvQtPlayer::setPause(bool paused)
{
    if (!m_mpv) return;

    setProperty("pause", paused);
}

void MpvQtPlayer::togglePause()
{
    setPause(!m_pause);
}

void MpvQtPlayer::on_mpv_events(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<MpvQtPlayer*>(ctx),
                             "handleMpvEvents", Qt::QueuedConnection);
}

void MpvQtPlayer::handleMpvEvents()
{
    if (!m_mpv) return;

    while (true) {
        mpv_event *event = mpv_wait_event(m_mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }

        switch (event->event_id) {
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property *prop = (mpv_event_property *)event->data;
            if (strcmp(prop->name, "duration") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    m_duration = *(double *)prop->data;
                    emit durationChanged(m_duration);
                }
            } else if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    m_position = *(double *)prop->data;
                    emit positionChanged(m_position);
                }
            } else if (strcmp(prop->name, "pause") == 0) {
                if (prop->format == MPV_FORMAT_FLAG) {
                    m_pause = *(int *)prop->data;
                    emit pauseChanged(m_pause);
                }
            }
            break;
        }
        default:
            break;
        }
    }
}
