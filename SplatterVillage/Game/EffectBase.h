#pragma once
#include<list>
#include<map>


class EffectFactory;

///エフェクトは基本的に「ひと通りエフェクトを再生したら自殺」という方式をとります。
///もし基本からそれるエフェクトや、特殊な機能を付加する場合はvirtual継承して使いましょう
class EffectBase{
	friend EffectFactory;
	protected:
		bool _suicide;//自殺フラグ
	public:
		EffectBase():_suicide(false){};
		virtual ~EffectBase(void){};
		virtual void Update(){};
		virtual void Draw(){};
		bool IsAvailable() const{return !_suicide;}
};

///エフェクトマネージャ
class EffectManager{
private:
	EffectManager(){}
	virtual ~EffectManager(){}
public:

	//全エフェクトロード
	void LoadResources();

	//全エフェクトアンロード
	void UnloadResources();

	//エフェクト生成
	EffectBase* Create();

	void Update();
	void Draw();
	
};

enum EffectType{
	 eff_bloodsplash//血しぶき
	,eff_hiteffect//ヒットエフェクト
};

class EffectBase;
class BloodSplash;
class HitEffect;
class Camera;

///エフェクト工場
class EffectFactory{
private:
	typedef std::list<EffectBase*> effects_t;
	effects_t _effects;
	std::map<EffectType,int> _handleTable;
	//コピー、代入禁止
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
