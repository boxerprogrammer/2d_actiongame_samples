#pragma once

///BGM�p�N���X
class BackMusic
{
private:
	int _handle;
public:
	BackMusic(const char* fileName);
	BackMusic(void);
	~BackMusic(void);
	///BGM��炷�B
	void Play();

	///BGM���~�߂�B
	void Stop();
};

