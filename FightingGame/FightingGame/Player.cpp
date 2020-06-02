#include "Player.h"
#include<DxLib.h>
#include<iostream>//標準出力
#include"GameMain.h"
#include"Enemy.h"
#include"Hadoken.h"

const int GROUND_LINE = 200;

enum DirBit {
	right = 0b0001,
	left = 0b0010,
	up = 0b0100,
	down = 0b1000
};


//ボタン状態
enum ButtonState{
	bs_none,//押してない
	bs_punch,//パンチボタン
	bs_kick//キックボタン
};

int voicesH[5];
int psychoForceH;
int curseH;

void 
Player::CreateActionsData(){
	//パンチ
	//正拳突きA
	CreateAction(ds_right,bs_punch,Sequence(Position2(360,0),10)
		.SetActionCenter(2, 40)
		.AddRects(25,-17,8,6,rt_attack,this)
		.AddRects(14,-6,   24,12,rt_damage,this)
		.AddRects(17, -16, 14, 8, rt_damage, this)
		.AddRects(17, -24, 12, 8, rt_damage, this)
	);

	//顔面狙い
	CreateAction(ds_left, bs_punch, Sequence(Position2(320, 0), 10)
		.SetActionCenter(5,40)
		.AddRects(23,-27,8,10,rt_attack,this));

	//正拳突きB
	CreateAction(ds_rightdown, bs_punch, Sequence(Position2(0, 40), 10)
		.SetActionCenter(14,40)
		.AddRects(20,-15,10,6,rt_attack,this));
	
	//開脚突き
	CreateAction(ds_leftdown, bs_punch, Sequence(Position2(200, 40),Size(41,40), 10)
		.SetActionCenter(10,40)
		.AddRects(27,-14,8,6,rt_attack,this));

	//足首狙い
	CreateAction(ds_down, bs_punch, Sequence(Position2(40, 40), 10)
		.SetActionCenter(18,40)
		.AddRects(20,-4,6,8,rt_attack,this));
	//ジャンプ突き
	CreateAction(ds_up, bs_punch, Sequence(Position2(241, 40), 10)
		.SetActionCenter(18,60)
		.AddRects(14,-32,10,10,rt_attack,this));
	
	//頭上突き-1
	CreateAction(ds_rightup, bs_punch, Sequence(Position2(80, 0), 12,-3.0f));
	//頭上突き-2
	CreateAction(ds_rightup, bs_punch, Sequence(Position2(160, 40), 8)
		.SetActionCenter(8,40)
		.AddRects(18,-3,2,2,rt_attack,this));
	
	CreateAction(ds_leftup, bs_punch, Sequence(Position2(80, 40), 8));//まわし突き-1
	//まわし突き-2
	CreateAction(ds_leftup, bs_punch, Sequence(Position2(120, 40), 10)
		.SetActionCenter(2,40)
		.AddRects(28,-28,6,8,rt_attack,this));

	//キック
	CreateAction(ds_right, bs_kick, Sequence(Position2(80, 80), 15)
		.SetActionCenter(12,40)
		.AddRects(20,-16,16,8,rt_attack,this));//まわし蹴り
	CreateAction(ds_left, bs_kick, Sequence(Position2(40, 80), 15)
		.AddRects(16,-29,8,8,rt_attack,this));//斜め上段狙い
	//足払い
	CreateAction(ds_rightdown, bs_kick, Sequence(Position2(120, 80), 15)
		.SetActionCenter(15,40)
		.AddRects(22,-4,8,8,rt_attack,this));
	//寝技蹴り
	CreateAction(ds_leftdown, bs_kick, Sequence(Position2(280, 80), 15)
		.SetActionCenter(32,40)
		.AddRects(-6,-28,8,8,rt_attack,this));
	//足首狙い
	CreateAction(ds_down, bs_kick, Sequence(Position2(240, 80), 15)
		.SetActionCenter(17,40)
		.AddRects(19,-3,10,6,rt_attack,this));

	//ジャンプ蹴り
	CreateAction(ds_up, bs_kick, Sequence(Position2(200, 80), 15)
		.SetActionCenter(18,60)
		.AddRects(18,-30,10,8,rt_attack,this));

	//蹴り上げ-1
	CreateAction(ds_rightup, bs_kick, Sequence(Position2(160, 0), 5,-2.2f));
	
	//蹴り上げ-2
	CreateAction(ds_rightup, bs_kick, Sequence(Position2(280, 40), 15)
		.SetActionCenter(10, 40)
		.AddRects(19, -27, 8, 10, rt_attack, this));

	//旋風脚-1
	CreateAction(ds_leftup, bs_kick, Sequence(Position2(0, 80), 12,-2.2f));
	
	//旋風脚-2
	CreateAction(ds_leftup, bs_kick, Sequence(Position2(160, 80), 15)
		.AddRects(11,-21,12,12,rt_attack,this));

	//特殊技
	//波動拳-1
	CreateAction(ds_none, bs_none, cmd_hadoken, Sequence(Position2(400, 0), 15)
		.SetActionCenter(4, 40)
		.SetInitialFunction([]() {PlaySoundMem(voicesH[cmd_hadoken], DX_PLAYTYPE_BACK); })
	);
	//波動拳-2
	CreateAction(ds_none, bs_none, cmd_hadoken, Sequence(Position2(436, 0), 20)
		.SetActionCenter(4, 40)
		.SetInitialFunction([this]() {this->ShootHadoken(); })
	);

	//竜巻旋風脚-1
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(360, 90), 12)
		.SetActionCenter(16, 40)
		.AddRects(12,-22,16,8,rt_attack,this)
		.SetInitialVector(Vector2(1,0))
		.SetInitialFunction([]() {PlaySoundMem(voicesH[cmd_tornadokick], DX_PLAYTYPE_BACK); })
	);
	//竜巻旋風脚-2
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(400, 90), 12)
		.SetActionCenter(22, 40)
		.SetInitialVector(Vector2(1, 0))
		.AddRects(-14, -22, 16, 8, rt_attack, this)
	);
	//竜巻旋風脚-3
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(360, 90), 12)
		.SetActionCenter(16, 40)
		.SetInitialVector(Vector2(1, 0))
		.AddRects(12, -22, 16, 8, rt_attack, this)
	);
	//竜巻旋風脚-4
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(400, 90), 12)
		.SetActionCenter(22, 40)
		.SetInitialVector(Vector2(1, 0))
		.AddRects(-14, -22, 16, 8, rt_attack, this)
	);
	//竜巻旋風脚-5
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(360, 90), 12)
		.SetActionCenter(16, 40)
		.SetInitialVector(Vector2(1, 0))
		.AddRects(12, -22, 16, 8, rt_attack, this)
	);
	//竜巻旋風脚-6
	CreateAction(ds_none, bs_none, cmd_tornadokick, Sequence(Position2(400, 90), 12)
		.SetActionCenter(22, 40)
		.SetInitialVector(Vector2(1, 0))
		.AddRects(-14, -22, 16, 8, rt_attack, this)
	);

	//昇竜拳-1
	CreateAction(ds_none, bs_none, cmd_shoryuken, Sequence(Position2(312, 40), 15)
		.SetActionCenter(20, 40)
		.SetInitialFunction([]() {PlaySoundMem(voicesH[cmd_shoryuken], DX_PLAYTYPE_BACK); })
		);
	CreateAction(ds_none, bs_none, cmd_shoryuken, Sequence(Position2(354, 40), 45)
		.SetActionCenter(15, 40)
		.SetInitialVector(Vector2(1,-5))
		);

	int psychoH = psychoForceH;

	//サイコクラッシャー
	CreateAction(ds_none, bs_none, cmd_psycho, Sequence(Position2(400, 54),Size(54,16), 60)
		.SetActionCenter(20, 30)
		.SetInitialFunction([this]() {_effectFrame = 0; PlaySoundMem(voicesH[cmd_psycho], DX_PLAYTYPE_BACK); })
		.SetPreDrawFunction([this, psychoH]() {
			int m = MakeGraph(128, 64);
			GetDrawScreenGraph(_pos.x * 2 - 64, _pos.y * 2 - 32 - 32,
				_pos.x * 2 + 64, _pos.y * 2 + 32 - 32, m);
			GraphFilter(m, DX_GRAPH_FILTER_GAUSS, 32, 1000);
			DrawGraph(_pos.x * 2 - 64, _pos.y * 2 - 32 - 32, m, false);
			DeleteGraph(m);

			int x = _effectFrame % 5;
			int y = (_effectFrame / 5)+1;
			int cx = this->_lastTurn ? 128 - 60 : 60;
			DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, x * 128, y * 64, 128, 64, cx, 54, 2.0f, 0.0f, psychoH, true, this->_lastTurn);
			_effectFrame=(_effectFrame+1)%25;
		})
		.SetPostDrawFunction([this, psychoH]() {
			int x = _effectFrame % 5;
			int y = (_effectFrame / 5) + 1;
			int cx = this->_lastTurn ? 128 - 60 : 60;
			SetDrawBlendMode(DX_BLENDMODE_ADD,128);
			DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, x * 128, y * 64, 128, 64, cx, 54, 2.0f, 0.0f, psychoH, true, this->_lastTurn);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			_effectFrame = (_effectFrame + 1) % 25;
		})
		.SetInitialVector(Vector2(4,0))
	);

	//サマソ
	CreateAction(ds_none, bs_none, cmd_summer, Sequence(Position2(444, 80), 60)
		.SetActionCenter(20, 40)
		.SetInitialVector(Vector2(2, -5))
		.SetInitialFunction([]() {PlaySoundMem(voicesH[cmd_summer], DX_PLAYTYPE_BACK); })
	);

}

void 
Player::ShootHadoken() {
	const Position2& pos = Position();
	_hadoken->SetPosition(pos.x, pos.y - 20);
	_hadoken->SetVelocity(IsTurn() ? -4 : 4, 0);
	_hadoken->Resurrect();
}

void
Player::CreateAction(char direction, char button, Sequence seq) {
	_actions[InputId(direction, button)].push_back(seq);
}



void
Player::CreateAction(char direction, char button, unsigned char cmd,Sequence seq) {
	_actions[InputId(direction, button,cmd)].push_back(seq);
}

Player::Player(int handle) :_handle(handle), _walkFrame(0),_input(handle){
	_pos.x = 50;
	_pos.y = GROUND_LINE;
	_velY = 0;
	_velX = 0;
	_rc.w = 40;
	_rc.h = 40;
	_rc.SetCenter(_pos.x, _pos.y);
	_jumpSE = DxLib::LoadSoundMem("se/jump.wav");

	voicesH[cmd_hadoken] = LoadSoundMem("se/hadoken.wav");
	voicesH[cmd_shoryuken] = LoadSoundMem("se/shoryuken.wav");
	voicesH[cmd_tornadokick] = LoadSoundMem("se/tatumaki.wav");
	voicesH[cmd_psycho] = LoadSoundMem("se/psychocrasher.wav");
	voicesH[cmd_summer] = LoadSoundMem("se/summersult.wav");

	psychoForceH = LoadGraph("img/psycho_effect.png");
	curseH=LoadGraph("img/curse.png");


	_effectFrame = 0;
	_currentActionState = as_neutral;
	_attackTimer = 0;
	
	//一番最初はやっぱりニュートラル
	//だよね
	_updater = &Player::Neutral;
	CreateActionsData();
	_currentActionIt = _actions.end();

	_hadoken = new Hadoken(CutGraphInfo(_handle, 124, 542, 32, 12), 0, 0, 12, 12, Position2(0, 0), Vector2(0, 0), *this);
	_hadoken->Kill();
	_hadoken->Activate();
	_lastTurn = false;

	_life = 100;
	_drawer = []() {};
	_endTimer = 0;
}


Player::~Player(){
	DeleteSoundMem(_jumpSE);
	DeleteSoundMem(voicesH[cmd_hadoken]);
	DeleteSoundMem(voicesH[cmd_shoryuken]);
	DeleteSoundMem(voicesH[cmd_tornadokick]);
	DeleteSoundMem(voicesH[cmd_summer]);
	
	delete _hadoken;
}
const Rect&
Player::GetRect()const {
	return _rc;
}


bool 
Player::IsTurn()const{
	return _pos.x > _enemy->Position().x;
}
void
Player::Update(const KeyState& key) {
	if(!_hadoken->IsDead())
		_hadoken->Update();

	//呪い奥
	static float angle = 0.0f;
	DrawGraph(_pos.x * 2 - 10 + sin(angle) * 30, _pos.y * 2 - 40, curseH, true);

	_lastcmd=_input.CheckInput(IsTurn());
	(this->*_updater)(key);
	_input.Draw();
	_pos.x = max(min(310,_pos.x+(int)_velX),10);


	//呪い手前
	if (cos(angle)<0) {
		DrawGraph(_pos.x * 2 - 10 + sin(angle) * 30, _pos.y * 2 - 40, curseH, true);
	}
	angle += 0.15f;


	//矩形描画
	_rc.SetCenter(_pos.x,_pos.y-20);
	_rc.Draw();

	//中心点描画
	DrawBox((_pos.x - 1)*2, (_pos.y - 1)*2, (_pos.x + 1)*2, (_pos.y + 1)*2, 0xffff00, true);

	//攻撃とかやられ矩形の描画
	if (_currentActionIt==_actions.end()|| _currentSequenceIt == _currentActionIt->second.end())return;
	for (auto& hrc : _currentSequenceIt->rects) {
		hrc.Draw();
	}
	
}


void 
Player::SetEnemy(Enemy* enemy) {
	_enemy = enemy;
}
void
Player::GotoAction(const KeyState& key) {
	auto id = GetInputIdFromKeyState(key);
	if (_lastcmd != cmd_none) {
		id.button = bs_none;
		id.direction = ds_none;
		id.command = _lastcmd;
		
	}
	_currentActionIt = _actions.find(id);
	if (_currentActionIt!=_actions.end()) {
		_lastTurn = IsTurn();
		_updater = &Player::Attack;
		_currentSequenceIt = _currentActionIt->second.begin();
		_currentSequenceIt->FirstExecute();
		(this->*_updater)(key);
		//全矩形ぶっ生き返す
		for (auto& rc : _currentSequenceIt->rects) {
			rc.Resurrect();
		}
		_attackTimer = _currentSequenceIt->duration;
		_velX = IsTurn() ? -_currentSequenceIt->iv.x : _currentSequenceIt->iv.x;
		_velY = _currentSequenceIt->iv.y;
		this->_pos.y += (int)_velY;
	}
}
void
Player::Neutral(const KeyState& key){
	bool turnFlg = _pos.x > _enemy->Position().x;
	static float angle=0.0f;
	_drawer=[=]() {
		DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 0, 0, 40, 40, 19, 40, 2.0f, 0, _handle, true, IsTurn());
	};
	_drawer();


	_velX = 0;

	InputInfo now = _input.CurrentInput();
	InputInfo trigger = _input.IsTrigger();
	if ((now.part.direction&up) && now.part.length>2 && _jumpPrepare && _lastcmd == cmd_none) {
		_velY = -6.0f;
		_pos.y += (int)_velY;
		PlaySoundMem(_jumpSE, DX_PLAYTYPE_BACK);
		_updater = &Player::Aerial;
		_jumpPrepare = false;
	}
	if (trigger.part.direction&up) {
		_jumpPrepare = true;
	}
	if (now.part.direction & down) {
		_updater = &Player::Crouch;
	}
	if ((now.part.direction & right) || (now.part.direction & left)) {
		_updater = &Player::Move;
	}
	if (trigger.part.LP|| trigger.part.LK) {
		GotoAction(key);
	}

	
}

void
Player::Aerial(const KeyState& key) {
	
	if (_pos.y>=GROUND_LINE) {
		_updater = &Player::Neutral;
		_velY = 0.0f;
		_pos.y = GROUND_LINE;
	}
	else {
		_velY += 0.2f;
		_pos.y += (int)_velY;
	}
	_drawer = [=]() {
		DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 200, 0, 40, 40, 19, 40, 2.0f, 0, _handle, true, IsTurn());
	};
	_drawer();
}

void
Player::Crouch(const KeyState& key) {
	InputInfo now = _input.CurrentInput();
	InputInfo trigger = _input.IsTrigger();
	if (now.part.direction & down) {//しゃがみ続ける
	}else{
		_updater = &Player::Neutral;
	}
	_drawer = [=]() {
		if ((now.part.direction & 1) || (now.part.direction & 2)) {
			DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 120, 0, 40, 40, 19, 40, 2.0f, 0, _handle, true, IsTurn());
		}
		else {
			DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 80, 0, 40, 40, 19, 40, 2.0f, 0, _handle, true, IsTurn());
		}
	};
	_drawer();
	if (trigger.part.LP || trigger.part.LK) {
		GotoAction(key);
	}
	_velX = 0;
}

void
Player::Move(const KeyState& key){
	InputInfo now = _input.CurrentInput();
	InputInfo trigger = _input.IsTrigger();
	if (trigger.part.LP || trigger.part.LK) {
		GotoAction(key);
		return;
	}
	if ((now.part.direction&up) && now.part.length>2 && _jumpPrepare && _lastcmd==cmd_none) {
		_velY = -6.0f;
		_pos.y += (int)_velY;
		PlaySoundMem(_jumpSE, DX_PLAYTYPE_BACK);
		_updater = &Player::Aerial;
		_jumpPrepare = false;
	}
	if (trigger.part.direction&up) {
		_jumpPrepare=true;
	}
	if (now.part.direction & right) {
		_walkFrame = (_walkFrame + 1) % 8;
		_velX = 2;
	}else if (now.part.direction &left) {
		_walkFrame = (_walkFrame + 1) % 8;
		_velX = -2;
	}
	else {
		_updater = &Player::Neutral;
	}

	if (now.part.direction & down){
		_velX = 0;
		_updater = &Player::Crouch;
	}
	_drawer = [=]() {
		DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 0 + (_walkFrame / 4) * 40, 0, 40, 40, 19, 40, 2.0f, 0, _handle, true, IsTurn());
	};
	_drawer();
}
InputId 
Player::GetInputIdFromKeyState(const KeyState& key){
	InputId id = {};//戻り値用
	int shift = IsTurn() ? 1:0;//反転フラグをビットシフトに変換
	InputInfo info= _input.CurrentInput();
	InputPart& p = info.part;
	if(p.LP){
		id.button=bs_punch;
	}else if(p.LK){
		id.button=bs_kick;
	}
	char numpadbit=0;//上下左右でビット立てる
	//右1,左2,上4,下8
	if(p.direction&right){
		numpadbit |= (right<< shift);
	}
	if (p.direction&left) {
		numpadbit |= (left>>shift);
	}
	if (p.direction&up) {
		numpadbit |= up;
	}
	if (p.direction&down) {
		numpadbit |= down;
	}
	switch(numpadbit){
	case 10://左下
		id.direction=ds_leftdown;
		break;
	case 8://下
		id.direction = ds_down;
		break;
	case 9://右下
		id.direction = ds_rightdown;
		break;
	case 1://右
		id.direction = ds_right;
		break;
	case 5://右上
		id.direction = ds_rightup;
		break;
	case 4://上
		id.direction = ds_up;
		break;
	case 6://左上
		id.direction = ds_leftup;
		break;
	case 2://左
		id.direction = ds_left;
		break;
	}
	return id;
}
void
Player::Attack(const KeyState& key){
	if (_hitStopTimer == 0) {
		if (_pos.y < GROUND_LINE) {
			_pos.y += (int)_velY;
			_velY += 0.2f;
		}
	}
	int w = _currentSequenceIt->size.w;
	int centerx = _lastTurn ? w - _currentSequenceIt->centerpos.x : _currentSequenceIt->centerpos.x;
	Sequence& seq = *_currentSequenceIt;

	_currentSequenceIt->PreDrawExecute();
	_drawer = [=]() {
		DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, seq.pos.x, seq.pos.y, seq.size.w, seq.size.h, centerx, seq.centerpos.y, 2.0f, 0, _handle, true, _lastTurn);
	};
	_drawer();
	_currentSequenceIt->PostDrawExecute();

	if (_hitStopTimer > 0) {
		--_hitStopTimer;
		return;
	}
	if(--_attackTimer==0 )
	{
		if (++_currentSequenceIt == _currentActionIt->second.end()) {
			if (_pos.y < GROUND_LINE) {
				_updater = &Player::Aerial;
			}else if (_input.CurrentInput().part.direction&down) {
				_updater = &Player::Crouch;
			}
			else {
				_updater = &Player::Neutral;
			}
			return;
		}
		else {
			_currentSequenceIt->FirstExecute();
			_attackTimer = _currentSequenceIt->duration;
			_velX = IsTurn()? -_currentSequenceIt->iv.x:_currentSequenceIt->iv.x;
			_velY = _currentSequenceIt->iv.y;
			_pos.y += (int)_velY;
			for (auto& rc : _currentSequenceIt->rects) {
				rc.Resurrect();
			}
		}
	}
}

void
Player::Dying(const KeyState&) {
	int cx = IsTurn() ? 40 - 24 : 24;
	DxLib::DrawRectRotaGraph2(_pos.x * 2, _pos.y * 2, 320, 80, 40, 40, cx, 40, 2.0f, 0, _handle, true, IsTurn());
}

void
Player::PauseForEnd(const KeyState&) {
	if (--_endTimer <= 0) {
		if (_life > 0) {
			_updater = &Player::Neutral;
		}
		else {
			_updater = &Player::Dying;
		}
	}
	_drawer();
}
void 
Player::PauseForRendering() {
	_endTimer = 60; 
	_updater = &Player::PauseForEnd; 
	_velX=0;
}

void
Player::OnCollided(RectType rc) {
	_hitStopTimer = 30;
	if (rc == rt_attack) {
		_life -= 10;
	}
}

std::vector<HitRect>*
Player::GetCurrentRects() {
	if (_currentActionIt == _actions.end() || _currentSequenceIt == _currentActionIt->second.end()) {
		return nullptr;
	}
	return &_currentSequenceIt->rects;
}

