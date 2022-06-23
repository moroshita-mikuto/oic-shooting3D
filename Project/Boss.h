#pragma once

#include "GameDefine.h"
#include "Enemy.h"

#define BOSS_PARTS_MAX (3)

class CBoss{
private:
	CMeshContainer		m_Mesh;			//�{�X�{�̂̃��b�V��
	CMeshContainer		m_PartsMesh;	//���͂̃p�[�c�̃��b�V��
	Vector3				m_Pos;			//���W
	Vector3				m_Rot;			//��]�p�x�i���W�A���j
	CMatrix44			m_matWorld;		//�S�̂̃��[���h�}�g���N�X
	bool				m_bShow;		//�\�����

	int					m_HP;			//�̗�
	int					m_ShotWait;		//�e���˂̑҂�����
	Vector3				m_TargetPos;	//�e��]�A�j���[�V�����I����Ɍ������W
	float				m_AnimTime;		//��]�A�j���[�V�����̌o�ߎ���

	CEnemy				m_Parts[BOSS_PARTS_MAX];	//���͂̃p�[�c�Ǘ��pCEnemy�̃C���X�^���X

public:
	CBoss();
	~CBoss();
	
	/**
	 * �ǂݍ���
	 *
	 */
	bool Load();

	/**
	 * ������
	 *
	 */
	void Initialize();

	/**
	 * �J�n
	 *
	 */
	void Start();

	/**
	 * �X�V
	 *
	 */
	void Update(CEnemyShot* shot,int smax);

	/**
	 * �_���[�W����
	 *
	 */
	void Damage(int dmg);

	/**
	 * �`��
	 *
	 */
	void Render();

	/**
	 * �f�o�b�O�`��
	 *
	 */
	void RenderDebug();

	/**
	 * �\����Ԏ擾
	 *  �߂�l��true�Ȃ�\�����Afalse�Ȃ��\��
	 */
	bool GetShow(){ return m_bShow; }
	
	/**
	 * �Փˋ���Ԃ�
	 *
	 */
	CSphere GetSphere(){ return CSphere(m_Pos,3.1f); }
	
	/**
	 * �^�[�Q�b�g���W�̐ݒ�
	 *
	 */
	void SetTargetPos(const Vector3& t){ m_TargetPos = t; }
	
	/**
	 * i�Ԗڂ̃p�[�c���擾
	 *
	 */
	CEnemy& GetParts(int i){ return m_Parts[i]; }

	/**
	 * �\�����̃p�[�c�̐���Ԃ�
	 *
	 */
	int GetPartsCount();

	/**
	 * ���
	 *
	 */
	void Release();
	
	/**
	 * ����̕����։�]����
	 *
	 */
	void RotateTarget();
	
	/**
	 * �S���ʂւ̒e����
	 *
	 */
	void ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt);
	
	/**
	 * �p�[�c�X�V
	 *
	 */
	void UpdateParts(int idx, CEnemyShot* shot, int smax, bool bShot);
};