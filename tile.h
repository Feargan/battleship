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
		RESERVED,	// helper state for the builder, marks tiles which can be taken by an uncommitted ship
		VITRIFIED,	// helper state for the builder, treated as "impossible to occupy for these sizes of ships"
	};
private:
	std::shared_ptr<CShip> m_Owner;
	CState m_State;
public:
	CTile(CState State = CState::EMPTY);
	CTile(const std::shared_ptr<CShip>& Owner);
	CTile(const CTile& r);
	CTile(CTile&& r);
	CTile& operator=(CTile r);

	friend void swap(CTile& l, CTile& r);

	~CTile();

	CState getState() const;
	void setState(CState State);

	void setOwner(const std::shared_ptr<CShip>& Owner);
	const std::shared_ptr<CShip>& getOwner() const;

	CState attack();
};

