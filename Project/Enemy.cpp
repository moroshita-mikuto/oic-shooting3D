#include "Enemy.h"

ANIM_DATA g_EnemyAnimPosY[2] = {
	{1.0f,-10.0f,EASE_LINEAR},
	{3.0f,0.0f,EASE_INOUT_SINE},
};

ANIM_DATA g_EnemyAnimPosZ[5] = {
	{0.0f,-FIELD_HALF_Z,EASE_LINEAR},
	{1.0f,FIELD_HALF_Z - 10.0f,EASE_LINEAR},
	{2.0f,FIELD_HALF_Z,EASE_OUT_SINE},
	{3.0f,FIELD_HALF_Z - 10.0f,EASE_IN_SINE},
	{5.0f,-FIELD_HALF_Z,EASE_LINEAR},
};

/**
 * コンストラクタ
 *
 */
CEnemy::CEnemy():
m_pMesh(NULL),
m_Pos(0,0,0),
m_Rot(0,0,0),
m_bShow(false),
m_HP(5),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0,0,0),
m_AnimTime(0){
}

/**
 * デストラクタ
 *
 */
CEnemy::~CEnemy(){
}

/**
 * 初期化
 *
 */
void CEnemy::Initialize(){
	m_Pos = Vector3(0, 0, 0);
	m_Rot = Vector3(0, 0, 0);
	m_bShow = false;
	m_AnimTime = 0;
}

/**
 * 開始
 *
 */
void CEnemy::Start(const Vector3& p, int t){
	m_Pos = p;
	m_Rot = Vector3(0, 0, 0);
	m_bShow = true;
	m_HP = 5;
	m_ShotWait = 0;
	m_ShotWaitSet = 40; 
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;

	m_Type = t;
	switch (m_Type)
	{
	case1:
	case2:
	case3:
		m_HP = 100;
		m_ShotWaitSet = 100;
		m_ShotWait = m_ShotWaitSet;
		break;
	}
}

/**
 * 更新
 *
 */
void CEnemy::Update(CEnemyShot* shot, int smax) {
	if (!GetShow()) {
		return;
	}
	switch (m_Type)
	{
	case0:	UpdateType0(shot, smax);	break;
	case1:
	case2:
	case3:	UpdateBossParts(shot, smax);	break;
	}
}
void CEnemy::UpdateType0(CEnemyShot* shot, int smax) {
	m_AnimTime += CUtilities::GetFrameSecond();
	m_Pos.y = InterpolationAnim(m_AnimTime, g_EnemyAnimPosY, 2);
	m_Pos.z = InterpolationAnim(m_AnimTime, g_EnemyAnimPosZ, 5);
	if (g_EnemyAnimPosY[1].Time < m_AnimTime)
	{
		if (m_ShotWait <= 0)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot)
			{
				m_ShotWait = m_ShotWaitSet;
				Vector3 direction = m_TargetPos - m_Pos;
				float distance = CVector3Utilities::Length(direction);
				if (distance > 0)
				{
					direction /= distance;
					newShot->Fire(m_Pos, direction * 0.075f);
				}
			}
		}
		else
		{
			m_ShotWait--;
		}
	}
	if (g_EnemyAnimPosZ[4].Time < m_AnimTime)
	{
		m_bShow = false;
	}
}

void CEnemy::UpdateBossParts(CEnemyShot* shot, int smax) {
	if (m_ShotWait <= 0) 
	{
		m_ShotWait = m_ShotWaitSet;
	}
	else 
	{
		m_ShotWait--;
	}

	if (m_ShotWait % 10 == 0 && m_ShotWait / 10 < 3) 
	{
		int sCnt = m_Type - 1;
		for (int cnt = -sCnt; cnt <= sCnt; cnt++)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (!newShot) 
			{
				continue;
			}

			Vector3 pos = m_Pos;
			Vector3 direction = m_TargetPos - pos;

			float distance = CVector3Utilities::Length(direction);

			if (distance <= 0) 
			{
				continue;
			}

			direction /= distance;
			float ad = atan2(direction.z, direction.x) + cnt * MOF_ToRadian(10);
			Vector3 vt(cos(ad), 0, sin(ad));
			newShot->Fire(pos, vt * 0.2f);
		}
	}
}
	



/**
 * 描画
 *
 */
void CEnemy::Render(){
	if(!GetShow())
	{
		return;
	}

	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	m_pMesh->Render(matWorld);
	


}

/**
 * デバッグ文字描画
 *
 */
void CEnemy::RenderDebugText(int i){
	//位置の描画
	CGraphicsUtilities::RenderString(10,70 + i * 24,MOF_XRGB(0,0,0),
		"敵[%d] %s , 位置 X : %.1f , Y : %.1f , Z : %.1f",i + 1,
		(GetShow() ? "表示" : "非表示"),m_Pos.x,m_Pos.y,m_Pos.z);
}

void CEnemy::Damage(int dmg) {
	m_HP -= dmg;
	if (m_HP <= 0) 
	{
		m_bShow = false;
	}
}

void CEnemy::RenderDebug() {
	if (!GetShow())
	{
		return;
	}
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(1, 0, 0, 0.3f));
}
