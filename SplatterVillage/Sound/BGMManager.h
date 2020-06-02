#pragma once

enum BGMID{
	bid_title,
	bid_opening1,
	bid_opening2,
	bid_before_stage,
	bid_stage_1,
};

///BGM管理クラス
///基本、BGMは一度にひとつずつしか鳴らないのでシングルトンにもたせます
class BGMManager
{
private:
	
	int _fadeCount;
	int _currentVolume;
	int _reduction;//1フレごとに減る量(デフォルトは0)
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
	///BGMを再生する
	///@param filename BGMファイルパス
	void Play(const char* filename);

	///BGMを再生する
	///@param bgmid BGM識別ID
	void Play(BGMID bgmid);

	///再生中のBGMを停止する
	void Stop();

	///再生中のBGMをフェードアウトする
	///@param frames 何フレでフェードアウトしたいのか
	void FadeOut(int frames=60);

	void Update();	

};

