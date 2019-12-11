#include "rotation.h"



CRotation::CRotation(CValue Value) : m_Value(Value)
{
}


CRotation::~CRotation()
{
}

void CRotation::set(CValue Value)
{
	m_Value = Value;
}

CRotation::CValue CRotation::get() const
{
	return m_Value;
}

int CRotation::degrees() const
{
	switch (m_Value)
	{
	case CValue::QUARTER:
		return 90;
		break;
	case CValue::HALF:
		return 180;
		break;
	case CValue::THREE_QUARTER:
		return 270;
		break;
	}
	return 0;
}

CRotation& CRotation::operator+=(CDir Dir)
{
	switch(Dir)
	{
	case CDir::CLOCKWISE:
		{
			switch (m_Value)
			{
			case CValue::NONE:
				m_Value = CValue::QUARTER;
				break;
			case CValue::QUARTER:
				m_Value = CValue::HALF;
				break;
			case CValue::HALF:
				m_Value = CValue::THREE_QUARTER;
				break;
			case CValue::THREE_QUARTER:
				m_Value = CValue::NONE;
				break;
			}
		}
		break;
	case CDir::COUNTER_CLOCKWISE:
		{
			switch (m_Value)
			{
			case CValue::NONE:
				m_Value = CValue::THREE_QUARTER;
				break;
			case CValue::QUARTER:
				m_Value = CValue::NONE;
				break;
			case CValue::HALF:
				m_Value = CValue::QUARTER;
				break;
			case CValue::THREE_QUARTER:
				m_Value = CValue::HALF;
				break;
			}
		}	
		break;
	}
	return *this;
}

bool CRotation::operator==(const CRotation & r) const
{
	return m_Value == r.m_Value;
}

bool CRotation::operator!=(const CRotation & r) const
{
	return m_Value != r.m_Value;
}

std::ostream& operator<<(std::ostream& out, CRotation & r)
{
	return out << r.degrees();
}
