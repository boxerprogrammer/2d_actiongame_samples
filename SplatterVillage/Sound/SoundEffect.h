#pragma once

///SE用クラス。
class SoundEffect
{
private:
	int _handle;
public:
	///ファイル名を指定してロードしておくコンストラクタ
	///@param filepath ファイルパス
	SoundEffect(const wchar_t* filepath);
	SoundEffect(const char* filepath);
	///サウンドハンドルを指定するコンストラクタ
	///クローンの作成等に利用
	///@param handle サウンドハンドル
	SoundEffect(const int handle);

	///とりあえずオブジェクトは作成するがなにもしないコンストラクタ
	SoundEffect(void);

	///基本的にデストラクト時にはハンドルも解放してやる
	~SoundEffect(void);

	void Load(const char* filepath);

	///サウンドを再生する
	void Play();
};

