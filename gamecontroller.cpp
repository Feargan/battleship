#include "gamecontroller.h"
#include "gameboard.h"
#include "player.h"

CGameController::CGameController(const CGamePreset* Preset)
{
	m_Preset = Preset;
}

CGameController::~CGameController()
{
}

bool CGameController::attack(IPlayer * Attacker, const IPlayer* Victim, int x, int y)
{
	if (Attacker != whoseTurn())
		return false;
	auto VictimIt = std::find(m_Victims.begin(), m_Victims.end(), Victim);
	// check if the Victim can be attacked
	if (VictimIt == m_Victims.end())
		return false;
	// get the map iterator of our Victim so that we can acquire their board
	auto It = std::find_if(m_Players.begin(), m_Players.end(),
		[Victim](const decltype(m_Players)::value_type &p) -> bool
		{
			if (p.first == Victim)
				return true;
			return false;
		});

	if (It == m_Players.end())
		return false;
	// perform attack
	CTile::CState State = It->second.attack(x, y);
	if (State == CTile::CState::NO_ATTACK)
		return false;
	CGameEvent AttackEvent;
	AttackEvent.m_Type = CGameEvent::CType::PLAYER_ATTACKED;
	AttackEvent.m_PlayerAttackedEvent = { Victim, x, y, State };
	notifyObservers(AttackEvent);
	if (State == CTile::CState::MISS)
	{
		m_Victims.erase(VictimIt); // cannot be attacked more in this turn if someone misses
	}
	else if (State == CTile::CState::DESTROYED)
	{
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
		// let the next player play
		m_Queue.pop();
		m_Queue.push(Attacker);
		// remove all who lost
		while (m_Players[m_Queue.front()].destroyed())
			m_Queue.pop();
		if (m_Queue.size() <= 1)
		{
			// one player remained - it's done
			CGameEvent FinishEvent;
			FinishEvent.m_Type = CGameEvent::CType::GAME_FINISHED;
			FinishEvent.m_GameFinishedEvent.m_Winner = m_Queue.front();
			notifyObservers(FinishEvent);
		}
		else
		{
			prepareVictims();
			CGameEvent NextTurnEvent;
			NextTurnEvent.m_Type = CGameEvent::CType::GAME_NEXT_TURN;
			NextTurnEvent.m_GameNextTurnEvent.m_Next = m_Queue.front();
			notifyObservers(NextTurnEvent);
		}
	}
	return true;
}

void CGameController::start()
{
	if (!m_Players.size() || m_Queue.size()) // has it started before?
		return;
	// prepare the queue
	for (auto &p : m_Players)
		m_Queue.push(p.first);
	prepareVictims();
	notifyObservers({ CGameEvent::CType::GAME_STARTED });
}

void CGameController::run()
{
	if (m_Queue.size() && m_Victims.size())
		m_Queue.front()->play(); // call next player to play if the game is in progress
}

bool CGameController::isInProgress() const
{
	return m_Victims.size() && m_Queue.size();
}

const IPlayer * CGameController::whoseTurn() const
{
	return m_Queue.size() ? m_Queue.front() : nullptr;
}

const CGameBoard * CGameController::seat(IPlayer * Player, const CGameBoardBuilder& Builder)
{
	if (!(m_Players.find(Player) == m_Players.end() && Builder.isReady() && Builder.getPreset() == m_Preset))
		return nullptr;
	// all players receive game events by default
	addObserver(Player);
	return &(m_Players.emplace(Player, Builder.getBoard())).first->second;
}

void CGameController::addObserver(IObserver * Observer)
{
	m_Observers.insert(Observer);
}

void CGameController::removeObserver(IObserver * Observer)
{
	m_Observers.erase(Observer);
}

std::vector<const IPlayer*> CGameController::getPlayers() const
{
	std::vector<const IPlayer*> v;
	for (auto &p : m_Players)
		v.push_back(p.first);
	return v;
}

const IPlayer * CGameController::getSuggestedVictim() const
{
	if(m_Victims.size())
		return m_Victims.back();
	return nullptr;
}

const CGamePreset* CGameController::getPreset() const
{
	return m_Preset;
}

void CGameController::notifyObservers(const CGameEvent& Event)
{
	for (auto& o : m_Observers)
		o->onEvent(Event);
}

void CGameController::prepareVictims()
{
	m_Victims.clear();
	for (auto& p : m_Players)
		if (p.first != whoseTurn() && !p.second.destroyed())
			m_Victims.push_back(p.first);
}
