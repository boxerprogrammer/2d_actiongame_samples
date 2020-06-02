#pragma once

///SE�p�N���X�B
class SoundEffect
{
private:
	int _handle;
public:
	///�t�@�C�������w�肵�ă��[�h���Ă����R���X�g���N�^
	///@param filepath �t�@�C���p�X
	SoundEffect(const wchar_t* filepath);
	SoundEffect(const char* filepath);
	///�T�E���h�n���h�����w�肷��R���X�g���N�^
	///�N���[���̍쐬���ɗ��p
	///@param handle �T�E���h�n���h��
	SoundEffect(const int handle);

	///�Ƃ肠�����I�u�W�F�N�g�͍쐬���邪�Ȃɂ����Ȃ��R���X�g���N�^
	SoundEffect(void);

	///��{�I�Ƀf�X�g���N�g���ɂ̓n���h����������Ă��
	~SoundEffect(void);

	void Load(const char* filepath);

	///�T�E���h���Đ�����
	void Play();
};

