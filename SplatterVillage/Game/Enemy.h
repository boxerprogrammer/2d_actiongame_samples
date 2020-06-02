#pragma once

#include<map>
#include<string>
#include"Character.h"
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"

#include"BloodSplash.h"

#include"PictInfo.h"


class EnemyFactory;
class Camera;
class Model3D;
class Player;
class Shot;



///敵キャラ基底クラス
class Enemy{
protected:
	int _crashTimer;
	bool _isDead;
	PositionInt2D _initialPos;
	PositionInt2D _pos;
	Vector2D<int> _vel;
	Rect _rect;
	int _life;
	const Camera& _camera;
	const EnemyFactory& _factory;
	void CrashEffect();
public:
	Enemy(const Camera& camera,const PositionInt2D& position,const EnemyFactory& factory);
	virtual ~Enemy();

	void SetLife(int life){
		_life=life;
	}

	const Rect& HitRect()const{
		return _rect;
	}

	bool IsDead()const{return _isDead;}

	
	virtual void OnDamage(Shot*);
	virtual void OnDead(Shot*);
	virtual void OnWakeUp(){}

	///通常以外の追加アクションのセット
	virtual void SetAppearAction(int handle,int num,int interval=6){}
	virtual void SetDisppearAction(int handle){}
	virtual void SetDeathAction(int handle){}

	virtual void SetPosition(const PositionInt2D& );

	///現在位置の初期化
	virtual void InitPosition(const PositionInt2D& );
	
	virtual void Disappear(){}

	///蘇り
	virtual void Resurrect(){}

	///
	virtual const PositionInt2D& Position()const{
		return _pos;
	}
		
	///更新
	virtual void Update()=0;

	///描画
	virtual void Draw()=0;
};



///2D敵の制御、表示を行うよ
class Enemy2D : public Enemy
{
	friend EnemyFactory;
	protected:
		
		Enemy2D(const Camera& camera,int handle,const EnemyFactory&,bool animTurn=false);
		Enemy2D(const Camera& camera,int handle,int x,int y,const EnemyFactory&,bool animTurn=false);
		///アニメーションありの場合のスタンダード
		///@param camera カメラへの参照
		///@param handle 画像ハンドル
		///@param w アニメーション１枚あたりの幅
		///@param h アニメーション１枚あたりの高さ
		///@param interval アニメーション１枚の表示時間(デフォルト6なのは適当)
		Enemy2D(const Camera& camera,int handle,int w,int h,int num,const EnemyFactory& f,int interval=6,bool animTurn=false);
		int _w,_h;
		std::map<int,PictInfo> _handles;
		int _key;
		bool _isAnim;
		int _interval;
		int _num;
		int _idx;
		int _waitTimer;
		BloodSplash* _blood;//血しぶきエフェクトへの弱参照
		HitEffect* _hiteffect;//ヒットエフェクトへの弱参照
		bool _turn;
		const bool _animTurn;//アニメーションのターン再生
	public:
		
		virtual ~Enemy2D(void);
		
		///更新
		virtual void Update();

		///描画
		virtual void Draw();

		virtual void OnDead(Shot*);
};

///3D敵表示クラス
class Enemy3D : public Enemy
{
	friend EnemyFactory;
	private:
		Enemy3D(const Camera& camera,int handle,const EnemyFactory& f);
		Enemy3D(const Camera& camera,int handle,int x,int y,const EnemyFactory& f);
		
		ScopedPtr<Model3D> _model;
		
		void UpdateAppear();
		void UpdateNeutral();
		void (Enemy3D::*_updater)();
	public:
		
		virtual ~Enemy3D(void);
		
		//場所の変更
		virtual void SetPosition(const PositionInt2D& );

		///更新
		virtual void Update();

		///描画
		virtual void Draw();
};


///敵種別
enum EnemyType{
	enm_enemy,
	enm_dokuro,//ドクロ
	enm_crow,//烏
	enm_eyeball,//目玉
	enm_lipman//クチビルゲ
};

///敵SE種別
enum EnemySeType{
	none,
	se_dokuro_appear,
};

///敵キャラを生み出すファクトリクラス
///ファクトリだけどシングルトンクラスではない
///中身的には、グラフィクスハンドルについてのFlyweightになっている。
class EnemyFactory{
	public:
		EnemyFactory(const Camera&,const Player& player);
		~EnemyFactory();

		///ファイル名を受け取り、そこから敵オブジェクトを返す
		///@note 同名ファイルを指定された場合は、その名前を元にロード済みの画像を使う
		Enemy* Create(const char* name ,EnemyType type=enm_enemy);

		///発生時のアクションに関連するファイルを登録する
		///@param enemy 登録したい敵オブジェクト
		///@param filename ファイル名
		void SetAppearAction(Enemy* enemy,const char* filename);

		///消滅時のアクションに関連するファイルを登録する
		///@param enemy 登録したい敵オブジェクト
		///@param filename ファイル名
		void SetDisappearAction(Enemy* enemy,const char* filename);

		///死亡時のアクションに関連するファイルを登録する
		///@param enemy 登録したい敵オブジェクト
		///@param filename ファイル名
		void SetDeathAction(Enemy* enemy,const char* filename);

		void PlaySE(EnemySeType type) const;

		void Destroy();
	private: 
		//コピー・代入禁止
		EnemyFactory(const EnemyFactory&);
		EnemyFactory& operator=(const Enemy&);
		typedef std::map<std::string,int> namemap_t;
		namemap_t _handles;
		std::map<EnemySeType,int> _seMap;
		const Camera& _camera;
		const Player& _player;
		
};

