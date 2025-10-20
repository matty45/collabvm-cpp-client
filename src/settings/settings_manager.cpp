#include "settings_manager.h"

settings_manager::settings_manager(QObject* parent) :
	QObject(parent), settings("./config.ini",QSettings::IniFormat)
{

}

settings_manager::~settings_manager()
{
}

bool settings_manager::get_persistence_mode() const
{
	return settings.value("Servers/persistence_mode", false).toBool();
}

void settings_manager::save_persistence_mode(const bool& toggle) {
	settings.setValue("Servers/persistence_mode", toggle);
	emit server_persistence_mode_changed(toggle);  // Notify all listeners  
}

QStringList settings_manager::get_servers() const
{
	QStringList defaults = {
		"wss://computernewb.com/collab-vm/vm1",
		"wss://computernewb.com/collab-vm/vm2",
		"wss://computernewb.com/collab-vm/vm3",
		"wss://computernewb.com/collab-vm/vm4",
		"wss://computernewb.com/collab-vm/vm5",
		"wss://computernewb.com/collab-vm/vm6",
		"wss://computernewb.com/collab-vm/vm7",
		"wss://computernewb.com/collab-vm/vm8",
		"wss://computernewb.com/collab-vm/vm9"

	};
	return settings.value("Servers/urls", defaults).toStringList();
}

void settings_manager::save_servers(const QStringList& urls) {
	settings.setValue("Servers/urls", urls);
	emit server_urls_changed(urls);  // Notify all listeners  
}