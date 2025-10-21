#pragma once

#include <QSettings>

class settings_manager : public QObject
{
    Q_OBJECT
public:
    explicit settings_manager(QObject* parent = nullptr);
    ~settings_manager();

    bool get_persistence_mode() const;
    void save_persistence_mode(const bool& toggle);

    QStringList get_servers() const;
    void save_servers(const QStringList& urls);

signals:
    void server_urls_changed(const QStringList& urls);
    void server_persistence_mode_changed(const bool& toggle);

private:
    QSettings m_settings;
};