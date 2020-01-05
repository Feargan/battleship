#pragma once

#include "rotation.h"

#include <type_traits>
#include <stdexcept>
#include <functional>
#include <string>

template<typename T>
class CMatrix
{
public:
	using ElementType = T;
private:
	T** m_Array;
	int m_BaseWidth;
	int m_BaseHeight;
	int m_Width;
	int m_Height;
	CRotation m_Rotation;
public:
	/*
		creates a matrix of an unspecified size
	*/
	CMatrix()
		: m_BaseWidth(0), m_BaseHeight(0), m_Array(nullptr), m_Height(0), m_Width(0), m_Rotation(CRotation::CValue::NONE)
	{
		rotate(CRotation::CDir::NONE);
	}
	/*
		creates a matrix of the size specified by the parameters
		throws std::bad_alloc() if the given size is incorrect or a general memory allocation failure occurs
	*/
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
	CMatrix(const CMatrix& r)
		: m_BaseWidth(r.m_BaseWidth), m_BaseHeight(r.m_BaseHeight), m_Rotation(r.m_Rotation),
		  m_Width(r.m_Width), m_Height(r.m_Height)
	{
		alloc();
		for (int i = 0; i < m_BaseWidth; i++)
			for (int j = 0; j < m_BaseHeight; j++)
				m_Array[i][j] = r.m_Array[i][j];
	}
	CMatrix& operator=(CMatrix r)
	{
		swap(*this, r);
		return *this;
	}
	CMatrix(CMatrix&& r)
		: CMatrix()
	{
		swap(*this, r);
	}
	friend void swap(CMatrix& l, CMatrix& r)
	{
		using std::swap;
		swap(l.m_Array, r.m_Array);
		swap(l.m_BaseHeight, r.m_BaseHeight);
		swap(l.m_BaseWidth, r.m_BaseWidth);
		swap(l.m_Height, r.m_Height);
		swap(l.m_Width, r.m_Width);
		swap(l.m_Rotation, r.m_Rotation);
	}

	/*
		rotates in a direction affecting the matrix width and height
	*/
	void rotate(CRotation::CDir Dir)
	{
		m_Rotation += Dir;
		setRotation(m_Rotation);
	}
	/*
		forcefully sets a rotation affecting the matrix width and height
	*/
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

	/* 
		returns the matrix width depending on the current rotation
	*/
	int getWidth() const
	{
		return m_Width;
	}
	/*
		returns the matrix height depending on the current rotation
	*/
	int getHeight() const
	{
		return m_Height;
	}

	/* 
		return a reference to element at (x,y)
		throws std::out_of_range()
	*/
	T& at(int x, int y)
	{
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::at(" + std::to_string(x) + ", " + std::to_string(y) + ")");
		return m_Array[x][y];
	}
	/* 
		return const a reference to the element at (x,y)
		throws std::out_of_range()
	*/
	const T& at(int x, int y) const
	{
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::at(" + std::to_string(x) + ", " + std::to_string(y) + ") const");
		return m_Array[x][y];
	}
	/* returns a reference to the element at (x,y) */
	T& operator[](const std::pair<int, int>& p) noexcept
	{
		int x = p.first;
		int y = p.second;
		evaluate(x, y);
		return m_Array[x][y];
	}
	/* 
		returns a const reference to the element at (x,y)
	*/
	const T& operator[](const std::pair<int,  int>& p) const noexcept
	{
		int x = p.first;
		int y = p.second;
		evaluate(x, y);
		return m_Array[x][y];
	}
	/* 
		returns true if the array was allocated, false otherwise
	*/
	operator bool() const { return static_cast<bool>(m_Array); }
	/*
		sets the element at (x,y)
		can be used to move an object into the array
		throws std::out_of_range()
	*/
	template<typename U> void set(int x, int y, U&& v)
	{
		static_assert(std::is_same<typename std::remove_reference<U>::type, T>::value, "CMatrix<T>::set<U>(): template typenames do not match");
		evaluate(x, y);
		if (!checkBasePoint(x, y))
			throw std::out_of_range("CMatrix::set(" + std::to_string(x) + ", " + std::to_string(y) + ")");
		m_Array[x][y] = std::forward<U>(v);
	}
	/*
		fills the array with v when the predicate returns true
	*/
	void fill(const T& v, std::function<bool(const T& c)> pred = [](const T& v) -> bool { return true; })
	{
		for (int i = 0; i < m_BaseWidth; i++)
			for (int j = 0; j < m_BaseHeight; j++)
				if (pred(m_Array[i][j]))
					m_Array[i][j] = T(v);
	}
	/*
		crops or extends the array filling new rows and columns with a default value
	*/
	void resize(int x, int y, const T& d = T())
	{
		CMatrix m(x, y);
		m.fill(d);
		m.setRotation(getRotation());
		for (int i = 0; i < m.getWidth() && i < getWidth(); i++)
			for (int j = 0; j < m.getHeight() && j < getHeight(); j++)
				m[{i, j}] = this->operator[]({ i,j });
		swap(*this, m);
	}
	/*
		returns true if the point (x,y) is in range of the rotated array, false otherwise
	*/
	bool checkPoint(int x, int y) const
	{
		if (x >= m_Width || y >= m_Height || x < 0 || y < 0)
			return false;
		return true;
	}
private:
	/*
		returns true if the point (x,y) is in range of the base array, false otherwise
	*/
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
	/*
		evaluate actual (x,y) point depending on the rotation
	*/
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