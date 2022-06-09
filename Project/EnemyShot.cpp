#include "EnemyShot.h"

/**
 * �R���X�g���N�^
 *
 */
CEnemyShot::CEnemyShot() :
m_pMesh(NULL),
m_Pos(0,0,0),
m_Spd(0,0,0),
m_bShow(false),
m_RotY(0){
}

/**
 * �f�X�g���N�^
 *
 */
CEnemyShot::~CEnemyShot(){
}

/**
 * ������
 *
 */
void CEnemyShot::Initialize(){
	m_Pos = Vector3(0,0,0);
	m_Spd = Vector3(0,0,0);
	m_bShow = false;
	m_RotY = 0;
}

/**
 * ����
 *
 */
void CEnemyShot::Fire(const Vector3& p,const Vector3& s){
	m_Pos = p;
	m_Spd = s;
	m_bShow = true;
	m_RotY = 0;
}

/**
 * �X�V
 *
 */
void CEnemyShot::Update(){
	// ��\��
	if(!m_bShow)
	{
		return;
	}
	// ���x�𗘗p�����ړ�
	m_Pos += m_Spd;
	// ��ʊO�ŏ���
	float inflate = 1.0f;
	if (m_Pos.x < -FIELD_HALF_X - inflate || FIELD_HALF_X + inflate < m_Pos.x ||
		m_Pos.z < -FIELD_HALF_Z - inflate || FIELD_HALF_Z + inflate < m_Pos.z) 
	{
		m_bShow = false;
	}
	// �e����]
	m_RotY += MOF_ToRadian(10);
	
}

/**
 * �`��
 *
 */
void CEnemyShot::Render(){
	if(!m_bShow)
	{
		return;
	}
	//��]�𔽉f
	CMatrix44 matWorld;
	
	//���W��ݒ�
	matWorld.RotationY(m_RotY);
	matWorld.SetTranslation(m_Pos);
	//�`��
	m_pMesh->Render(matWorld);
}

/**
 * �f�o�b�O�\��
 *
 */
void CEnemyShot::RenderDebug(void){
	if(!m_bShow)
	{
		return;
	}
	
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(1,0,0,0.3f));
}

/**
 * �e�̔z�񂩂��\���̂��̂���������
 *
 */
CEnemyShot* CEnemyShot::FindAvailableShot(CEnemyShot* shot,int smax){
	for (int i = 0; i < smax; i++)
	{
		if (!shot[i].GetShow()) 
		{
			return &shot[i];
		}
	}
	return NULL;
}