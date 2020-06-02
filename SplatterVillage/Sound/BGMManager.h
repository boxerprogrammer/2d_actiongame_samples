#pragma once

enum BGMID{
	bid_title,
	bid_opening1,
	bid_opening2,
	bid_before_stage,
	bid_stage_1,
};

///BGM�Ǘ��N���X
///��{�ABGM�͈�x�ɂЂƂ�������Ȃ��̂ŃV���O���g���ɂ������܂�
class BGMManager
{
private:
	
	int _fadeCount;
	int _currentVolume;
	int _reduction;//1�t�����ƂɌ����(�f�t�H���g��0)
	BGMManager(void);
	BGMManager(const BGMManager&);
	BGMManager& operator=(const BGMManager&);
	void (BGMManager::*_updater)();
	void Normal();
	void Fade();
public:
	~BGMManager(void);
	static BGMManager& Instance(){
		static BGMManager instance;
		return instance;
	}
	///BGM���Đ�����
	///@param filename BGM�t�@�C���p�X
	void Play(const char* filename);

	///BGM���Đ�����
	///@param bgmid BGM����ID
	void Play(BGMID bgmid);

	///�Đ�����BGM���~����
	void Stop();

	///�Đ�����BGM���t�F�[�h�A�E�g����
	///@param frames ���t���Ńt�F�[�h�A�E�g�������̂�
	void FadeOut(int frames=60);

	void Update();	

};

