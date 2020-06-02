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
		if((*it)->_suicide){//���E�t���O�������Ă�����I�u�W�F�N�g��������A���X�g����������܂��B
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
	for(;it!=_effects.end();++it){//��{�I�Ɂu���E�ς݁v�I�u�W�F�N�g�͂����ŃR�[������Ȃ����߂��̋�����OK
		(*it)->Draw();
	}
}

