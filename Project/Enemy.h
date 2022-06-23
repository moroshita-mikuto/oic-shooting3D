#pragma once

#include "GameDefine.h"
#include "EnemyShot.h"

class CEnemy{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	bool				m_bShow;
	int					m_HP;
	int					m_ShotWait;
	int					m_ShotWaitSet;
	Vector3				m_TargetPos;

	float				m_AnimTime;
	int					m_Type;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p, int t = 0);
	void Update(CEnemyShot* shot,int smax);
	void Render();
	void RenderDebugText(int i);
	void Damage(int dmg);
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	bool GetShow(){ return m_bShow; }
	void RenderDebug();
	CSphere GetSphere() {
		switch (m_Type)
		{
		case1:
		case2:
		case3:
			return CSphere(m_Pos, 1.5f);
		}
		return CSphere(m_Pos, 0.5f);
	}
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }
	void UpdateType0(CEnemyShot* shot, int smax);
	void UpdateBossParts(CEnemyShot* shot, int smax);
	void SetPosition(const Vector3& p) { m_Pos = p; }
	void SetRotation(const Vector3& r) { m_Rot = r; }
};