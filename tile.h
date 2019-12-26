#pragma once

#include <memory>

class CShip;

class CTile
{
public:
	enum class CState
	{
		NO_ATTACK,

		EMPTY,
		MISS,
		HIT,
		DESTROYED,

		TAKEN,
		RESERVED,
		VITRIFIED,
	};
private:
	std::shared_ptr<CShip> m_Owner;
	CState m_State;
public:
	CTile(CState State = CState::EMPTY);
	CTile(const std::shared_ptr<CShip>& Owner);

	~CTile();

	CState getState() const;
	void setState(CState State);

	void setOwner(const std::shared_ptr<CShip>& Owner);
	const std::shared_ptr<CShip>& getOwner() const;

	CState attack();
};

