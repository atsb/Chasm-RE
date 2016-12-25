#pragma once

#include "../commands_processor.hpp"
#include "../connection_info.hpp"
#include "../time.hpp"
#include "i_connections_listener.hpp"
#include "fwd.hpp"
#include "map.hpp"

namespace PanzerChasm
{

class Server final
{
public:
	Server(
		CommandsProcessor& commands_processor,
		const GameResourcesConstPtr& game_resources,
		const MapLoaderPtr& map_loader,
		const IConnectionsListenerPtr& connections_listener );
	~Server();

	void Loop();

	void ChangeMap( unsigned int map_number, DifficultyType difficulty );

public: // Messages handlers
	void operator()( const Messages::MessageBase& message );
	void operator()( const Messages::PlayerMove& message );

private:
	enum class State
	{
		NoMap,
		PlayingMap,
	};

	struct ConnectedPlayer final
	{
		ConnectedPlayer(
			const IConnectionPtr& connection,
			const GameResourcesConstPtr& game_resoruces,
			Time current_time );

		ConnectionInfo connection_info;
		PlayerPtr player;
		EntityId player_monster_id;
	};

	typedef std::unique_ptr<ConnectedPlayer> ConnectedPlayerPtr;

private:
	void UpdateTimes();

	void GiveAmmo();
	void GiveArmor();
	void GiveWeapon();
	void GiveKeys();
	void ToggleGodMode();
	void ToggleNoclip();

private:
	const GameResourcesConstPtr game_resources_;
	const MapLoaderPtr map_loader_;
	const IConnectionsListenerPtr connections_listener_;

	CommandsMapConstPtr commands_;

	State state_= State::NoMap;
	unsigned int current_map_number_= ~0;
	MapDataConstPtr current_map_data_;
	std::unique_ptr<Map> map_;

	bool map_end_triggered_= false;

	std::vector<ConnectedPlayerPtr> players_;
	ConnectedPlayer* current_player_= nullptr;

	Time last_tick_; // Real time
	Time server_accumulated_time_; // Not real, can be faster or slower.
	Time last_tick_duration_;

	// Cheats
	bool noclip_= false;
};

} // PanzerChasm