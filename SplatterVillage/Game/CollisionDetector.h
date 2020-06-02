#pragma once
#include<vector>
#include "../Geometry/geometry.h"
#include "../system/scoped_ptr.h"
class Camera;
class Player;
class Enemy;
class Shot;
class SoundEffect;
class Ladder;
///�����蔻��S��
class CollisionDetector
{
private:
	typedef std::vector<Rect> rects_t;
	rects_t _geoRects;

	typedef std::vector<Enemy*> enemies_t;
	ScopedPtr<SoundEffect> _hitSE;
	enemies_t _enemies;
	
	typedef std::vector<Ladder*> ladders_t;
	ladders_t _ladders;

	PositionInt2D _cameraPos;
	CollisionDetector();
	CollisionDetector(const CollisionDetector& );
	CollisionDetector& operator=(const CollisionDetector&);
public:
	~CollisionDetector(void);
	void SetCameraPos(const PositionInt2D& pos){
		_cameraPos = pos;
	}
	bool IsCollided(const Rect& rcA, const Rect& rcB) const;
	Size2D DentedValue(const Rect& rcA, const Rect& rcB) const;
	static CollisionDetector& Instance(){
		static CollisionDetector instance;
		return instance;
	}
	
	void CheckHitEnemiesAndShot(Shot& topshot);

	//����Ώۂ̓G��`��ς݂܂�
	void StoreDetectableEnemyTarget(Enemy* enemy);
	void ClearDetectableEnemyTarget();
	void CheckHitEnemies(Player& player);

	///����Ώۂ̒n�`��`��ς݂܂�
	void StoreDetectableGeoTarget(Rect& rc);
	void ClearDetectableGeoTarget();
	void CheckGeograph(Player& player);
	void DisplayDebugInfo();

	void CheckGeographAndEnemy();
	void CheckGeograph(Enemy& enemy);

	///����Ώۂ̃C�x���g�I�u�W�F�N�g��`��ς݂܂�
	void StoreDetectableEventTarget();
	void ClearDetectableEventTarget();

	//�͂����n�̓����蔻��
	void StoreLadder(Ladder* ladder);
	void ClearLadder();
	void CheckHitLadders(Player& player);

};

