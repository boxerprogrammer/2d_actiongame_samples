#pragma once
#include"Geometry.h"
//キー情報
struct KeyState {
	char state[256];//現フレームのキー情報
	char lastState[256];//前のフレームのキー情報
};




enum CharaType {
	ct_gouki,//豪鬼
	ct_yun,//ユン
	ct_ryu,//リュウ
	ct_urien,//ユリアン
	ct_remy,//レミー
	ct_oro,//オロ
	ct_necro,//ネクロ
	ct_q,//Q
	ct_dudley,//ダッドリー
	ct_ibuki,//いぶき
	ct_chunli,//春麗
	ct_elena,//エレナ
	ct_sean,//ショーン
	ct_makoto,//まこと
	ct_hugo,//ヒューゴー
	ct_alex,//アレックス
	ct_twelve,//トゥエルヴ
	ct_ken,//ケン
	ct_yang,//ヤン
	ct_invalid//無効
};


class Scene;

class GameMain
{
private:
	Scene* _scene;
	int _wincount;
	Size _windowSize;//ウィンドウサイズ
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
	///ゲームの実行
	void Run();

	///シーンの変更
	///@param scene 次のシーン
	///@attention 呼び出した後は現在のシーンオブジェクトは
	///削除されます。
	void ChangeScene(Scene* scene);

	///勝ち数を取得
	int WinCount()const;

	///勝ち数をインクリメント
	void IncrementWinCount();

	~GameMain();
};


