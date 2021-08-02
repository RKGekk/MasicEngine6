#pragma once

#include <Windows.h>

#include "point.h"

class Rect {
public:
	long top;
	long left;
	long right;
	long bottom;

	enum class RectCorner {
		INVALID_CORNER = 0,
		TOPLEFT_CORNER,
		TOPRIGHT_CORNER,
		BOTTOMLEFT_CORNER,
		BOTTOMRIGHT_CORNER
	};

	Rect() { left = top = right = bottom = 0; }
	Rect(long newLeft, long newTop, long newRight, long newBottom) { Set(newLeft, newTop, newRight, newBottom); }
	Rect(const Rect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }
	Rect(Rect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; }
	Rect(const Point& topLeft, const Point& bottomRight) { top = topLeft.y; left = topLeft.x; right = bottomRight.x; bottom = bottomRight.y; }
	Rect(const RECT& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }

	Rect& operator=(const Rect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; return (*this); }
	Rect& operator=(const Rect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; return (*this); }

	Rect& operator+=(const Rect& newRect) { left += newRect.left; top += newRect.top; right += newRect.right; bottom += newRect.bottom; return (*this); }
	Rect& operator-=(const Rect& newRect) { left -= newRect.left; top -= newRect.top; right -= newRect.right; bottom -= newRect.bottom; return (*this); }
	Rect& operator+=(const Rect* pNewRect) { left += pNewRect->left; top += pNewRect->top; right += pNewRect->right; bottom += pNewRect->bottom; return (*this); }
	Rect& operator-=(const Rect* pNewRect) { left -= pNewRect->left; top -= pNewRect->top; right -= pNewRect->right; bottom -= pNewRect->bottom; return (*this); }
	Rect operator+(Rect& other) { Rect temp(this); temp += other; return temp; }
	Rect operator-(Rect& other) { Rect temp(this); temp -= other; return temp; }

	Rect& operator+=(const Point& delta) { left += delta.x; top += delta.y; right += delta.x; bottom += delta.y; return (*this); }
	Rect& operator-=(const Point& delta) { left -= delta.x; top -= delta.y; right -= delta.x; bottom -= delta.y; return (*this); }
	Rect& operator+=(const Point* pDelta) { left += pDelta->x; top += pDelta->y; right += pDelta->x; bottom += pDelta->y; return (*this); }
	Rect& operator-=(const Point* pDelta) { left -= pDelta->x; top -= pDelta->y; right -= pDelta->x; bottom -= pDelta->y; return (*this); }
	Rect operator+(Point& delta) { Rect temp(this); temp += delta; return temp; }
	Rect operator-(Point& delta) { Rect temp(this); temp -= delta; return temp; }

	bool operator==(const Rect& other) const { return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom)); }
	bool operator!=(const Rect& other) const { return (!((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom))); }

	bool Collide(const Rect& other) const { if (other.bottom < top || other.right < left || other.left > right || other.top > bottom) { return false; } else { return true; } }
	bool Collide(const Point& p) const { if (p.x > left && p.x < right && p.y < bottom && p.y > top) { return true; } else { return false; } }
	bool IsWithin(const Rect& other) const { return ((left >= other.left && top >= other.top && right <= other.right && bottom <= other.bottom) || (other.left >= left && other.top >= top && other.right <= right && other.bottom <= bottom)); }
	bool IsWithin(const Point& other) const { return (other.x >= left && other.x <= right && other.y >= top && other.y <= bottom); }
	bool IsValid(void) const { return (left <= right && top <= bottom); }
	bool IsNull(void) const { return (left == 0 && right == 0 && top == 0 && bottom == 0); }

	void ShiftX(int dx) { left += dx; right += dx; }
	void ShiftY(int dy) { top += dy; bottom += dy; }
	void SetX(int x) { int dx = x - left; ShiftX(dx); }
	void SetY(int y) { int dy = y - top; ShiftY(dy); }

	long GetTop(void) const { return top; }
	long GetLeft(void) const { return left; }
	long GetRight(void) const { return right; }
	long GetBottom(void) const { return bottom; }
	Point GetCenter(void) const 	{
		if (IsValid()) {
			return (Point(left + ((right - left) / 2), top + ((bottom - top) / 2)));
		}
		return Point();
	}
	Point TopLeft(void) const { return Point(left, top); }
	Point TopRight(void) const { return Point(right, top); }
	Point BottomLeft(void) const { return Point(left, bottom); }
	Point BottomRight(void) const { return Point(right, bottom); }
	long GetWidth(void) const { return right - left; }
	long GetHeight(void) const { return bottom - top; }
	void Set(long newLeft, long newTop, long newRight, long newBottom) { left = newLeft; top = newTop; right = newRight; bottom = newBottom; }
	void MoveDelta(long dx, long dy) { left += dx; top += dy; right += dx; bottom += dy; }
	void MoveDelta(const Point deltaPoint) { MoveDelta(deltaPoint.x, deltaPoint.y); }
	void MoveTo(long x, long y) 	{
		long width = right - left;
		long height = bottom - top;
		left = x;
		right = left + width;
		top = y;
		bottom = top + height;
	}
	void MoveTo(const Point& point) { MoveTo(point.x, point.y); }
};