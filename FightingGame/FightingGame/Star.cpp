#include "Star.h"
#include<DxLib.h>
#include"Player.h"

#include<algorithm>

using namespace std;
Star::Star(int handle, Player* player):_handle(handle),_player(player),Enemy(handle,player)
{
	_player->SetEnemy(this);
	_pos.y = 200;
	_pos.x = 160;
	_updater = &Star::BackStep;
	_aiTimer = 60;
	_frame = 0;
	_life = 100;

	_rc.w = 40;
	_rc.h = 40;
	_rc.SetCenter(_pos.x, _pos.y);

	srand(GetTickCount());

	_updatefuncTbl.push_back(&Star::BackStep);
	_updatefuncTbl.push_back(&Star::StepIn);
	_updatefuncTbl.push_back(&Star::Kick);
	_updatefuncTbl.push_back(&Star::Shoot);
	_subupdater = []() {};
	_attackposition = ap_upper;
	_missiles.clear();
	_endTimer = 0;
}


Star::~Star()
{
}
//次の振る舞いを決める
void 
Star::DecideNextBevior() {
	//とにかく一定時間毎にランダムで動きを決めている
	//特にAIなどは搭載していない
	if (--_aiTimer == 0) {
		_frame = 0;
		int next = rand() % _updatefuncTbl.size();
		if (next >= 2) {
			_attackposition = (AttackPosition)(rand() % 3);
			int vx = IsTurn() ? 2 : -2;
			if (Distance() > 50  ) {
				next = 3;
				
				switch (_attackposition) {
				case ap_lower:
				{
					vector<Missile>::iterator it = find_if(_missiles.begin(),_missiles.end(),[](Missile& m){return m.IsDead();});
					if(it==_missiles.end()){
						Missile m(CutGraphInfo(_handle, 226, 566, 4, 4), 2, 2, 4, 4, Position2(_pos.x, _pos.y - 4), Vector2(vx, 0), *this);
						m.Activate();
						_missiles.push_back(m);
					}else{
						it->Resurrect();
						it->SetPosition(_pos.x, _pos.y - 4);
						it->SetVelocity(vx, 0);
					}
				}
				break;
				case ap_middle:
				{
					vector<Missile>::iterator it = find_if(_missiles.begin(), _missiles.end(), [](Missile& m) {return m.IsDead(); });
					if (it == _missiles.end()) {
						Missile m(CutGraphInfo(_handle, 226, 566, 4, 4), 2, 2, 4, 4, Position2(_pos.x, _pos.y - 16), Vector2(vx, 0), *this);
						m.Activate();
						_missiles.push_back(m);
					}else{
						it->Resurrect();
						it->SetPosition(_pos.x, _pos.y - 16);
						it->SetVelocity(vx, 0);
					}
				}
					break;
				case ap_upper:
					vector<Missile>::iterator it = find_if(_missiles.begin(), _missiles.end(), [](Missile& m) {return m.IsDead(); });
					if (it == _missiles.end()) {
						Missile m(CutGraphInfo(_handle, 226, 566, 4, 4), 2, 2, 4, 4, Position2(_pos.x, _pos.y - 28), Vector2(vx, 0), *this);
						m.Activate();
						_missiles.push_back(m);
					}else{
						it->Resurrect();
						it->SetPosition(_pos.x, _pos.y - 28);
						it->SetVelocity(vx, 0);
					}
					break;
				}
			}
			else {
				next = 2;
			}
		}
		_updater=_updatefuncTbl[next];
		_aiTimer = 60;
		
	}
}

//プレイヤーとの距離を返す
unsigned int 
Star::Distance() {
	return abs(_pos.x - _player->Position().x);
}

///震えによるXオフセットを返す
int 
Star::TrembleX() {
	return (_hitStopTimer % 2) * 3;
}


//前進
void 
Star::StepIn() {
	bool turnFlg = IsTurn();
	_drawer = [=]() {
		if ((_frame / 6) % 2 == 0) {
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 0, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
		}
		else {
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 40, 168, 32, 40, 16, 40, 2.0f, 0, _handle, true, turnFlg);
		}
	};
	_drawer();
	if (_hitStopTimer>0) {
		return;
	}
	++_frame;
	if (Distance() > 10) {
		_pos.x += turnFlg ? 2 : -2;
	}
	DecideNextBevior();
}

//後退
void 
Star::BackStep() {
	bool turnFlg = IsTurn();
	_drawer = [=]() {
		if ((_frame / 6) % 2 == 0) {
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 0, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
		}
		else {
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 40, 168, 32, 40, 16, 40, 2.0f, 0, _handle, true, turnFlg);
		}
	};
	_drawer();
	if (_hitStopTimer>0) {
		return;
	}
	++_frame;
	if (Distance() > 10) {
		_pos.x += turnFlg ? -2 : +2;
	}
	DecideNextBevior();
}

//キック
void 
Star::Kick() {
	bool turnFlg = IsTurn();

	_drawer = [=]() {
		switch (_attackposition) {
		case ap_lower:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 280, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		case ap_middle:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 424, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		case ap_upper:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 352, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		}
	};
	_drawer();
	if (_hitStopTimer>0) {
		return;
	}

	DecideNextBevior();
}

//弾
void 
Star::Shoot() {
	bool turnFlg = IsTurn();
	_drawer = [=]() {
		switch (_attackposition) {
		case ap_lower:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 240, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		case ap_middle:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 200, 168, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		case ap_upper:
			DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 168, 168, 32, 40, 16, 40, 2.0f, 0, _handle, true, turnFlg);
			break;
		}
	};
	_drawer();
	if (_hitStopTimer>0) {
		return;
	}

	DecideNextBevior();
}
std::vector<Missile>&
Star::GetMissiles() {
	return _missiles;
}
///敵の状態を更新し、画像を描画する
void 
Star::Update() {
	(this->*_updater)();
	_pos.x = max(min(310, _pos.x), 10);
	_rc.SetCenter(_pos.x, _pos.y - 20);
	_rc.Draw();
	_subupdater();

	std::vector<Missile>::iterator it = _missiles.begin();
	for(;it!=_missiles.end();){
		if(it->IsDead()){
			it=_missiles.erase(it);
		}else{
			++it;
		}
	}

}

///敵を覆う矩形を返す
Rect& 
Star::GetRect() {
	return _rc;
}

///矩形が当たった時に発生するイベント(BattleSceneからコールされる)
void 
Star::OnCollided(RectType rt) {
	if (_life <= 0)return;
	void(Star::*nextupdate)();
	nextupdate = &Star::BackStep;
	if (rt == rt_attack) {
		_hitStopTimer = 30;
		_life -= 10;
		if (_life<=0) {
			nextupdate = &Star::PauseForEnd;
		}
		_subupdater = [&, nextupdate]() {
			if (--_hitStopTimer == 0) {
				_updater = nextupdate;
				_subupdater = []() {};
			}
		};
	}
	else {
		_updater = &Star::BackStep;
	}
}

const Position2& 
Star::Position() const {
	return _pos;
}
bool 
Star::IsTurn()const {
	return _pos.x < _player->Position().x;
}

void 
Star::PauseForEnd() {
	if (--_endTimer <= 0) {
		if (_life > 0) {
			_updater = &Star::Kick;
		}
		else {
			_updater = &Star::Dying;
		}
	}
	_drawer();
}

void
Star::Dying() {
	DxLib::DrawRectRotaGraph2(_pos.x * 2 , _pos.y * 2, 460, 168, 40, 40, 20, 40, 2.0f, 0, _handle, true, IsTurn());
}