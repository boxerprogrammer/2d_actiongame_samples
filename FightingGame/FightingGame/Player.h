#pragma once

#include"CollidableActor.h"
#include"CommandInput.h"

#include<map>
#include<vector>
#include<functional>


struct KeyState;

enum ActionState {
	as_neutral,
	as_jump,
	as_crouch,
	as_tiltcrouch,
	as_punch,//パンチ
	as_kick//キック
};



class Hadoken;
class Enemy;
class Player : public CollidableActor
{

private:
	//入力情報
	//あとでincludeは消す
	CommandInput _input;
	ActionCommand _lastcmd;
	//再生情報[値]になります。
	struct Sequence {
		Sequence():pos(),duration(0),iv(){}
		Sequence(Position2 p, int dur,float vy=0.0f):iv() {
			pos = p;
			duration = dur;
			iv.y = vy;
			size.w = 40;
			size.h = 40;
			centerpos.x = size.w/2;
			centerpos.y = 40;
			_initfunction = []() {};
			_predrawfunction = []() {};
			_postdrawfunction = []() {};
		}
		Sequence(Position2 p,Size s, int dur, float vy = 0.0f) : iv(){
			pos = p;
			duration = dur;
			iv.y = vy;
			size.w = s.w;
			size.h = s.h;
			centerpos.x = size.w/2;
			centerpos.y = 40;
			_initfunction = []() {};
			_predrawfunction = []() {};
			_postdrawfunction = []() {};

		}
		Position2 pos;//左上座標
		Size size;
		Position2 centerpos;//アクション中心点
		int duration;//表示継続時間
		Vector2 iv;//初速度
		std::vector<HitRect> rects;//当たり判定用
		std::function<void(void)> _initfunction;//シーケンス切替時に呼び出される関数オブジェクト
		std::function<void(void)> _predrawfunction;//毎フレーム描画前に呼び出される関数オブジェクト
		std::function<void(void)> _postdrawfunction;//毎フレーム描画後に呼び出される関数オブジェクト

		///シーケンス切替時に一度だけ関数オブジェクトを呼び出す
		void FirstExecute() {
			_initfunction();
		}
		///シーケンス切替時に一度だけ関数オブジェクトを呼び出す
		void PreDrawExecute() {
			_predrawfunction();
		}
		void PostDrawExecute() {
			_postdrawfunction();
		}

		Sequence& SetInitialVector(Vector2& v){
			iv=v;
			return *this;
		}

		///シーケンス切替時に実行される関数をセットする
		///@param f シーケンス切替時に実行される関数
		Sequence& SetInitialFunction(std::function<void(void)> f) {
			_initfunction = f;
			return *this;
		}

		///毎フレーム実行される関数をセットする
		///@param f シーケンス切替時に実行される関数
		Sequence& SetPreDrawFunction(std::function<void(void)> f) {
			_predrawfunction = f;
			return *this;
		}
		Sequence& SetPostDrawFunction(std::function<void(void)> f) {
			_postdrawfunction = f;
			return *this;
		}

		///アクション中心点の設定
		Sequence& SetActionCenter(int x, int y) {
			centerpos.x = x;
			centerpos.y = y;
			return *this;
		}

		///あたり矩形を登録していく
		Sequence& AddRects(HitRect& hrc) {
			rects.push_back(hrc);
			return *this;
		}

		///当たり矩形の登録(オーバーロード)
		Sequence& AddRects(int x,int y , int w, int h ,RectType rt,CollidableActor* ca) {
			rects.push_back(HitRect(x,y,w,h,rt,ca));
			return *this;
		}
	};

	Hadoken* _hadoken = nullptr;


	Rect _baseCollider;

	std::map<InputId, std::vector<Player::Sequence>> _actions;//アクション情報

	////アクションデータの作成
	void CreateActionsData();
	void CreateAction(char direction,char button,Sequence s);
	void CreateAction(char direction, char button, unsigned char command ,Sequence s);

	//入力情報からInputIdを返す
	InputId GetInputIdFromKeyState(const KeyState& key);

	//現在再生中のアクション
	std::map<InputId, std::vector<Player::Sequence>>::iterator _currentActionIt;
	//現在再生中のシーケンス
	std::vector<Player::Sequence>::iterator _currentSequenceIt;
	bool _lastTurn;//技開始時のターンフラグ
	bool _jumpPrepare;
	int _attackTimer;//攻撃時間
	ActionState _currentActionState;
	Enemy* _enemy;//エネミーへの参照
	float _velY;//Y方向速度
	float _velX;//X方向速度
	int _walkFrame;//歩いてるフレーム(歩きアニメーション用)
	int _handle;//画像ハンドル
	int _jumpSE;//ジャンプのSE
	Position2 _pos;//現在座標
	int _hitStopTimer=0;
	int _effectFrame = 0;

	int _life;//ライフ

	void GotoAction(const KeyState& key);

	bool IsTurn()const;
	
	//ニュートラル
	void Neutral(const KeyState& key);
	
	//ジャンプ
	void Aerial(const KeyState& key);

	//しゃがみ
	void Crouch(const KeyState& key);

	//攻撃
	void Attack(const KeyState& key);

	//移動
	void Move(const KeyState& key);

	//終了処理用
	void PauseForEnd(const KeyState&);

	//死亡
	void Dying(const KeyState& key);

	//メンバ関数ポインタ
	void (Player::*_updater)(const KeyState& key);
	Rect _rc;

	void ShootHadoken();

	std::function<void(void)> _drawer;
	int _endTimer;
public:
	Player(int handle);
	~Player();
	void Update(const KeyState& key);
	void Update();
	const Position2& Position()const { return _pos; }
	void SetEnemy(Enemy* enemy);
	const Rect& GetRect()const;
	///@note 参照でなくて、ポインタを返してるのは
	///ヌルポインタを返す必要がある場合があるため
	std::vector<HitRect>* GetCurrentRects();
	void OnCollided(RectType rc);

	//演出のためのポーズ
	void PauseForRendering();

	int GetLife()const { return _life; }

	Hadoken* GetMissile() { return _hadoken; }
};

