#include "gamecontroller.h"
#include "gameboard.h"
#include "player.h"

IGameController::IGameController(const CGamePreset* Preset)
{
	m_Preset = Preset;
}

IGameController::~IGameController()
{
}

bool IGameController::attack(IPlayer * Attacker, const IPlayer* Victim, int x, int y)
{
	if (Attacker != whoseTurn())
		return false;
	auto VictimIt = std::find(m_Victims.begin(), m_Victims.end(), Victim);
	if (VictimIt == m_Victims.end())
		return false;
	auto It = std::find_if(m_Players.begin(), m_Players.end(),
		[Victim](const decltype(m_Players)::value_type &p) -> bool
		{
			if (p.first == Victim)
				return true;
			return false;
		});

	if (It == m_Players.end())
		return false;
	CTile::CState State = It->second.attack(x, y);
	if (State == CTile::CState::NO_ATTACK)
		return false;
	CGameEvent AttackEvent;
	AttackEvent.m_Type = CGameEvent::CType::PLAYER_ATTACKED;
	AttackEvent.m_PlayerAttackedEvent = { Victim, x, y, State };
	notifyObservers(AttackEvent);
	if (State == CTile::CState::MISS)
	{
		m_Victims.erase(VictimIt);
	}
	else if (State == CTile::CState::DESTROYED)
	{
		CGameEvent DestroyEvent;
		DestroyEvent.m_Type = CGameEvent::CType::SHIP_DESTROYED;
		DestroyEvent.m_ShipDestroyedEvent.m_Ship = It->second.getField().at(x, y).getOwner().get(); // fix to shared_ptr
		DestroyEvent.m_ShipDestroyedEvent.m_Victim = Victim;
		notifyObservers(DestroyEvent);

		if (It->second.destroyed())
		{
			m_Victims.erase(VictimIt);
			CGameEvent LoseEvent;
			LoseEvent.m_Type = CGameEvent::CType::PLAYER_LOST;
			LoseEvent.m_PlayerLostEvent.m_Loser = Victim;
			notifyObservers(LoseEvent);
		}
	}
	if (!m_Victims.size())
	{
		m_Queue.pop();
		m_Queue.push(Attacker);
		while (m_Players[m_Queue.front()].destroyed())
			m_Queue.pop();
		prepareVictims();
	}
	return true;
}

void IGameController::start()
{
	if (!m_Players.size() || m_Queue.size())
		return;
	for (auto &p : m_Players)
		m_Queue.push(p.first);
	prepareVictims();
	notifyObservers({ CGameEvent::CType::GAME_STARTED });
}

void IGameController::run()
{
	if (m_Queue.size() && m_Victims.size())
	{
		//while (m_Players[m_Queue.front()].destroyed())
		//	m_Queue.pop();
		m_Queue.front()->play();
	}
}

bool IGameController::isInProgress() const
{
	return m_Victims.size() > 0;
}

const IPlayer * IGameController::whoseTurn() const
{
	return m_Queue.size() ? m_Queue.front() : nullptr;
}

const CGameBoard * IGameController::seat(IPlayer * Player, const CGameBoard & Board)
{
	if (!canSeat(Player, Board))
		return nullptr;
	addObserver(Player);
	auto& Emplaced = (m_Players.emplace(Player, Board)).first->second;
	Emplaced.cleanup();
	return &Emplaced;
}

const CGameBoard * IGameController::seat(IPlayer* Player, CGameBoard&& Board) // probably will get removed
{
	if (!canSeat(Player, Board))
		return nullptr;
	addObserver(Player);
	auto& Emplaced = (m_Players.emplace(Player, Board)).first->second;
	Emplaced.cleanup();
	return &Emplaced;
}

void IGameController::addObserver(IObserver * Observer)
{
	m_Observers.insert(Observer);
}

void IGameController::removeObserver(IObserver * Observer)
{
	m_Observers.erase(Observer);
}

std::vector<const IPlayer*> IGameController::getPlayers() const
{
	std::vector<const IPlayer*> v;
	for (auto &p : m_Players)
		v.push_back(p.first);
	return v;
}

const IPlayer * IGameController::getSuggestedVictim() const
{
	if(m_Victims.size())
		return m_Victims.back();
	return nullptr;
}

const CGamePreset* IGameController::getPreset() const
{
	return m_Preset;
}

bool IGameController::canSeat(IPlayer * Player, const CGameBoard & Board)
{
	return m_Players.find(Player) == m_Players.end() && Board.isReady() && Board.getPreset() == m_Preset;
}

void IGameController::notifyObservers(const CGameEvent& Event)
{
	for (auto& o : m_Observers)
		o->onEvent(Event);
}

void IGameController::prepareVictims()
{
	m_Victims.clear();
	for (auto& p : m_Players)
		if (p.first != whoseTurn() && !p.second.destroyed())
			m_Victims.push_back(p.first);
}
