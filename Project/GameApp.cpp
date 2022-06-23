/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"
#include	"Stage1.h"
#include	"Boss.h"

CCamera				gCamera;
CDirectionalLight	gLight;
CPlayer				gPlayer;

#define				ENEMY_COUNT			(20)
CEnemy				gEnemyArray[ENEMY_COUNT];
#define				ENEMYSHOT_COUNT		(200)
CEnemyShot			gShotArray[ENEMYSHOT_COUNT];
CMeshContainer		gEnemyShotMesh;
CBoss				gBoss;
CStage				gStage;
bool				gbEnemyDestroyed = false;
bool				gbClear = false;
bool				gbDebug = false;

CVector3			gCameraPosition;
CVector3			gTargetPosition;
CVector3			gUpVector;


/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");
	gCamera.SetViewPort();
	gCameraPosition = Vector3(0, 6.0f, -17.0f);
	gTargetPosition = Vector3(0, 0, -10);
	gUpVector = Vector3(0, 1, 0);
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);

	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	gPlayer.Load();

	gStage.Load();
	if (!gEnemyShotMesh.Load("eshot.mom"))
	{
		return false;
	}

	gBoss.Load();

	gPlayer.Initialize();

	gStage.Initialize(&gStg1EnemyStart);

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Initialize();
	}
	for (int i = 0;i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetMesh(&gEnemyShotMesh);
	}
	gBoss.Initialize();
	
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	gStage.Update(gEnemyArray, ENEMY_COUNT);
	gPlayer.Update();

	for(int i = 0; i < ENEMY_COUNT; i++) 
	{
		gEnemyArray[i].SetTargetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray, ENEMYSHOT_COUNT);
	}

	gBoss.SetTargetPos(gPlayer.GetPosition());
	gBoss.Update(gShotArray, ENEMYSHOT_COUNT);
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update();
	}
	for (int i = 0; i < ENEMY_COUNT; i++) 
	{
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}
	gPlayer.CollisionBoss(gBoss);
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gPlayer.CollisionEnemyShot(gShotArray[i]);
	}
	if (!gbEnemyDestroyed && gStage.IsAllEnemy())
	{
		bool btmp = true;
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (gEnemyArray[i].GetShow())
			{
				btmp = false;
			}
		}
		gbEnemyDestroyed = btmp;
		if (gbEnemyDestroyed)
		{
			gBoss.Start();
			gBoss.SetTargetPos(gPlayer.GetPosition());
			gBoss.Update(gShotArray, ENEMYSHOT_COUNT);
		}
	}

	if (!gbClear && gbEnemyDestroyed && !gBoss.GetShow())
	{
		gbClear = true;
	}
	
	if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		gbDebug = ((gbDebug ? false : true));
	}
	
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && (gPlayer.IsDead() || gbClear))
	{
		gbEnemyDestroyed = false;
		gbClear = false;
		gPlayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].Initialize();
		}
		for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
		{
			gShotArray[i].Initialize();
		}
		gBoss.Initialize();
	}

	float posX = gPlayer.GetPosition().x * 0.4f;
	CVector3 cpos = gCameraPosition;
	CVector3 tpos = gTargetPosition;
	CVector3 vup = gUpVector;
	cpos.x = posX;
	tpos.x = posX;
	vup.RotationZ(gPlayer.GetPosition().x / FIELD_HALF_X * MOF_ToRadian(10.0f));
	gCamera.LookAt(cpos, tpos, vup);
	gCamera.Update();
	return TRUE;
}

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	//�k�x�o�b�t�@
	g_pGraphics->SetDepthEnable(TRUE);

	gStage.Render();

	gPlayer.Render();

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}
	gBoss.Render();
	for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
	{
		gShotArray[i].Render();
	}
	if (gbDebug) 
	{
		gPlayer.RenderDebug();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebug();
		}
		gBoss.RenderDebug();
		for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
		{
			gShotArray[i].RenderDebug();
		}
		CMatrix44 matWorld;
		matWorld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1, 1, 1, 0.4f));
	}

	g_pGraphics->SetDepthEnable(FALSE);

	if (gbDebug)
	{
		gStage.RenderDebugText();
		gPlayer.RenderDebugText();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	if (gPlayer.IsDead())
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "�Q�[���I�[�o�[�@�F�@Enter�L�[�ł�����x�ŏ�����");
	}
	else if (gbClear)
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "�Q�[���N���A�@:�@Enter�L�[�ł�����x�ŏ�����");
	}
	
	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gStage.Release();
	gPlayer.Release();
	gEnemyShotMesh.Release();
	gBoss.Release();
	return TRUE;
}