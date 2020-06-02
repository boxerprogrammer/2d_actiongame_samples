#include"EffectBase.h"
#include "BloodSplash.h"
#include"HitEffect.h"
#include"../DxLib/DxLib.h"
#include"Camera.h"

EffectFactory::EffectFactory(){
}

BloodSplash* 
EffectFactory::CreateBloodSplash(int x,int y,const Camera& camera,bool turn){
	if(_handleTable.find(eff_bloodsplash)==_handleTable.end()){
		_handleTable[eff_bloodsplash]=DxLib::LoadGraph("img/bloodsplash.png");
	}
	BloodSplash* ret = new BloodSplash(camera,_handleTable[eff_bloodsplash],x,y,256,256,30,turn);
	_effects.push_back(ret);
	return ret;
}

HitEffect* 
EffectFactory::CreateHitEffect(int x,int y,const Camera& camera,bool turn){
	if(_handleTable.find(eff_hiteffect)==_handleTable.end()){
		_handleTable[eff_hiteffect]=DxLib::LoadGraph("img/hiteffect2.png");
	}
	HitEffect* ret=new HitEffect(camera,_handleTable[eff_hiteffect],x,y,256,256,10,turn);
	_effects.push_back(ret);
	return ret;
}

void 
EffectFactory::Update(){
	effects_t::iterator it=_effects.begin();
	for(;it!=_effects.end();++it){
		(*it)->Update();
	}
	for(it=_effects.begin();it!=_effects.end();++it){
		if((*it)->_suicide){//自殺フラグが立っていたらオブジェクトを解放し、リストから消去します。
			delete *it;
			it=_effects.erase(it);
			if(_effects.empty()){
				return;
			}
		}
	}
}

void 
EffectFactory::Draw(){
	effects_t::iterator it=_effects.begin();
	for(;it!=_effects.end();++it){//基本的に「自殺済み」オブジェクトはここでコールされないためこの挙動でOK
		(*it)->Draw();
	}
}

