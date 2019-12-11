#pragma once

#include "rotation.h"

#include <type_traits>
#include <stdexcept>
#include <functional>
#include <string>

template<typename T>
class CMatrix
{
private:
	T** m_Array;
	int m_BaseWidth;
	int m_BaseHeight;
	int m_Width;
	int m_Height;
	CRotation m_Rotation;
public:
	CMatrix()
		: m_BaseWidth(0), m_BaseHeight(0), m_Array(nullptr), m_Rotation(CRotation::CValue::NONE)
	{
		rotate(CRotation::CDir::NONE);
	}
	CMatrix(int Width, int Height)
		: m_BaseWidth(Width), m_BaseHeight(Height), m_Rotation(CRotation::CValue::NONE)
	{
		if (Width <= 0 || Height <= 0)
			throw std::bad_alloc();
		alloc();
		rotate(CRotation::CDir::NONE);
	}
	~CMatrix()
	{
		free();
	}

	operator bool() const { return static_cast<bool>(m_Array); }

	// copy constructor/assignment
	CMatrix(const CMatrix& r)
		: m_BaseWidth(r.m_BaseWidth), m_BaseHeight(r.m_BaseHeight), m_Rotation(r.m_Rotation), m_Width(r.m_Width), m_Height(r.m_Height)
	{
		alloc();
		for (int i = 0; i < m_BaseWidth; i++)
			for (int j = 0; j < m_BaseHeight; j++)
				m_Array[i][j] = r.m_Array[i][j];
	}
	CMatrix& operator=(const CMatrix& r) // !!FIX OPTIMIZATION!!
	{
		CMatrix other(r);
		std::swap(*this, other);
		return *this;
	}
	// move constructor/assignment
	CMatrix(CMatrix&& r)
		: m_BaseWidth(r.m_BaseWidth), m_BaseHeight(r.m_BaseHeight), m_Rotation(r.m_Rotation), m_Width(r.m_Width), m_Height(r.m_Height), m_Array(r.m_Array)
	{
		r.m_Array = nullptr;
		/*r.m_BaseHeight = 0;
		r.m_BaseWidth = 0;
		r.m_Width = 0;
		r.m_Height = 0;*/
	}
	CMatrix& operator=(CMatrix&& r)
	{
		free();
		m_Array = r.m_Array;
		m_BaseHeight = r.m_BaseHeight;
		m_BaseWidth = r.m_BaseWidth;
		m_Height = r.m_Height;
		m_Width = r.m_Width;
		m_Rotation = r.m_Rotation;

		r.m_Array = nullptr;
		/*r.m_BaseHeight = 0;
		r.m_BaseWidth = 0;
		r.m_Width = 0;
		r.m_Height = 0;*/
		return *this;
	}

	void insert(int x, int y, const CMatrix & other)
	{

	}
	void move(int x, int y, CMatrix& other)
	{

	}
	void rotate(CRotation::CDir Dir)
	{
		m_Rotation += Dir;
		setRotation(m_Rotation);
	}
	void setRotation(CRotation Rotation)
	{
		m_Rotation = Rotation;
		if (m_Rotation.get() == CRotation::CValue::QUARTER || m_Rotation == CRotation::CValue::THREE_QUARTER)
		{
			m_Width = m_BaseHeight;
			m_Height = m_BaseWidth;
		}
		else
		{
			m_Width = m_BaseWidth;
			m_Height = m_BaseHeight;
		}
	}
	CRotation getRotation() const
	{
		return m_Rotation;
	}

	int getWidth() const
	{
		return m_Width;
	}
	int getHeight() const
	{
		return m_Height;
	}

	T& at(int x, int y)
	{
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::at(" + std::to_string(x) + ", " + std::to_string(y) + ")");
		return m_Array[x][y];
	}
	const T& at(int x, int y) const
	{
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::at(" + std::to_string(x) + ", " + std::to_string(y) + ") const");
		return m_Array[x][y];
	}
	T& operator[](const std::pair<int, int>& p) noexcept
	{
		int x = p.first;
		int y = p.second;
		evaluate(x, y);
		return m_Array[x][y];
	}
	const T& operator[](const std::pair<int,  int>& p) const noexcept
	{
		int x = p.first;
		int y = p.second;
		evaluate(x, y);
		return m_Array[x][y];
	}
	template<typename U> void set(int x, int y, U&& v)
	{
		static_assert(std::is_same<typename std::remove_reference<U>::type, T>::value, "CMatrix<T>::set<U>(): template typenames do not match");
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::set(" + std::to_string(x) + ", " + std::to_string(y) + ")");
		m_Array[x][y] = std::forward<U>(v);
	}
	void fill(const T& v, std::function<bool(const T& c)> pred = [](const T& v) -> bool { return true; })
	{
		for (int i = 0; i < m_BaseWidth; i++)
			for (int j = 0; j < m_BaseHeight; j++)
				if (pred(m_Array[i][j]))
					m_Array[i][j] = T(v);
	}
	/*void fill(const T& v)
	{
		fill_if(v, [](const T& v) -> bool { return true; });
		//for (unsigned int i = 0; i < m_BaseWidth; i++)
		//	for (unsigned int j = 0; j < m_BaseHeight; j++)
		//		m_Array[i][j] = T(v);
	}
	void fill_if(const T& v, std::function<bool(const T& c)> pred)
	{
		for (unsigned int i = 0; i < m_BaseWidth; i++)
			for (unsigned int j = 0; j < m_BaseHeight; j++)
				if (pred(m_Array[i][j]))
					m_Array[i][j] = T(v);
	}*/
	bool checkPoint(int x, int y) const
	{
		if (x >= m_Width || y >= m_Height || x < 0 || y < 0)
			return false;
		return true;
	}
private:
	bool checkBasePoint(int x, int y) const
	{
		if (x >= m_BaseWidth || y >= m_BaseHeight || x < 0 || y < 0)
			return false;
		return true;
	}
	void alloc()
	{
		m_Array = new T*[m_BaseWidth];
		for (int i = 0; i < m_BaseWidth; i++)
			m_Array[i] = new T[m_BaseHeight];
	}
	void free()
	{
		if (!m_Array)
			return;
		for (int i = 0; i < m_BaseWidth; i++)
			delete[] m_Array[i];
		delete[] m_Array;
	}
	void evaluate(int& x, int& y) const
	{
		using CValue = CRotation::CValue;
		int rX, rY;

		switch (m_Rotation.get())
		{
		case CValue::QUARTER:
			rX = m_BaseWidth - 1 - y;
			rY = x;
			break;
		case CValue::HALF:
			rX = m_BaseWidth - 1 - x;
			rY = m_BaseHeight - 1 - y;
			break;
		case CValue::THREE_QUARTER:
			rX = y;
			rY = m_BaseHeight - 1 - x;
			break;
		default:
			rX = x;
			rY = y;
		}
		x = rX;
		y = rY;
	}
};