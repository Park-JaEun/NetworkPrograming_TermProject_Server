#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType) : m_hDC(_dc), m_hDefaultBrush(nullptr), m_hDefaultPen(nullptr)
{
	HPEN hSelectPen = CCore::GetInst()->GetPen(_ePenType);
	m_hDefaultPen = (HPEN)SelectObject(_dc, hSelectPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType) : m_hDC(_dc), m_hDefaultBrush(nullptr), m_hDefaultPen(nullptr)
{
	HBRUSH hSelectBrush = CCore::GetInst()->GetBrush(_eBrushType);
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hSelectBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);
}
