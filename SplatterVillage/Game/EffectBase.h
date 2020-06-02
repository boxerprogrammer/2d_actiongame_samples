#pragma once
#include<list>
#include<map>


class EffectFactory;

///�G�t�F�N�g�͊�{�I�Ɂu�Ђƒʂ�G�t�F�N�g���Đ������玩�E�v�Ƃ����������Ƃ�܂��B
///������{���炻���G�t�F�N�g��A����ȋ@�\��t������ꍇ��virtual�p�����Ďg���܂��傤
class EffectBase{
	friend EffectFactory;
	protected:
		bool _suicide;//���E�t���O
	public:
		EffectBase():_suicide(false){};
		virtual ~EffectBase(void){};
		virtual void Update(){};
		virtual void Draw(){};
		bool IsAvailable() const{return !_suicide;}
};

///�G�t�F�N�g�}�l�[�W��
class EffectManager{
private:
	EffectManager(){}
	virtual ~EffectManager(){}
public:

	//�S�G�t�F�N�g���[�h
	void LoadResources();

	//�S�G�t�F�N�g�A�����[�h
	void UnloadResources();

	//�G�t�F�N�g����
	EffectBase* Create();

	void Update();
	void Draw();
	
};

enum EffectType{
	 eff_bloodsplash//�����Ԃ�
	,eff_hiteffect//�q�b�g�G�t�F�N�g
};

class EffectBase;
class BloodSplash;
class HitEffect;
class Camera;

///�G�t�F�N�g�H��
class EffectFactory{
private:
	typedef std::list<EffectBase*> effects_t;
	effects_t _effects;
	std::map<EffectType,int> _handleTable;
	//�R�s�[�A����֎~
	EffectFactory(const EffectFactory& );
	EffectFactory& operator =(const EffectFactory& );
public:
	static EffectFactory& Instance(){
		static EffectFactory instance;
		return instance;
	}
	EffectFactory() ;

	BloodSplash* CreateBloodSplash(int x,int y,const Camera& camera,bool turn);
	HitEffect* CreateHitEffect(int x,int y,const Camera& camera,bool turn);

	void Update();
	void Draw();
	
};
