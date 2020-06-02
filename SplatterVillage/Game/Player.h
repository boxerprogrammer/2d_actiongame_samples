#pragma once
#include"Character.h"
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"
class SoundEffect;
class Camera;
class Model3D;
class Ladder;

enum PlayerState{
	pst_armor,
	pst_naked,
	pst_dying,//死に中
	pst_dead//死
};

class Player{

private:
	ScopedPtr<SoundEffect> _deadSE;//断末魔SE
	ScopedPtr<SoundEffect> _throwSE;//なげSE
	ScopedPtr<SoundEffect> _jumpSE;//ジャンプSE
	PlayerState _pst;
	PositionInt2D _position;
	PositionInt2D _lastPosition;
	ScopedPtr<Model3D> _model;//標準の鎧つきモデル
	ScopedPtr<Model3D> _modelN;//nakedのN。要は鎧が剥がれた際のモデルですね。
	ScopedPtr<Model3D> _modelD;//deadのD。要は死のモデルですね。
	Model3D* _currentModel;//モデルへの弱参照(このポインタを解放してはならない！！)
	Rect _rect;
	VectorInt2D _velocity;
	int _crashTimer;//ダメージエフェクト時間
	int _ultimateTimer;//無敵時間

	bool _isPressShotButton;

	//プレイヤーステータス
	bool _isGround;//地面か？
	bool _isMoved;//動作中か？
	bool _isPrejumping;//ジャンプ予備動作中か？
	bool _isJumping;//ジャンプ中か？
	bool _isLanding;//着地中か？
	bool _isThrowing;//武器投げ中か？
	bool _isCrouch;//しゃがみ中か？
	bool _isClear;//クリア動作中か？

	void OnDamaged();
	void OnDead();
	void OnPrejump();
	void OnJump();
	void OnStop();
	void OnLanding();
	void OnLaddering();//梯子のぼりイベント
	void CrashEffect(const Camera& camera);

	void DisplayDebugInfo(const Camera& camera);
	
public:

	Player();
	~Player();
	const PositionInt2D& CurrentPos() const;
	const VectorInt2D& Velocity() const;
	CharacterDirection Direction()const;
	bool IsMoved() const{
		return _isMoved;
	}

	bool IsStanding()const;
	bool IsCrouching()const{return _isCrouch;}
	bool IsNotJumping()const;
	void Draw(const Camera& camera);

	const PlayerState State() const;
	//着地イベント
	void OnGround();
	void OnAir();
	
	//敵ヒットイベント
	void OnHitEnemy();
	void OnClear();
	const Rect& HitRect() const;
	const int LastFootPosY() const;
	void KillMe();
	void Adjust(const Size2D& size,CharacterDirection dir=dir_none);

	//ハシゴフラグ
	bool _isOverlappedLadder=false;
	bool _isLaddering=false;
	//ハシゴイベント
	void OnHitLadder(Ladder& ladder);
	void OnNotHitLadder();
	bool IsLaddering();
	bool IsOverlappedLadder();

};



