#include"Player.h"
#include <DxLib.h>
#include "Camera.h"
#include "../3D/Model3D.h"
#include "../Sound/SoundEffect.h"
#include "GameObject.h"
#include "Shot.h"
float _yvel=0.f;
float _g=0.6f;
float _runVel=3.0;


Player::Player() : _position(300,640),
	_velocity(0,0),
	_isGround(false),
	_isPrejumping(false),
	_isJumping(false),
	_isLanding(false),
	_isThrowing(false),
	_rect(324,664,16,32),
	_crashTimer(0),
	_ultimateTimer(0),
	_isPressShotButton(false),
	_pst(pst_armor),
	_isClear(false){
	_model.Reset(new Model3D("model/inock/inock.x"));
	_modelN.Reset(new Model3D("model/inock/inock_naked.x"));
	_modelD.Reset(new Model3D("model/inock/inock_dead.x"));
	_damageSE.Reset(new SoundEffect("snd/osb/yurusite.wav"));
	_deadSE.Reset(new SoundEffect("snd/osb/hoa-.wav"));
	_throwSE.Reset(new SoundEffect("snd/osb/throw.wav"));
	_jumpSE.Reset(new SoundEffect("snd/voice/jump.wav"));
	_model->SetScale(0.75f);
	_model->SetAnimSpeed(2.f);
	_currentModel=_model.Get();
	_yvel=0.f;
}


Player::~Player(){
}
void 
Player::CrashEffect(const Camera& camera){
	if(_crashTimer>0){
		--_crashTimer;
		int offset = 30-_crashTimer;
		int radius = 3+(_crashTimer%4)*2;
		int x = _position.x-camera.CurrentPos().x + _rect.size.w;
		int y = _position.y-camera.CurrentPos().y + _rect.size.h;

		if (_pst != pst_dying) {
			DrawCircle(x - offset, y - offset, radius, 0xffffff);
			DrawCircle(x - offset, y + offset, radius, 0xffffff);
			DrawCircle(x + offset, y - offset, radius, 0xffffff);
			DrawCircle(x + offset, y + offset, radius, 0xffffff);
		}

		if(_crashTimer==0&&_pst==pst_dying){
			_pst=pst_dead;
		}
	}
}
bool
Player::IsNotJumping()const{
	return !_isPrejumping && !_isJumping;
}

bool 
Player::IsStanding()const{
	return !_isPrejumping && !_isJumping && !_isLanding;
}


void 
Player::OnPrejump(){
	_currentModel->ChangeAnimation("inock_jump",false);
	_isPrejumping=true;
	if(_currentModel->IsAnimEnd()){
		OnJump();
	}
}

void 
Player::OnStop(){
	_currentModel->Move(Vector3D(0.0f,0.f,0.f));
	if(!_isThrowing){
		_currentModel->ChangeAnimation("inock_stop",true);
	}
}

void 
Player::OnLanding(){
	if(_pst==pst_dead || _pst==pst_dying){
		return;
	}
	_currentModel->ChangeAnimation("inock_landing");
	
	_currentModel->SetVelocity(Vector3D(0.f,0.f,0.f));
	_isJumping = false;
	_isGround=true;
	if(_currentModel->IsAnimEnd()){
		_isLanding = false;
	}
}

static bool debug_M=false;

void
Player::DisplayDebugInfo(const Camera& camera){
	DxLib::DrawBox(_rect.Left()-camera.CurrentPos().x,
		_rect.Top()-camera.CurrentPos().y,
		_rect.Right()-camera.CurrentPos().x,
		_rect.Bottom()-camera.CurrentPos().y,
		0xAAAAFF,FALSE);
}


void 
Player::Draw(const Camera& camera){
	int input = DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if(_isClear){
		_currentModel->Update(_ultimateTimer%2==0);
		return;
	}
	_lastPosition = _position;
	_isCrouch=false;
	_isMoved = false;
	_isLaddering=false;
	_rect.size.h = 32;
	if(input&PAD_INPUT_RIGHT && IsNotJumping()){
		_currentModel->ChangeAnimation("inock_run",true);
		_currentModel->RotateY(DegreeToRadian(-90.f));
		_velocity.x=_runVel;	
		_isLanding=false;
		_isMoved=true;
	}else if(input&PAD_INPUT_LEFT && IsNotJumping() ){
		_velocity.x=-_runVel;
		_currentModel->ChangeAnimation("inock_run",true);
		_currentModel->RotateY(DegreeToRadian(90.f));
		_isLanding=false;
		_isMoved=true;
	}else if(input&PAD_INPUT_DOWN  && IsNotJumping() ){
		_rect.size.h = 16;
		if(!_isThrowing && !_isCrouch && !_isLanding){
			_currentModel->ChangeAnimation("inock_crouch");
			_velocity.x=0;
		}
		_isCrouch=true;
	}else if(input&PAD_INPUT_UP  && IsNotJumping() ){
		if(!_isThrowing && !_isCrouch){
			_currentModel->RotateY(PI);
			_currentModel->ChangeAnimation("inock_laddering",true);
			if(_isOverlappedLadder){
				_isLaddering=true;
				_position.y-=2;
			}
		}
	}else{
		if(IsStanding()){
			OnStop();
			_velocity.x=0;
		}
	}
	if(camera.PlayerPosition().x+_velocity.x<=0 || camera.PlayerPosition().x+_velocity.x>=800-64){
		_velocity.x=0;
	}
	_position.x+=_velocity.x;

	
	if(input&PAD_INPUT_A){
		if(IsStanding()){
			_isPrejumping=true;
		}
	}
	if(input&PAD_INPUT_B){
		if(!_isPressShotButton&&!_isThrowing){
			_throwSE->Play();
			if(_isCrouch){
				_currentModel->ChangeAnimation("inock_crouch_throw");
			}else{
				_currentModel->ChangeAnimation("inock_throw");
			}
			ShotFactory::Instance().Create(camera,*this);
			_isThrowing=true;
		}
		_isPressShotButton=true;
	}else{
		_isPressShotButton=false;
	}


	if(_currentModel->IsAnimEnd()){
		_isThrowing=false;
	}

	if(_isPrejumping){
		OnPrejump();
	}
	if(_isGround){
		_yvel=0.f;
		_velocity.y=0;
	}else{
		_isMoved=true;
		_yvel+=_g;
		_velocity.y = _yvel;
		_position.y+=_velocity.y;
	}
	_rect.center.x=_position.x+_rect.size.w;
	_rect.center.y=_position.y+_rect.size.h;

	_currentModel->SetPosition(Vector3D(_position.x+_rect.size.w-camera.CurrentPos().x,
		800.f-_position.y,
		0.f));

	if(_isLanding){
		OnLanding();
	}
	if(_ultimateTimer>0){
		--_ultimateTimer;
	}
	_currentModel->Update(_ultimateTimer%2==0);
	CrashEffect(camera);
}

CharacterDirection
Player::Direction()const{
	return _currentModel->Rotation().y<0.f ? dir_right : dir_left;
}

const PositionInt2D&
Player::CurrentPos() const{
	return _position;
}

const VectorInt2D&
Player::Velocity() const{
	return _velocity;
}

const int
Player::LastFootPosY()const{
	return _position.y + _rect.size.w*2;
}

void
Player::OnJump(){
	_currentModel->ChangeAnimation("inock_jump");
	_jumpSE->Play();
	_isMoved=true;
	_yvel=-12.f;
	_isJumping = true;
	_isPrejumping=false;
	_isGround=false;
}

void
Player::OnDamaged(){
	if(_ultimateTimer>0){
		return;
	}
	_modelN->Syncronize(*_model);
	_currentModel=_modelN.Get();
	_currentModel->ChangeAnimation("inock_damage");
	_isMoved=true;
	_yvel=-8.f;
	if(Direction()==dir_right){
		_velocity.x=-_runVel;
	}else{
		_velocity.x=_runVel;
	}
	_isJumping = true;
	_isPrejumping=false;
	_isGround=false;
	_pst = pst_naked;
	_damageSE->Play();
	_crashTimer=30;
	_ultimateTimer=120;
}

void
Player::OnDead(){
	if(_ultimateTimer>0){
		return;
	}
	_modelD->Syncronize(*_modelN);
	_currentModel=_modelD.Get();
	_currentModel->ChangeAnimation("inock_dead");
	_isMoved=true;
	_yvel=-8.f;
	if(Direction()==dir_right){
		_velocity.x=-_runVel;
	}else{
		_velocity.x=_runVel;
	}
	_isJumping = true;
	_isPrejumping=false;
	_isGround=false;
	_pst = pst_dying;
	_deadSE->Play();
	_crashTimer=120;
}

void 
Player::OnGround(){
	_isGround=true;
	if(_isJumping){
		_velocity.x=0;
		_isLanding=true;
	}
}


void
Player::OnAir(){
	_isGround=false;
}


const Rect&
Player::HitRect() const{
	return _rect;
}

void
Player::Adjust(const Size2D& size,CharacterDirection dir){
	if( dir==dir_up||dir==dir_down||abs(size.w)>abs(size.h) ){
		_position.y-=size.h;
	}else{
		_position.x-=size.w;
	}
}

const PlayerState 
Player::State() const{
	return _pst;
}

void
Player::KillMe(){
	OnDead();
	_pst=pst_dead;
}

void 
Player::OnHitEnemy(){
	if (_pst == pst_dying || _pst==pst_dead)return;
	if(_pst==pst_armor){
		OnDamaged();
	}else{
		OnDead();
	}
}

void 
Player::OnClear(){
	_currentModel->ChangeAnimation("inock_columbia");
	_isClear=true;
}


void
Player::OnHitLadder(Ladder& ladder){
	_isOverlappedLadder=true;
	if(_isLaddering){
		Rect& rc=ladder.Collider();
		_position.x=rc.Left();
	}
}

void
Player::OnNotHitLadder(){
	_isOverlappedLadder=false;
}