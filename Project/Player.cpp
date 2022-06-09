#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f),
m_bDead(false),
m_Speed(0.0f),
m_ShotMesh(),
m_ShotArray(),
m_ShotWait(){
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	if (!m_Mesh.Load("player.mom")) 
	{
		return false;
	}

	if (!m_ShotMesh.Load("pshot.mom"))
	{
		return false;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_RotZ = 0;
	m_bDead = false;
	m_Speed = PLAYER_SPEED;
	m_ShotMode = MODE_DOUBLE;
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
}

/**
 * 更新
 */
void CPlayer::Update(void){
	if (m_bDead)
	{
		return;
	}
	//回転方向
	float Roll = 0;
	float RotSpeed = MOF_ToRadian(10);
	//キーボードでの移動
	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT)) 
	{
		m_Speed = 0.7f;
		RotSpeed = 0.7f;
	}
	else 
	{
		m_Speed = PLAYER_SPEED;
	}

	if (g_pInput->IsKeyHold(MOFKEY_LEFT)) 
	{
		m_Pos.x = max(m_Pos.x - m_Speed, -FIELD_HALF_X);
		Roll -= MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_Pos.x = min(m_Pos.x + m_Speed, FIELD_HALF_X);
		Roll += MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_Pos.z = min(m_Pos.z + m_Speed, FIELD_HALF_Z);
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_Pos.z = max(m_Pos.z - m_Speed, -FIELD_HALF_Z);
	}

	//回転
	
	if (Roll == 0)
	{
		RotSpeed = min(abs(m_RotZ) * 0.1f, RotSpeed);
	}
	if (abs(m_RotZ) <= RotSpeed || signbit(m_RotZ) != signbit(Roll)) 
	{
		m_RotZ += Roll;
	}
	m_RotZ -= copysignf(min(RotSpeed, abs(m_RotZ)), m_RotZ);
	
	
	UpdateMode();
	

	if (m_ShotWait <= 0)
	{
		if (g_pInput->IsKeyHold(MOFKEY_SPACE)) 
		{
			switch (m_ShotMode)
			{
			case MODE_SINGLE:
				UpdateSingleMode();
					break;
			case MODE_DOUBLE:
				UpdateDoubleMode();
					break;
			}
			
		}
	}
	else
	{
		m_ShotWait--;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++) 
	{
		m_ShotArray[i].Update();
	}
}
void CPlayer::UpdateMode(void) {
	if (g_pInput->IsKeyPush(MOFKEY_1)) {
		m_ShotMode = MODE_SINGLE;
	}
	if (g_pInput->IsKeyPush(MOFKEY_2)) {
		m_ShotMode = MODE_DOUBLE;
	}
}
void CPlayer::UpdateSingleMode(void){
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if (m_ShotArray[i].GetShow()) { continue; }
		CVector3 ShotPos(0, 0, 0);
		ShotPos.RotationZ(m_RotZ);
		ShotPos += m_Pos;
		m_ShotWait = PLAYERSHOT_WAIT;
		m_ShotArray[i].Fire(ShotPos);
		break;
	}
}
void CPlayer::UpdateDoubleMode(void) {
	for (int cnt = 0; cnt < 2; cnt++)
	{
		for (int i = 0; i < PLAYERSHOT_COUNT; i++)
		{
			if (m_ShotArray[i].GetShow()) { continue; }
			CVector3 ShotPos(0.4f * (cnt * 2 - 1), 0, 0);
			ShotPos.RotationZ(m_RotZ);
			ShotPos += m_Pos;
			m_ShotWait = PLAYERSHOT_WAIT;
			m_ShotArray[i].Fire(ShotPos);
			break;
		}
	}
}
/**
 * 描画
 */
void CPlayer::Render(void){
	if (m_bDead)
	{
		return;
	}
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	m_Mesh.Render(matWorld);
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**                                          
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
	
}
void CPlayer::RenderDebug(void) {
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug();
	}
}

void CPlayer::CollisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if (ps.CollisionSphere(es)) 
	{
		m_bDead = true;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++) 
	{
		if (!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

void CPlayer::CollisionEnemyShot(CEnemyShot& shot) {
	CSphere ps = GetSphere();
	if (!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if (ss.CollisionSphere(ps)) 
	{
		m_bDead = true;
		shot.SetShow(false);
	}
}