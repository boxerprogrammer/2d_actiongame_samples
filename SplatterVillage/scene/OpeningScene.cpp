#include "OpeningScene.h"
#include "SceneManager.h"
#include "BeforeStage.h"
#include "../application.h"
#include "../Graphics/BaseDrawer.h"
#include "../EasyScript/TextDrawer.h"
#include "../Sound/BGMManager.h"
#include "../Sound/SoundEffect.h"
#include <DxLib.h>
#include "../File/KFile.h"

///炎の表現
class DrawFire{
private:
	int _handle;
	PositionInt2D _pos;
	int _ratio;
	int _bright;
public:
	DrawFire(int x,int y,const char* name);
	DrawFire(int x,int y,int handle);
	DrawFire* Clone(int x,int y){
		return new DrawFire(x,y,_handle);
	}
	void operator()();
};

void
DrawFire::operator()(){
	DxLib::DrawGraph(_pos.x,_pos.y,_handle,TRUE);
	if(_bright<64){
		++_bright;
	}
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,_bright);
	for(int i=1;i<10; ++i ){
		int rx=rand()%10-5;
		int ry=i;
		DxLib::DrawGraph(_pos.x+rx*_ratio,_pos.y-ry*_ratio,_handle,TRUE);
	}
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}
DrawFire::DrawFire(int x,int y,int handle):_pos(x,y),_ratio(1),_bright(0){
	_handle=handle;
	int w,h;
	DxLib::GetGraphSize(_handle,&w,&h);
	_ratio=h/25;
}

DrawFire::DrawFire(int x,int y,const char* name):_pos(x,y),_ratio(1),_bright(0){
	_handle=DxLib::LoadGraph(name);
	int w,h;
	DxLib::GetGraphSize(_handle,&w,&h);
	_ratio=h/25;
}


OpeningScene::OpeningScene(void) : _timer(64),_thunderTimer(0),_thunderInterval(32)
{
	_drawFires.resize(64);
	_drawer.Reset(new BaseDrawer("img/piece_villedge.png"));
	_thunder1.Reset(new BaseDrawer("img/thunder1.png"));
	_thunder2.Reset(new BaseDrawer("img/thunder2.png"));
	_drawFires[0].Reset(new DrawFire(250,400,"img/fire.png"));
	_thunderSE.Reset(new SoundEffect("snd/osb/thunder1.mp3"));
	for(int i=1;i<64;++i){
		_drawFires[i].Reset(_drawFires[0]->Clone(rand()%700,rand()%500));
	}
	_drawer->AdjustWindow();
	_updater = &OpeningScene::UpdateIntroducing;
	
	//DxLibのFileRead_sizeはオープン→シーク→テル→クローズと、イチイチ処理を食うため
	//自前のを使用(とは言え実際はWinAPIのGetFileSizeも内部でseek&tellやってんだろうけど)
	ScopedPtr<KFile> senario(FileOpener::Instance().Open("script/senario.txt"));
	unsigned int filesize = senario->FileSize();
	_script.resize(filesize+1);
	senario->Read(&_script[0],filesize);
	_script[filesize]='\0';
	_textDrawer.Reset(new TextDrawer(&_script[0]));

	BGMManager::Instance().Play(BGMID::bid_opening1);//最初に音鳴らすと、音切れが発生するため、後で鳴らす
}


OpeningScene::~OpeningScene(void)
{
}

void
OpeningScene::Update(SceneManager& caller){
	(this->*_updater)(caller);
}

void 
OpeningScene::UpdateIntroducing(SceneManager& caller){
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,255-(_timer*4));
	_drawer->Draw();
	if(--_timer==0){
		_timer=150;
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
		_updater=&OpeningScene::UpdateMain;
		_thunderSE->Play();
		
	}
}

void 
OpeningScene::UpdateMain(SceneManager& caller){
	_drawer->Draw();

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,(_thunderInterval-abs(_thunderInterval-_thunderTimer))*2);
	_thunder1->Draw();
	

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,abs(_thunderInterval-_thunderTimer)*(256/_thunderInterval));
	_thunder2->Draw();
	DxLib::DrawBox(0,0,800,600,0x555555,TRUE);

	++_thunderTimer;
	if(_thunderTimer==_thunderInterval){
		
		_thunder2->SetPosition(rand()%400,rand()%300);
	}

	if(_thunderTimer==_thunderInterval*2){
		_thunderSE->Play();
		_thunder1->SetPosition(rand()%400,rand()%300);
		_thunderTimer=0;
	}

	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	_thunderTimer+=0.1;
	if(--_timer==0){
		_timer=0;
		_updater=&OpeningScene::UpdateFire;
	}else if(CheckNextRequest_Debug()){	
		BGMManager::Instance().FadeOut();
		_updater=&OpeningScene::UpdateExiting;
	}
}

void 
OpeningScene::UpdateFire(SceneManager& caller){
	_drawer->Draw();
	if(_timer<300){
		++_timer;
	}

	int firenum = _timer/5;
	for(int i=0;i<firenum;++i){
		(*_drawFires[i])();
	}
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD,_timer);
	DrawBox(0,0,800,600,GetColor(255,32,8),true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	_textDrawer->Draw();
	if(CheckNextRequest_Debug()){	
		BGMManager::Instance().FadeOut();
		_updater=&OpeningScene::UpdateExiting;
	}
}

void 
OpeningScene::UpdateExiting(SceneManager& caller){
	caller.GetStageInfo().stageNo=0;
	caller.ChangeScene(new BeforeStage());
}