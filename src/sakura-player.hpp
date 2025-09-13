#pragma once

#include <QObject>
#include <mpv/client.h>

class SakuraPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(bool pause READ pause WRITE setPause NOTIFY pauseChanged)

public:
    explicit SakuraPlayer(QObject* parent = nullptr);
    ~SakuraPlayer();

    Q_INVOKABLE void command(const QVariantList &params);
    Q_INVOKABLE void setProperty(const QString &name, const QVariant &value);
    Q_INVOKABLE QVariant getProperty(const QString &name) const;

    double duration() const;
    double position() const;
    void setPosition(double value);

    bool pause() const;
    void setPause(bool paused);

    Q_INVOKABLE void togglePause();

signals:
    void durationChanged(double duration);
    void positionChanged(double position);
    void pauseChanged(bool paused);

private slots:
    void handleMpvEvents();

private:
    static void on_mpv_events(void *ctx);
    mpv_handle *m_mpv;
    double m_duration;
    double m_position;
    bool m_pause;
};
