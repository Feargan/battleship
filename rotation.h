#pragma once

#include <ostream>

class CRotation
{
public:
	enum class CDir
	{
		NONE,
		CLOCKWISE,
		COUNTER_CLOCKWISE,
	};
	enum class CValue
	{
		NONE,
		QUARTER,
		HALF,
		THREE_QUARTER,
	};
private:
	CValue m_Value;
public:
	CRotation(CValue Value = CValue::NONE);
	~CRotation();

	void set(CValue Value);
	CValue get() const;

	int degrees() const;

	CRotation& operator+=(CDir Dir);
	bool operator==(const CRotation& r) const;
	bool operator!=(const CRotation& r) const;

	friend std::ostream& operator<<(std::ostream& out, CRotation& r);
};

