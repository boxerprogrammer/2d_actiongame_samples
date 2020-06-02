#include "Model3D.h"
#include <DxLib.h>


VECTOR ToDxLibVector(const Vector3D& vec){
	return VGet(vec.x,vec.y,vec.z);
}

Vector3D ToVector3D(const DxLib::VECTOR& vec){
	return Vector3D(vec.x,vec.y,vec.z);
}

Vector3D Model3D::Position()const{
	DxLib::VECTOR& vec = DxLib::MV1GetPosition(_handle);
	return Vector3D(vec.x,vec.y,vec.z);
}

void 
Model3D::SetPosition(const Vector3D& vec){
	///à íuÇïœçXÇ∑ÇÈ
	_pos=vec;
	DxLib::MV1SetPosition(_handle,ToDxLibVector(vec));
}

void 
Model3D::RotateXYZ(Vector3D& vec){
	MV1SetRotationXYZ(_handle,ToDxLibVector(vec));
}

Vector3D
Model3D::Rotation()const {
	return ToVector3D(DxLib::MV1GetRotationXYZ(_handle));
}

Model3D::Model3D(const char* path) : _isAutoModel(true){
	_handle = DxLib::MV1LoadModel(path);
	Init(_handle);
}

Model3D::Model3D(int handle) : _handle(handle),_isAutoModel(false){
	Init(handle);
}

void Model3D::Init(int handle){
	_animTime=0.f;
	_animSpeed=1.f;
	_duration=0.f;
	_vel=Vector3D(0.f,0.f,0.f);
	_loop = false;
	_pos = Vector3D(0.f,0.f,0.f);
	_scale=Vector3D(1.f,1.f,1.f);
	int MaterialNum = MV1GetMaterialNum( _handle ) ;  
	for( int i = 0 ; i < MaterialNum ; i ++ )  
	{  
		// É}ÉeÉäÉAÉãÇÃó÷äsê¸ÇÃëæÇ≥ÇéÊìæ  
		float dotwidth = MV1GetMaterialOutLineDotWidth( _handle, i ) ;  
		// É}ÉeÉäÉAÉãÇÃó÷äsê¸ÇÃëæÇ≥ÇägëÂÇµÇΩï™è¨Ç≥Ç≠Ç∑ÇÈ  
		MV1SetMaterialOutLineDotWidth( _handle, i, dotwidth/50.0f ) ;  
	} 
}

Model3D::~Model3D(void)
{
	if(_isAutoModel){
		DxLib::MV1DeleteModel(_handle);
	}
}

void 
Model3D::ChangeAnimation(const char* animname,bool loop){
	ChangeAnimation(DxLib::MV1GetAnimIndex(_handle,animname),loop);
}

void 
Model3D::ChangeAnimation(const int animNo,bool loop){
	_loop=loop;
	if(_currentAnimNo==animNo){
		return;
	}
	DxLib::MV1DetachAnim(_handle,_attachedAnimNo);
	_currentAnimNo = animNo;
	_attachedAnimNo = DxLib::MV1AttachAnim(_handle,animNo,-1,FALSE);
	_animTime=0.f;
	_duration = DxLib::MV1GetAttachAnimTotalTime(_handle,_attachedAnimNo);
}

void 
Model3D::Update(bool isDraw){
	ExecuteVelocity();

	if(_loop && _animTime>=_duration){
		_animTime=0.f;
	}
	DxLib::MV1SetAttachAnimTime(_handle,_attachedAnimNo,_animTime);
	_animTime+=2.0f;

	if(isDraw){
		DxLib::MV1DrawModel(_handle);
	}
}


///ãœìôägëÂèkè¨ÇçsÇ§
void 
Model3D::SetScale(float scale){
	_scale=Vector3D(scale,scale,scale);
	DxLib::MV1SetScale(_handle,ToDxLibVector(_scale));
}

void 
Model3D::Syncronize(const Model3D& model){
	this->ChangeAnimation(model._currentAnimNo,model._loop);
	_animTime=model._animTime;
	this->SetScale(model._scale.x);
	this->SetPosition(model._pos);
	this->SetVelocity(model._vel);
	this->RotateXYZ(model.Rotation());
}