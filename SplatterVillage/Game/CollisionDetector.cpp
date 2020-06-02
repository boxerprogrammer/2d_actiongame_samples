#include "CollisionDetector.h"
#include <DxLib.h>
#include"Player.h"
#include"Enemy.h"
#include"Shot.h"
#include"GameObject.h"
#include"../Sound/SoundEffect.h"

CollisionDetector::CollisionDetector(void)
{
	_hitSE.Reset(new SoundEffect("snd/osb/hit.wav"));
}


CollisionDetector::~CollisionDetector(void)
{
}

///判定対象の敵矩形を積みます
void
CollisionDetector::StoreLadder(Ladder* ladder){
	_ladders.push_back(ladder);
}

void 
CollisionDetector::ClearLadder(){
	_ladders.clear();
}
void
CollisionDetector::CheckHitLadders(Player& player){
	ladders_t::iterator it=_ladders.begin();
	ladders_t::iterator itEnd=_ladders.end();
	for(;it!=itEnd;++it){
		if(IsCollided(player.HitRect(),(*it)->Collider())){
			player.OnHitLadder(**it);
			return;
		}
	}
	player.OnNotHitLadder();
}



///判定対象の敵矩形を積みます
void
CollisionDetector::StoreDetectableEnemyTarget(Enemy* enemy){
	_enemies.push_back(enemy);
}

void 
CollisionDetector::ClearDetectableEnemyTarget(){
	_enemies.clear();
}

void
CollisionDetector::CheckHitEnemies(Player& player){
	enemies_t::iterator it=_enemies.begin();
	enemies_t::iterator itEnd=_enemies.end();
	for(;it!=itEnd;++it){
		if(!(*it)->IsDead() && (*it)->HitRect().IsValid() && IsCollided(player.HitRect(),(*it)->HitRect())){
			player.OnHitEnemy();
			return;
		}
	}
}

void 
CollisionDetector::CheckHitEnemiesAndShot(Shot& topshot){
	enemies_t::iterator itEnd=_enemies.end();
	Shot* shot = topshot.Next();
	while(shot!=NULL){
		enemies_t::iterator it=_enemies.begin();
		for(;it!=itEnd;++it){
			if(IsCollided(shot->HitRect(),(*it)->HitRect())){
				(*it)->OnDamage(shot);
				shot->OnDispose();
				_hitSE->Play();
				return;
			}
		}
		shot = shot->Next();
	}
}

///判定対象の地形矩形を積みます
void 
CollisionDetector::StoreDetectableGeoTarget(Rect& rc){
	_geoRects.push_back(rc);
}
void 
CollisionDetector::ClearDetectableGeoTarget(){
	_geoRects.clear();
}

bool 
CollisionDetector::IsCollided(const Rect& rcA,const Rect& rcB)const{
	if(abs(rcB.center.x-rcA.center.x)>rcA.size.w+rcB.size.w){
		return false;
	}
	if(abs(rcB.center.y-rcA.center.y)>rcA.size.h+rcB.size.h){
		return false;
	}
	return true;
}

void CollisionDetector::CheckGeographAndEnemy(){
	enemies_t::iterator it=_enemies.begin();
	enemies_t::iterator itEnd=_enemies.end();
	for(;it!=itEnd;++it){
		CheckGeograph(**it);
	}
}

void CollisionDetector::CheckGeograph(Enemy& enemy){
	if(enemy.IsDead()){
		return;
	}
	Rect r=enemy.HitRect();
	r.size.h+=4;
	rects_t::iterator it=_geoRects.begin();
	rects_t::iterator itEnd=_geoRects.end();
	for(;it!=itEnd;++it){
		if(IsCollided(r,*it)){
			return;
		}
	}
	enemy.Disappear();
}

void
CollisionDetector::CheckGeograph(Player& player){
	rects_t::iterator it=_geoRects.begin();
	rects_t::iterator itEnd=_geoRects.end();
	for(;it!=itEnd;++it){
		if(IsCollided(player.HitRect(),*it)){
			//下向きのときにのみ
			if(player.Velocity().y>0 && player.LastFootPosY() < it->center.y){
				const Size2D& dent = DentedValue(player.HitRect(),*it);
				player.Adjust(dent,dir_up);//補正方向を指定
				player.OnGround();
				return;
			}else{
				//const Size2D& dent = DentedValue(player.Rect(),*it);
				//player.Adjust(dent);//補正方向を指定
				//player.OnGround();
				//return;
			}
		}
	}
	player.OnAir();
}

void 
CollisionDetector::DisplayDebugInfo(){
	{
		rects_t::iterator it=_geoRects.begin();
		rects_t::iterator itEnd=_geoRects.end();
		for(;it!=itEnd;++it){
			DxLib::DrawBox(it->Left()-_cameraPos.x,
				it->Top()-_cameraPos.y,
				it->Right()-_cameraPos.x,
				it->Bottom()-_cameraPos.y,
				0xFFAAAA,
				FALSE);
		}
	}
	{
		enemies_t::iterator it=_enemies.begin();
		enemies_t::iterator itEnd=_enemies.end();
		for(;it!=itEnd;++it){
			const Rect& rect=(*it)->HitRect();
			DxLib::DrawBox(rect.Left()-_cameraPos.x,
				rect.Top()-_cameraPos.y,
				rect.Right()-_cameraPos.x,
				rect.Bottom()-_cameraPos.y,
				0xAAFFAA,
				FALSE);
		}
	}
}



Size2D 
CollisionDetector::DentedValue(const Rect& rcA,const Rect& rcB )const{
	Size2D ret=Size2D();
	if(rcA.center.x<rcB.center.x){
		ret.w=(rcA.center.x+rcA.size.w)-(rcB.center.x-rcB.size.w);
	}else{
		ret.w=(rcA.center.x-rcA.size.w)-(rcB.center.x+rcB.size.w);
	}

	if(rcA.center.y<rcB.center.y){
		ret.h=(rcA.center.y+rcA.size.h)-(rcB.center.y-rcB.size.h);
	}else{
		ret.h=(rcA.center.y-rcA.size.h)-(rcB.center.y+rcB.size.h);
	}
	return ret;
}