#include "Shot.h"
#include "../DxLib/DxLib.h"
#include "../3D/Model3D.h"
#include "../Game/Player.h"
#include "Camera.h"

Shot::Shot(const Camera* camera) : _camera(camera){
}

Shot::Shot(const Camera* camera,int handle,Player& player) : _camera(camera),_prev(NULL),_next(NULL)
{
	_model.Reset(new Model3D(handle));
	_rect.center=player.HitRect().center;
	_rect.center.x-=16;
	if(player.IsCrouching()){
		_rect.center.y+=16;
	}else{
		_rect.center.y-=16;
	}
	_rect.size.w=32;
	_rect.size.h=8;
	if(player.Direction()==dir_left){
		_model->RotateY(DegreeToRadian(180.f));
		_vec.x=-6;
	}else{
		_model->RotateY(DegreeToRadian(0.f));
		_vec.x=6;
	}
	_vec.y=0;
}


Shot::~Shot(void)
{
}

const Rect& 
Shot::HitRect()const{
	return _rect;
}

void 
Shot::DisplayDebugInfo(){
	DxLib::DrawBox(_rect.Left()-_camera->CurrentPos().x,
		_rect.Top()-_camera->CurrentPos().y,
		_rect.Right()-_camera->CurrentPos().x,
		_rect.Bottom()-_camera->CurrentPos().y,
		0xffffff,false);
}

void
Shot::Update(){
	_rect.center.x+=_vec.x;
	_model->Move(Vector3D(_vec.x,0.f,0.f));

	_model->SetPosition(Vector3D(_rect.center.x-_camera->CurrentPos().x,
		880.f-_rect.center.y,
		0.f));
	_model->Update();
}

void
Shot::OnDispose(){
	//ショットリスト繋ぎなおし
	_prev->_next = _next;
	if(_next!=NULL){
		_next->_prev = _prev;
	}
	delete this;
}

NullShot::NullShot():Shot(NULL){
	_rect.center=VectorInt2D(0,0);
	_rect.size=Size2D(0,0);
	_prev = NULL;
	_next = NULL;
}
NullShot::~NullShot(){
}
void
NullShot::Update(){
	//なにもしない
}

void
NullShot::OnDispose(){
	//こいつはDisposeされない
}

ShotFactory&
ShotFactory::Instance()
{
	static ShotFactory instance;
	return instance;
}

Shot*
ShotFactory::Create(const Camera& camera,Player& player)
{
	Shot* newshot = new Shot(&camera,_handle,player);
	Shot* lastShot=&_topShot;
	while(lastShot->Next()){
		lastShot=lastShot->Next();
	}
	lastShot->_next = newshot;
	newshot->_prev=lastShot;
	return newshot;
}

ShotFactory::ShotFactory() {
	_handle = DxLib::MV1LoadModel("model/inock/weapon.x");
}

void
ShotFactory::Clear(){
	Shot* shot = _topShot._next;
	while(shot!=NULL){
		Shot* del=shot;
		shot = shot->_next;
		delete del;
	}
}

void
ShotFactory::DrawShots(){
	Shot* shot = _topShot._next;
	while(shot!=NULL){
		shot->Update();
		shot = shot->_next;
	}
}

Shot& ShotFactory::TopShot(){
	return _topShot;
}
