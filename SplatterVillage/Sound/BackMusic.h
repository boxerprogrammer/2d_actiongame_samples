#pragma once

///BGM用クラス
class BackMusic
{
private:
	int _handle;
public:
	BackMusic(const char* fileName);
	BackMusic(void);
	~BackMusic(void);
	///BGMを鳴らす。
	void Play();

	///BGMを止める。
	void Stop();
};

