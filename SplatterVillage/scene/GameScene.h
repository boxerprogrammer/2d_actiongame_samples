#pragma once

//�Q�[���V�[���̊�{�N���X
class SceneManager;
class GameScene
{
public:
	GameScene(void)
	{
	}

	virtual ~GameScene(void)
	{
	}

	virtual void Update(SceneManager& caller)=0;

	///���V�[���ւ̗v�����s��ꂽ��
	///�f�o�O�p�֐�
	///
	bool CheckNextRequest_Debug();
};

