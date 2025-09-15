#pragma once

#include <QQuickRhiItem>
#include <QQuickRhiItemRenderer>
#include <mpv/client.h>
#include <mpv/render.h>

class SakuraPlayerRenderer;

class SakuraPlayer : public QQuickRhiItem
{
    Q_OBJECT
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(double position READ position NOTIFY positionChanged)
    Q_PROPERTY(bool playing READ playing NOTIFY playStateChanged)

public:
    SakuraPlayer(QQuickItem *parent = nullptr);
    ~SakuraPlayer();

    double duration() const { return m_duration; }
    double position() const { return m_position; }
    bool playing() const { return m_playing; }

    Q_INVOKABLE void loadFile(const QString &file);
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void seek(double position);
    Q_INVOKABLE void setVolume(int volume);

    QQuickRhiItemRenderer* createRenderer() override;

    mpv_render_context* mpvRenderContext() const
    {
        return m_mpv_gl;
    }

signals:
    void durationChanged(double duration);
    void positionChanged(double position);
    void playStateChanged(bool playing);

protected:
    virtual void componentComplete() override;

    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
    static void on_update(void *ctx);
    void handleMpvEvents();
    void initializeMpv();
    void cleanupMpv();

    mpv_handle *m_mpv;
    mpv_render_context *m_mpv_gl;
    double m_duration;
    double m_position;
    bool m_playing;
};

class SakuraPlayerRenderer : public QQuickRhiItemRenderer
{
public:
    SakuraPlayerRenderer() = default;
    ~SakuraPlayerRenderer() override = default;

    // 初始化渲染资源
    void initialize(QRhiCommandBuffer *cb) override;
    // 同步主线程（SakuraPlayer）和渲染线程（SakuraPlayerRenderer）的数据
    void synchronize(QQuickRhiItem *item) override;
    // 执行实际的渲染操作
    void render(QRhiCommandBuffer *cb) override;

private:
    SakuraPlayer *m_player = nullptr; // 对应的 QQuickRhiItem
    mpv_render_context *m_mpv_gl = nullptr; // 从 SakuraPlayer 同步过来的 mpv 渲染上下文

    // 可能需要一个离屏的 QRhiTexture 供 mpv 渲染，然后我们再将其绘制到屏幕上
    // 或者根据 mpv_render_context_render 的需求配置参数
    QRhiTexture *m_offscreenTexture = nullptr;
    QRhiTextureRenderTarget *m_offscreenRenderTarget = nullptr;
    QRhiRenderPassDescriptor *m_offscreenRenderPassDesc = nullptr;

    // 用于全屏绘图的资源
    QRhiShaderResourceBindings *m_srb = nullptr;
    QRhiGraphicsPipeline *m_pipeline = nullptr;
    QRhiBuffer *m_vbuf = nullptr;
    QRhiBuffer *m_ubuf = nullptr;
    QMatrix4x4 m_mvp; // 模型视图投影矩阵

    // 其他可能的渲染状态变量
    QSize m_lastSize; // 记录上次的尺寸以判断是否需要调整纹理大小
};
