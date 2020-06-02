#include "TitleScene.h"
#include <DxLib.h>
#include"SceneManager.h"
#include "OpeningScene.h"
#include "../Graphics/BlinkDrawer.h"
#include "../Graphics/FragmentDrawer.h"
#include "../Graphics/LagDrawer.h"
#include "../Sound/BGMManager.h"
#include "../Sound/SoundEffect.h"
#include<cmath>

float ang=0.f;

TitleScene::TitleScene(void) : _timer(60)
{
	_drawer.Reset(new BaseDrawer("img/logo_m.png"));
	_drawer->SetLocation(loc_center);
	_drawerB.Reset(new BaseDrawer("img/logo_b.png"));
	_drawerB->SetLocation(loc_center);
	_drawer->MovePosition(0,-100);
	_drawerB->MovePosition(0,-100);
	_gamestart.Reset(new BlinkDrawer("img/game_start.png",100));
	_gamestart->SetLocation(loc_center);
	_gamestart->MovePosition(0,64);
	_fragmentDrawer.Reset(new FragmentDrawer());
	_dokuro.Reset(new LagDrawer("img/skulleffect.png",30,30));
	_dokuro->SetLocation(loc_center);
	int w,h;
	DxLib::GetWindowSize(&w,&h);
	_capHandle=DxLib::MakeGraph(w,h);
	_updater = &TitleScene::UpdatePre;
	BGMManager::Instance().Play(BGMID::bid_title);
	_glassBreakSE.Reset(new SoundEffect("snd/osb/glass_break.wav"));
	
}


TitleScene::~TitleScene(void)
{

}

void
TitleScene::UpdatePre(SceneManager& caller){
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	_drawer->Draw();
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,4.4*_timer);
	_drawerB->Draw();
	if(--_timer==0){
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		_updater=&TitleScene::UpdateMain;
	}
}

void
TitleScene::UpdateMain(SceneManager& caller){
	_dokuro->Draw();
	float x = std::sin(ang+=0.01f);
	_dokuro->SetPosition(x*400+370,360-(x*x*50));
	_drawer->Draw();
	_gamestart->Draw();
	if(CheckNextRequest_Debug()){	
		int result = DxLib::GetDrawScreenGraph(0,0,800,600,_capHandle);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_INVDESTCOLOR,255);
		_timer=60;
		_updater=&TitleScene::UpdateChanging;
		BGMManager::Instance().FadeOut();
	}
}

void
TitleScene::UpdateChanging(SceneManager& caller){
	DrawGraph(0,0,_capHandle,FALSE);
	DrawBox(0,0,800,600,GetColor(255,255,255),TRUE);
	if(--_timer==0){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		_timer=120;
		_fragmentDrawer->Capture();
		_fragmentDrawer->Break();
		_updater=&TitleScene::UpdateBreaking;
		_glassBreakSE->Play();
	}
}

void
TitleScene::UpdateBreaking(SceneManager& caller){
	_fragmentDrawer->Draw();
	if(--_timer==0){
		caller.ChangeScene(new OpeningScene());
	}
}

void
TitleScene::Update(SceneManager& caller){

	(this->*_updater)(caller);
}