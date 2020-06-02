
#pragma once
#include "Scene.h"
///�^�C�g���V�[��
class TitleScene :	public Scene
{
private:
	int _fadeTimer;//�t�F�[�h�p�^�C�}�[
	int _blinkTimer;//�_�Ń^�C�}�[
	int _waitTimer;//�҂��^�C�}�[
	int _titleH;//�^�C�g�����S�n���h��
	int _pressStartH;//press start button�摜�n���h��
	int _startSE;

	//�P�t���[�����ƂɈ�x���s�����A�b�v�f�[�g�֐�
	void (TitleScene::*_updater)(const KeyState& key);
	void FadeInUpdate(const KeyState& key);
	void NormalUpdate(const KeyState& key);
	void FastBlinkUpdate(const KeyState& key);
	void FadeOutUpdate(const KeyState& key);

public:
	TitleScene();
	~TitleScene();
	void Update(const KeyState& key);
};

