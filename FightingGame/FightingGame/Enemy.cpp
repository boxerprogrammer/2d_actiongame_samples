#include "Enemy.h"
#include<DxLib.h>
#include"Player.h"
Enemy::Enemy(int handle,Player* player):_handle(handle),_player(player)
{
	_player->SetEnemy(this);
	_pos.y = 200;
	_pos.x = 160;
	_updater = &Enemy::Idle;
	_aiTimer = 60;
	_frame = 0;

	_rc.w = 40;
	_rc.h = 40;
	_rc.SetCenter(_pos.x, _pos.y);

	srand(GetTickCount());
	_subupdater = []() {};
	_missiles.clear();

	_life = 100;
}

std::vector<Missile>&
Enemy::GetMissiles() {
	return _missiles;
}

Enemy::~Enemy()
{
}

unsigned int 
Enemy::Distance() {
	return abs(_pos.x - _player->Position().x);
}
bool 
Enemy::IsTurn() const{
	return _pos.x < _player->Position().x;
}

void
Enemy::DecideNextBevior() {
	//とにかく一定時間毎にランダムで動きを決めている
	//特にAIなどは搭載していない
	if (--_aiTimer == 0) {
		_frame = 0;
		int next =rand() % 6;
		switch (next)
		{
		case 0:
			_updater = &Enemy::Idle;
			break;
		case 1:
			_updater = &Enemy::Keep;
			break;
		case 2:
			_updater = &Enemy::StepIn;
			break;
		case 3:
			_updater = &Enemy::BackStep;
			break;
		case 4:
			_updater = &Enemy::DiveUp;
			break;
		case 5:
			_updater = &Enemy::DiveDown;
			break;
		default:
			break;
		}
		_aiTimer = 60;
	}
}

void
Enemy::Idle() {
	bool turnFlg = IsTurn();
	DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 0, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	if (_hitStopTimer>0) {
		return;
	}
	DecideNextBevior();
}

//一定の距離を保つ
void
Enemy::Keep() {
	bool turnFlg = IsTurn();
	DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 0, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	if (_hitStopTimer>0) {
		return;
	}
	DecideNextBevior();
}

//近づく
void 
Enemy::StepIn() {
	bool turnFlg = IsTurn();
	
	if ((_frame / 6) % 2 == 0) {
		DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 0, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	}
	else {
		DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 40, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	}	
	if (_hitStopTimer>0) {
		return;
	}
	++_frame;
	if(Distance() > 10) {
		_pos.x += turnFlg ? 2 : -2;
	}
	DecideNextBevior();
}

//サイコクラッシャー
void
Enemy::DiveUp() {
	bool turnFlg = IsTurn();
	DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2-40, 120, 128, 56, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	if (_hitStopTimer>0) {
		return;
	}
	if(Distance() > 10) {
		_pos.x += turnFlg ? 4 : -4;
	}
	DecideNextBevior();
}

//サイコクラッシャー
void
Enemy::DiveDown() {
	bool turnFlg = IsTurn();
	DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 120, 128, 56, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	if (_hitStopTimer>0) {
		return;
	}
	if (Distance() > 10) {
		_pos.x += turnFlg ? 4 : -4;
	}
	DecideNextBevior();
}

//離れる
void Enemy::BackStep() {
	bool turnFlg = IsTurn();
	if ((_frame / 6) % 2 == 0) {
		DxLib::DrawRectRotaGraph2(_pos.x * 2+ TrembleX(), _pos.y * 2, 0, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	}
	else {
		DxLib::DrawRectRotaGraph2(_pos.x * 2 + TrembleX(), _pos.y * 2, 40, 128, 40, 40, 19, 40, 2.0f, 0, _handle, true, turnFlg);
	}	
	if (_hitStopTimer>0) {
		return;
	}
	++_frame;
	_pos.x += turnFlg ? -2 : 2;
	DecideNextBevior();
}
int
Enemy::TrembleX() {
	return (_hitStopTimer % 2) * 3;
}
void
Enemy::Update() {
	(this->*_updater)();
	_pos.x = max(min(310, _pos.x), 10);
	_rc.SetCenter(_pos.x, _pos.y - 20);
	_rc.Draw();
	_subupdater();
}

Rect&
Enemy::GetRect() {
	return _rc;
}

const Position2&
Enemy::Position()const {
	return _pos;
}

void
Enemy::OnCollided(RectType rt) {
	
	void(Enemy::*nextupdate)();
	nextupdate = &Enemy::BackStep;
	if (rt == rt_attack) {
		_life -= 10;
		_hitStopTimer = 30;
		_subupdater = [&,nextupdate]() {
			if (--_hitStopTimer == 0) {
				_updater = nextupdate;
				_subupdater = []() {};
			}
		};
	}else{
		_updater=&Enemy::BackStep;
	}

}