#pragma once

#include "tile.h"

class IPlayer;
class CShip;

struct CGameEvent
{
	enum class CType
	{
		PLAYER_ATTACKED,
		SHIP_DESTROYED,
		GAME_NEXT_TURN,
		GAME_STARTED,
		GAME_FINISHED,
		PLAYER_LOST,
	} m_Type;
	union
	{
		struct
		{
			const IPlayer* m_Victim;
			int m_X;
			int m_Y;
			CTile::CState m_State;
		} m_PlayerAttackedEvent;
		struct
		{
			const IPlayer* m_Victim;
			const CShip* m_Ship; // fix to shared ptr
		} m_ShipDestroyedEvent;
		struct
		{
			const IPlayer* m_Next;
		} m_GameNextTurnEvent;
		struct
		{
			const IPlayer* m_Winner;
		} m_GameFinishedEvent;
		struct
		{
			const IPlayer* m_Loser;
		} m_PlayerLostEvent;
	};
};