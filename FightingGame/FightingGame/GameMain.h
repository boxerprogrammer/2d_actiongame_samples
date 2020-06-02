#pragma once
#include"Geometry.h"
//�L�[���
struct KeyState {
	char state[256];//���t���[���̃L�[���
	char lastState[256];//�O�̃t���[���̃L�[���
};




enum CharaType {
	ct_gouki,//���S
	ct_yun,//����
	ct_ryu,//�����E
	ct_urien,//�����A��
	ct_remy,//���~�[
	ct_oro,//�I��
	ct_necro,//�l�N��
	ct_q,//Q
	ct_dudley,//�_�b�h���[
	ct_ibuki,//���Ԃ�
	ct_chunli,//�t��
	ct_elena,//�G���i
	ct_sean,//�V���[��
	ct_makoto,//�܂���
	ct_hugo,//�q���[�S�[
	ct_alex,//�A���b�N�X
	ct_twelve,//�g�D�G����
	ct_ken,//�P��
	ct_yang,//����
	ct_invalid//����
};


class Scene;

class GameMain
{
private:
	Scene* _scene;
	int _wincount;
	Size _windowSize;//�E�B���h�E�T�C�Y
	GameMain();
	GameMain(const GameMain&);
	GameMain& operator=(const GameMain&);
	void Initialize();
	void GameLoop();
public:
	const Size& WindowSize()const;
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}
	///�Q�[���̎��s
	void Run();

	///�V�[���̕ύX
	///@param scene ���̃V�[��
	///@attention �Ăяo������͌��݂̃V�[���I�u�W�F�N�g��
	///�폜����܂��B
	void ChangeScene(Scene* scene);

	///���������擾
	int WinCount()const;

	///���������C���N�������g
	void IncrementWinCount();

	~GameMain();
};


