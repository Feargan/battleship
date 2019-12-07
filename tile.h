#pragma once

#include <memory>

class CShip;

class CTile
{
public:
	enum CState
	{
		NO_ATTACK = -1,
		EMPTY = 0,
		TAKEN,
		MISS,
		HIT,
		DESTROYED,
	};
private:
	std::shared_ptr<CShip> m_Owner;
	CState m_State;
public:
	CTile(CState State = EMPTY);
	CTile(const std::shared_ptr<CShip>& Owner);

	~CTile();

	CState getState() const;
	void setState(CState State);

	void setOwner(const std::shared_ptr<CShip>& Owner);
	const std::shared_ptr<CShip>& getOwner() const;

	CState attack();
};

