#include "pch.h"
#include "CCamera.h"

#include "CObject.h"

#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimer.h"

CCamera::CCamera() : m_pTargetObj(nullptr)
{

}

CCamera::~CCamera()
{

}

void CCamera::update()
{
	if (m_pTargetObj) {
		if (m_pTargetObj->IsDead()) {
			m_pTargetObj = nullptr;
		}
		else {
			// 카메라가 맵 밖으로 나가지 못 하도록 설정
			if (m_pTargetObj->GetPos().x > 0.f && m_pTargetObj->GetPos().x < 5060.f) {
				m_vLookAt.x = m_pTargetObj->GetPos().x;
			}
		}
	}
	
	CalDiff();
}

void CCamera::CalDiff()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;

	m_vDiff = m_vLookAt - vCenter;
}
