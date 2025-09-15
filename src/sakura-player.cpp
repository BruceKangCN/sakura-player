#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QSGSimpleTextureNode>
#include <QQuickRenderTarget>
#include <QQuickWindow>
#include <stdexcept>

#include "sakura-player.hpp"

static void *get_proc_address(void *ctx, const char *name) {
    Q_UNUSED(ctx);
    QOpenGLContext *glctx = QOpenGLContext::currentContext();
    if (!glctx) return nullptr;
    return (void *)glctx->getProcAddress(QByteArray(name));
}

SakuraPlayer::SakuraPlayer(QQuickItem *parent)
    : QQuickItem(parent), m_mpv(nullptr), m_mpv_gl(nullptr),
      m_duration(0), m_position(0), m_playing(false)
{
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *window) {
        if (window) {
            connect(window, &QQuickWindow::beforeSynchronizing, this, &SakuraPlayer::initializeMpv, Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &SakuraPlayer::cleanupMpv, Qt::DirectConnection);
        }
    });
}

SakuraPlayer::~SakuraPlayer() {
    cleanupMpv();
}

void SakuraPlayer::componentComplete() {
    QQuickItem::componentComplete();
    initializeMpv();
}

void SakuraPlayer::initializeMpv() {
    if (m_mpv) return;

    m_mpv = mpv_create();
    if (!m_mpv)
        throw std::runtime_error("Could not create mpv context");

    // 设置MPV选项
    mpv_set_option_string(m_mpv, "terminal", "yes");
    mpv_set_option_string(m_mpv, "msg-level", "all=v");
    mpv_set_option_string(m_mpv, "hwdec", "auto");

    if (mpv_initialize(m_mpv) < 0)
        throw std::runtime_error("Could not initialize mpv context");

    // 监听属性变化
    mpv_observe_property(m_mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "pause", MPV_FORMAT_FLAG);

    // 设置渲染参数
    mpv_opengl_init_params gl_init_params{get_proc_address, nullptr};
    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
        {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    if (mpv_render_context_create(&m_mpv_gl, m_mpv, params) < 0)
        throw std::runtime_error("Failed to initialize mpv GL context");

    mpv_render_context_set_update_callback(m_mpv_gl, SakuraPlayer::on_update, this);
}

void SakuraPlayer::cleanupMpv() {
    if (m_mpv_gl) {
        mpv_render_context_free(m_mpv_gl);
        m_mpv_gl = nullptr;
    }

    if (m_mpv) {
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
    }
}

QSGNode *SakuraPlayer::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) {
    Q_UNUSED(data);

    if (!m_mpv_gl) {
        return node;
    }

    if (!window()) {
        return node;
    }

    QOpenGLFramebufferObject *fbo = window()->renderTarget();
    if (!fbo) return node;

    mpv_opengl_fbo mpfbo{
        static_cast<int>(fbo->handle()),
        static_cast<int>(width()),
        static_cast<int>(height()),
        0
    };

    int flip_y = 1;
    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
        {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    window()->beginExternalCommands();
    mpv_render_context_render(m_mpv_gl, params);
    window()->endExternalCommands();

    return node;
}

void SakuraPlayer::on_update(void *ctx) {
    auto* self = (SakuraPlayer*)ctx;
    QMetaObject::invokeMethod(self, "update");
}

void SakuraPlayer::handleMpvEvents() {
    while (m_mpv) {
        mpv_event *event = mpv_wait_event(m_mpv, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;

        switch (event->event_id) {
        case MPV_EVENT_PROPERTY_CHANGE: {
            auto* prop = (mpv_event_property*)event->data;
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
                    m_playing = !*(int *)prop->data;
                    emit playStateChanged(m_playing);
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void SakuraPlayer::loadFile(const QString &file) {
    if (!m_mpv) return;

    const char *cmd[] = {"loadfile", file.toUtf8().constData(), nullptr};
    mpv_command_async(m_mpv, 0, cmd);
}

void SakuraPlayer::togglePlayPause() {
    if (!m_mpv) return;

    const char *cmd[] = {"cycle", "pause", nullptr};
    mpv_command_async(m_mpv, 0, cmd);
}

void SakuraPlayer::seek(double position) {
    if (!m_mpv) return;

    QByteArray posStr = QByteArray::number(position);
    const char *cmd[] = {"seek", posStr.constData(), "absolute", nullptr};
    mpv_command_async(m_mpv, 0, cmd);
}

void SakuraPlayer::setVolume(int volume) {
    if (!m_mpv) return;

    QByteArray volStr = QByteArray::number(volume);
    const char *cmd[] = {"set", "volume", volStr.constData(), nullptr};
    mpv_command_async(m_mpv, 0, cmd);
}
