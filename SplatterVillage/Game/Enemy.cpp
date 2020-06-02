#include "Enemy.h"
#include<DxLib.h>
#include<algorithm>
#include<Shlwapi.h>
#include"Camera.h"
#include"../3D/Model3D.h"
#include"Player.h"
#include"Shot.h"
#include<cassert>
///ìGãììÆãÊï™(Ç«ÇÃÇÊÇ§Ç»äGÇï\é¶Ç∑ÇÈÇÃÇ©)
enum ActionKbn{
	act_normal,
	act_appear,
	act_disappear,
	act_dying,
	act_dead,
};

Enemy::Enemy(const Camera& camera,const PositionInt2D& pos,const EnemyFactory& factory):_camera(camera),
	_factory(factory),
	_pos(pos),
	_crashTimer(0),
	_isDead(false),
	_life(1){
	_rect.center=pos;
	_rect.size.w = 16;
	_rect.size.h = 16;
	_rect.center.x = pos.x+_rect.size.w;
	_rect.center.y = pos.y+_rect.size.h;
	
}

Enemy::~Enemy(){
}

void
Enemy::CrashEffect(){
	if(_crashTimer>0){
		--_crashTimer;
		//int offset = 30-_crashTimer;
		//int radius = 3+(_crashTimer%4)*2;
		//int x = _rect.center.x-_camera.CurrentPos().x;
		//int y = _rect.center.y-_camera.CurrentPos().y;

		//DrawCircle(x-offset,y-offset,radius,0xff4444);
		//DrawCircle(x-offset,y+offset,radius,0xff4444);
		//DrawCircle(x+offset,y-offset,radius,0xff4444);
		//DrawCircle(x+offset,y+offset,radius,0xff4444);
	}
}

void
Enemy::OnDamage(Shot* shot){
	if(--_life==0){
		OnDead(shot);
	}else{
		bool turn= shot->Vector().x<0;
		EffectFactory::Instance().CreateHitEffect(_pos.x+_rect.size.w,_pos.y+_rect.size.h,_camera,turn);
	}
}

void
Enemy::OnDead(Shot* shot){
	_crashTimer=15;
	_isDead=true;
	_rect.Off();
	
}

void
Enemy::SetPosition(const PositionInt2D& pos){
	_pos=pos;
	_rect.center.x = pos.x+_rect.size.w;
	_rect.center.y = pos.y+_rect.size.h;
}

void
Enemy::InitPosition(const PositionInt2D& pos){
	_initialPos=pos;
	SetPosition(pos);
}

//ÇQDìG
Enemy2D::Enemy2D(const Camera& camera,int handle,int x,int y,const EnemyFactory& f,bool animTurn):Enemy(camera,PositionInt2D(x,y),f)
	,_isAnim(false)
	,_turn(false)
	,_animTurn(animTurn)
{
	_key=act_normal;
	_handles[_key]=PictInfo(1,handle);
	DxLib::GetGraphSize(handle,&_w,&_h);
	_rect.size.w=_w/2;
	_rect.size.h=_h/2;
	SetPosition(PositionInt2D(x,y));
	
}

//ÇQDìG
Enemy2D::Enemy2D(const Camera& camera,int handle,int w,int h , int num , const EnemyFactory& f,int interval,bool animTurn):Enemy(camera,PositionInt2D(0,0),f),
	_interval(interval),
	_waitTimer(0),
	_isAnim(true),
	_w(w),
	_h(h),
	_idx(0),
	_num(num),
	_turn(false),
	_animTurn(animTurn)
{
	_key=act_normal;
	_handles[_key]=PictInfo(num,handle,interval);
	_rect.size.w=w/2;
	_rect.size.h=h/2;
	SetPosition(PositionInt2D(0,0));
}

Enemy2D::Enemy2D(const Camera& camera,int handle,const EnemyFactory& f,bool animTurn):Enemy(camera,PositionInt2D(0,0),f)
	,_isAnim(false)
	,_turn(false)
	,_animTurn(animTurn)
{
	_key=act_normal;
	_handles[_key]=PictInfo(1,handle);
	DxLib::GetGraphSize(handle,&_w,&_h);
	_rect.size.w=_w/2;
	_rect.size.h=_h/2;
	
}

Enemy2D::~Enemy2D(void)
{
}

void
Enemy2D::Update(){
	if(_isAnim){
		if(++_waitTimer>=_handles[_key].interval){
			if(_animTurn){
				_idx=(++_idx)%((_handles[_key].num-1)*2);
			}else{
				_idx=(_idx+1)%_handles[_key].num;
			}
			_waitTimer=0;
		}
	}
}

void
Enemy2D::Draw(){
	if(_pos.x-_camera.CurrentPos().x+_w<0)return;
	CrashEffect();
	if(_isDead ){
		if(_crashTimer>0){
			DxLib::DrawRectGraph(_pos.x-_camera.CurrentPos().x,_pos.y-_camera.CurrentPos().y,0,0,_w,_h,_handles[_key].handle,true,_turn);
		}
	}else{
		if(_isAnim){
			int idx=_idx;
			if(_animTurn){
				idx=abs(_idx-(_handles[_key].num-1));
			}
			assert(idx >= 0);
			assert(idx < _handles[_key].num);
			DxLib::DrawRectGraph(_pos.x-_camera.CurrentPos().x,_pos.y-_camera.CurrentPos().y,idx*_w,0,_w,_h,_handles[_key].handle,true,_turn);
		}else{
			DxLib::DrawRectGraph(_pos.x-_camera.CurrentPos().x,_pos.y-_camera.CurrentPos().y,0,0,_w,_h,_handles[_key].handle,true,_turn);
		}
	}
	
}

void
Enemy2D::OnDead(Shot* shot){
	bool turn= shot->Vector().x<0;
	EffectFactory::Instance().CreateBloodSplash(_pos.x+_rect.size.w,_pos.y+_rect.size.h,_camera,turn);
	Enemy::OnDead(shot);
}

//3DìG
Enemy3D::Enemy3D(const Camera& camera,int handle,int x,int y,const EnemyFactory& f):Enemy(camera,PositionInt2D(x,y),f),
	_model(new Model3D(handle))
{
	_updater = &Enemy3D::UpdateAppear;
	_life=10;
}

Enemy3D::Enemy3D(const Camera& camera,int handle,const EnemyFactory& f):Enemy(camera,PositionInt2D(0,0),f),
	_model(new Model3D(handle))
{
	_updater = &Enemy3D::UpdateAppear;
	_life=10;
}

Enemy3D::~Enemy3D(void)
{
}

void
Enemy3D::Update(){

}

void
Enemy3D::UpdateAppear(){
	_pos.y+=4.f;
	_model->ChangeAnimation("landing",false);
	if(_model->IsAnimEnd()){
		_updater = &Enemy3D::UpdateNeutral;
	}
}

void
Enemy3D::UpdateNeutral(){
	_model->ChangeAnimation("neutral",true);
}
void 
Enemy3D::SetPosition(const PositionInt2D& pos){
	_pos=pos;
	_rect.center.x = pos.x;
	_rect.center.y = pos.y;
}
void
Enemy3D::Draw(){
	CrashEffect();
	if(_isDead){
		return;
	}
	if(_pos.x-_camera.CurrentPos().x > 800 ){
		return;
	}
	(this->*_updater)();
	_model->SetPosition(Vector3D(_pos.x-_camera.CurrentPos().x,
		848.f-_pos.y,
		0.f));
	_model->Update();
	
}

void 
EnemyFactory::Destroy(){
	namemap_t::iterator it=_handles.begin();
	for(;it!=_handles.end();++it){
		DxLib::DeleteGraph(it->second);
	}
	_handles.clear();
}
///ÉhÉNÉçÉNÉâÉX
class Dokuro : public Enemy2D{
	private:
		void (Dokuro::*_updater)();
		const Player& _player;
		bool _sleeping;
		int _sleepTime;
		int _ridx;
		void UpdateWait(){
			if(!_sleeping){
				if(--_sleepTime<=0){
					_isDead=false;
					_key=act_appear;
					_waitTimer=0;
					_idx=0;
					_updater=&Dokuro::UpdateAppear;
					_factory.PlaySE(EnemySeType::se_dokuro_appear);
				}
			}
		}

		///ìGèoåª
		void UpdateAppear(){
			if(!_sleeping){
				if(_isAnim){
					if(_idx>0){
						_rect.On();
					}
					if(_waitTimer+1==_handles[_key].interval && _idx+1==_handles[_key].num){
						_updater=&Dokuro::UpdateNeutral;
						_key = act_normal;
						_waitTimer = 0;
						_idx = 0;

					}
				}
			}
			Enemy2D::Update();
		}

		void UpdateDisappear(){
			if(++_waitTimer>=_handles[_key].interval){
				_ridx=(_ridx+1)%(_handles[_key].num+1);
				_idx=_handles[_key].num-_ridx;
				if(_idx==0){
					_isDead=true;
					Resurrect();
				}
				_waitTimer=0;
			}

		}

		///ìGÉmÅ[É}Éã
		void UpdateNeutral(){
			if(!_isDead){
				_turn=(_vel.x>0.f);
				_pos.x+= _vel.x;
				_rect.center.x+= _vel.x;
			}
			if(_isDead&&_crashTimer==0){
				Resurrect();
			}
			Enemy2D::Update();
		}
	public:
		Dokuro(const Camera& camera,const Player& p , int handle,int w,int h,int num,const EnemyFactory& f) : Enemy2D(camera,handle,w,h,num,f,6)
			,_player(p),_sleeping(true)
		{
			_updater = &Dokuro::UpdateWait;
			_isDead=true;
		}
		void InitPosition(const PositionInt2D& pos){
			
			_vel.x= _player.CurrentPos().x==pos.x?0: (_player.CurrentPos().x-pos.x)/std::abs(_player.CurrentPos().x-pos.x);
			Enemy::InitPosition(pos);
			_turn=(_vel.x>0.f);
			_sleepTime=((std::rand()%15)+10)*3;
		}
		void Resurrect(){
			PositionInt2D p=_initialPos;
			p.x+=(rand()%64)-32;
			InitPosition(p);
			_life=1;
			_waitTimer=0;
			_key=act_appear;
			_updater = &Dokuro::UpdateWait;
		}
		void OnWakeUp(){
			_sleeping=false;
		}
		void SetAppearAction(int handle,int num,int interval=6){
			_key=act_appear;
			_handles[_key].handle=handle;
			_handles[_key].num=num;
			_handles[_key].interval=interval;
		}

		void Disappear(){
			if(_updater==&Dokuro::UpdateDisappear){
				return;
			}
			_key=act_appear;
			_ridx=1;
			_idx=_handles[_key].num-_ridx;
			_waitTimer=0;
			_updater = &Dokuro::UpdateDisappear;
		}

		//SetDeathAction();
		void Update(){
			(this->*_updater)();
			
		}

};

///ÉJÉâÉXÉNÉâÉX
class Crow : public Enemy2D{
	private:
		void (Crow::*_updater)();
		const Player& _player;
		bool _sleeping;
		int _sleepTime;
		int _ridx;
		void UpdateWait(){
			if(!_sleeping){
				if(--_sleepTime<=0){
					_isDead=false;
					_key=act_appear;
					_waitTimer=0;
					_idx=0;
					_updater=&Crow::UpdateAppear;
					_rect.On();
				}
			}
		}

		///ìGèoåª
		void UpdateAppear(){
			if(!_sleeping){
				if(_isAnim){
					if(_waitTimer+1==_handles[_key].interval && _idx+1==_handles[_key].num){
						_updater=&Crow::UpdateNeutral;
						_key=act_normal;
						_waitTimer=0;
					}
				}
			}
			Enemy2D::Update();
		}

		//ìGÇ™Ç≥ÇÊÇ»ÇÁ
		void UpdateDisappear(){
			if(++_waitTimer>=_handles[_key].interval){
				_ridx=(_ridx+1)%(_handles[_key].num+1);
				_idx=_handles[_key].num-_ridx;
				if(_idx==0){
					_isDead=true;
					Resurrect();
				}
				_waitTimer=0;
			}

		}

		///ìGÉmÅ[É}Éã
		void UpdateNeutral(){
			if(!_isDead){
				_turn=(_vel.x>0.f);
				_pos.x+= _vel.x;
				_rect.center.x+= _vel.x;
			}
			Enemy2D::Update();
			//if(_isDead&&_crashTimer==0){
			//	Resurrect();
			//}
			
		}
	public:
		Crow(const Camera& camera,const Player& p , int handle,int w,int h,int num,const EnemyFactory& f) : Enemy2D(camera,handle,w,h,num,f,6)
			,_player(p),_sleeping(true)
		{
			_updater = &Crow::UpdateWait;
			_isDead=true;
		}
		void InitPosition(const PositionInt2D& pos){
			if(_player.CurrentPos().x==pos.x){
				_vel.x=4;
			}else{
				_vel.x= ((_player.CurrentPos().x-pos.x)/std::abs(_player.CurrentPos().x-pos.x))*4;
			}
			Enemy::InitPosition(pos);
			_turn=(_vel.x>0.f);
			_sleepTime=((std::rand()%15)+10)*3;
		}
		void Resurrect(){
			PositionInt2D p=_initialPos;
			p.x+=(rand()%64)-32;
			InitPosition(p);
			_life=1;
			_waitTimer=0;
			_key=act_appear;
			_updater = &Crow::UpdateWait;
		}
		void OnWakeUp(){
			_sleeping=false;
		}
		void SetAppearAction(int handle,int num,int interval=6){
			_key=act_appear;
		}

		void Disappear(){
		}

		void Update(){
			(this->*_updater)();
			
		}

};

///ÉNÉ`ÉrÉãÉQÉNÉâÉX
class LipMan : public Enemy2D{
	private:
		void (LipMan::*_updater)();
		const Player& _player;
		bool _sleeping;
		int _sleepTime;
		int _ridx;
		void UpdateWait(){
			if(!_sleeping){
				;
			}
		}

		///ìGèoåª
		void UpdateAppear(){
			if(!_sleeping){
				;
			}
			Enemy2D::Update();
		}

		//ìGÇ™Ç≥ÇÊÇ»ÇÁ
		void UpdateDisappear(){
			;

		}

		///ìGÉmÅ[É}Éã
		void UpdateNeutral(){
			if(!_isDead){
				_turn=(_vel.x>0.f);
				_pos.x+= _vel.x;
				_rect.center.x+= _vel.x;
			}
			Enemy2D::Update();
			
		}
	public:
		LipMan(const Camera& camera,const Player& p , int handle,int w,int h,int num,const EnemyFactory& f) : Enemy2D(camera,handle,w,h,num,f,6)
			,_player(p),_sleeping(true)
		{
			_life=5;
			_updater = &LipMan::UpdateWait;
			_isDead=true;
		}
		void InitPosition(const PositionInt2D& pos){
			if(_player.CurrentPos().x==pos.x){
				_vel.x=4;
			}else{
				_vel.x= ((_player.CurrentPos().x-pos.x)/std::abs(_player.CurrentPos().x-pos.x))*4;
			}
			Enemy::InitPosition(pos);
			_turn=(_vel.x>0.f);
			_sleepTime=((std::rand()%15)+10)*3;
		}
		void Resurrect(){
			PositionInt2D p=_initialPos;
			p.x+=(rand()%64)-32;
			InitPosition(p);
			_life=5;
			_waitTimer=0;
			_key=act_appear;
			_updater = &LipMan::UpdateWait;
		}
		void OnWakeUp(){
			_sleeping=false;
		}
		void SetAppearAction(int handle,int num,int interval=6){
			_key=act_appear;
		}

		void Disappear(){
		}

		void Update(){
			(this->*_updater)();
			
		}

};

Enemy*
EnemyFactory::Create(const char* name,EnemyType type){
	bool is3D = false;
	const char* ext = ::PathFindExtension(name);
	if(strcmp(ext,".x")==0){
		is3D=true;
	}
	namemap_t::iterator it = _handles.find(name);
	int handle = -1;
	if(it==_handles.end()){
		if(is3D){
			handle = DxLib::MV1LoadModel(name);
		}else{
			handle = DxLib::LoadGraph(name);
		}
		_handles.insert(std::make_pair(name,handle));
	}else{
		handle = _handles[name];
	}
	if(is3D){
		return new Enemy3D(_camera,handle,*this);
	}else{
		if(strstr(name,"_anim")==NULL){
			return new Enemy2D(_camera,handle,*this);
		}else{
			std::string strName=name;
			std::string::size_type pos=strName.find(ext);
			const char* csv=".csv";
			strName.replace(strName.begin()+pos,strName.end(),csv,csv+sizeof(csv));
			int num = GetPictureNumForOneAnimation(strName);
			int w,h;
			DxLib::GetGraphSize(handle,&w,&h);
			switch(type){
				case enm_dokuro:
					return new Dokuro(_camera,_player,handle,w/num,h,num,*this);
				case enm_crow:
					return new Crow(_camera,_player,handle,w/num,h,num,*this);
				case enm_lipman:
					return new LipMan(_camera,_player,handle,w/num,h,num,*this);
				default:
					return new Enemy2D(_camera,handle,w/num,h,num,*this,8,true);
				
			}
		}
	}
}

int EnemyFactory::GetPictureNumForOneAnimation(std::string& strName)
{
	int num = 0;
	int csvHandle = DxLib::FileRead_open(strName.c_str());
	char buffer[256];
	while (DxLib::FileRead_gets(buffer, 256, csvHandle) != -1) {
		int frame = 0;
		auto ret = sscanf(buffer, "\"%d\"", &frame);
		if (ret == std::char_traits<char>::eof()) {
			continue;
		}
		if (frame>0) {
			++num;
		}
	}
	DxLib::FileRead_close(csvHandle);
	return num;
}

void EnemyFactory::SetAppearAction(Enemy* enemy,const char* name){
	const char* ext = ::PathFindExtension(name);
	namemap_t::iterator it = _handles.find(name);
	int handle = -1;
	if(it==_handles.end()){
		handle = DxLib::LoadGraph(name);
		_handles.insert(std::make_pair(name,handle));
	}else{
		handle = _handles[name];
	}
	if(strstr(name,"_anim")==NULL){
		enemy->SetAppearAction(handle,1);
	}else{
		std::string strName=name;
		std::string::size_type pos=strName.find(ext);
		const char* csv=".csv";
		strName.replace(strName.begin()+pos,strName.end(),csv,csv+sizeof(csv));
		int num = GetPictureNumForOneAnimation(strName);
		int w,h;
		DxLib::GetGraphSize(handle,&w,&h);
		enemy->SetAppearAction(handle,num,10);
	}
}

EnemyFactory::EnemyFactory(const Camera& camera,const Player& player): _camera(camera),_player(player){
	_seMap[se_dokuro_appear]= LoadSoundMem("snd/voice/zon_woo.wav");
}

EnemyFactory::~EnemyFactory(){
}


void 
EnemyFactory::PlaySE(EnemySeType type)const{
	int h =_seMap.find(type)->second;
	DxLib::PlaySoundMem(h,DX_PLAYTYPE_BACK);
}