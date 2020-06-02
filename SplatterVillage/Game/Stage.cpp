#include "Stage.h"
#include "Enemy.h"
#include "../Graphics/TileDrawer.h"
#include "../File/KFile.h"
#include <DxLib.h>
#include"Player.h"
#include"Shot.h"
#include"Camera.h"
#include"CollisionDetector.h"
#include"GameItem.h"
#include"GameObject.h"
namespace {
	enum MapDataType{
		mdtBack,//最背面背景
		mdtFront,//最背面手前
		mdtChip,//地形等の背景チップデータ
		mdtEvent,//ハシゴや宝箱系のイベントデータ
		mdtEnemy,//敵配置データ
	};
}
const int CHIP_SIZE=32;
Stage::Stage(void) : _croudScroll(),
	_position(),
	_count(0),
	_switch(false),
	_boss(NULL),
	player(new Player()),
	_endOfClearAction(false)
	
{
	
	camera.Reset(new Camera(*player));
	_enemyCreator.Reset(new EnemyFactory(*camera,*player)) ;
	_cloud.Reset(new TileDrawer("img/cloud3.png"));
	_terra.Reset(new TileDrawer("img/terra2.png"));
	_tree.Reset(new BaseDrawer("img/tree2.png"));
	_ladder.Reset(new BaseDrawer("img/ladder.png"));
	_far.Reset(new BaseDrawer("img/far.png"));
	_door_0.Reset(new BaseDrawer("img/door_0.png"));
	_door_1.Reset(new BaseDrawer("img/door_1.png"));
	_door_2.Reset(new BaseDrawer("img/door_2.png"));
	_door=_door_0.Get();
	_key.Reset(new GameItem("img/key.png",*camera,Rect(0,0,32,80)));
	_columbia=DxLib::LoadGraph("img/columbia.png");
	_clearFunc=&Stage::KeyStart;

	DxLib::SetupCamera_Ortho(600.f);
	_terra->SetPosition(0,250);

	//マップデータ読み込み
	ScopedPtr<KFile> stagefile(FileOpener::Instance().Open("data/stage_1.fmf"));
	stagefile->Read(&_header,sizeof(_header));
	data.resize(_header.numLayers);
	for(int i=0;i<_header.numLayers;++i){
		data[i].lines.resize(_header.height);
		for(int j=0;j<_header.height;++j){
			data[i].lines[j].chips.resize(_header.width);
			stagefile->Read(&data[i].lines[j].chips[0],_header.width);
		}
	}
	//背景チップ
	_chiphandle = DxLib::LoadGraph("img/bg_chips.png");


	EnemiesInitialize();
	LaddersInitialize();
	
	camera->SetRange(Size2D(32*data[0].lines[0].chips.size(),32*data[0].lines.size()));


}

//この関数(Update)は、ゲームステージに必要な物を表示し、当たり判定等の処理を行います。
void 
Stage::OCUpdate(){
	
	//
	//仕上げに、遠景と背景の表示をやって賑やかにします。それぞれDrawBackground関数とDrawTerrainChips関数を
	//呼び出しましょう…⑨
	//
	//ここで背景(遠景)の表示を行います。
	//背景の表示
	//DrawBackground();
	//
	//


	int offsetx=camera->CurrentPos().x/32;
	int offsety=camera->CurrentPos().y/32;
	int endX = GetEndXIdx(data,offsetx);

	//
	//ここで背景(近景)の表示を行います。
	//DrawTerrainChips(offsetx,offsety,endX);
	//
	//


	int pposX = (player->CurrentPos().x/32);
	_terra->SetScroll(camera->CurrentPos().x/3,0);



	//
	//ここで、当たり判定用の下準備をやっておきます。…②
	//当たり判定を行うためにdetectorという変数に、当たり判定を行うオブジェクトを代入します。
	//
	//CollisionDetector& detector=CollisionDetector::Instance();
	//

	//
	//detectorを利用する前に、きちんとクリアします。少し長い文字だけど正確に打ち込んでね
	//
	//detector.ClearDetectableEnemyTarget();
	//detector.ClearDetectableGeoTarget();
	//
	
	//
	//敵の表示をおこないます…③
	//いよいよもう少しで当たり判定が復活します、以下のUpdate～というコードを打ち込んでください。
	//
	//UpdateDetectableEnemies(detector,_enemies,player->CurrentPos());
	//
	//地形の当表示を行います…④
	//
	//UpdateDetectableChips(detector,
	//	data[mdtChip],
	//	offsetx,
	//	offsety,
	//	endX ,
	//	PositionInt2D(pposX,0));
	//
	//それぞれの関数に、先程取得した、detector等を代入してあげます。
	//
	//この関数呼び出しのように、引数が多く、横に長くなり、見づらくなるときは、,カンマを境に改行することができます。
	//
	

	//
	//まず、プレイヤーの描画を行います…①
	//以下のコードを打ち込んでください。
	//playerというのは、プレイヤーを表しています。Drawというのは描画を表します。
	//とにかく同じように打ちんでください。
	//
	//player->Draw(*camera);
	//

	//打ち込んだあとで見れば分かると思いますが、プレイヤーはあっという間に落下して死にます。
	//なので、地形との当たり判定が必要です。
	//
	//
	//カメラ位置の調整を行います…⑥
	//
	//さて、今の状態ではスクロールを行いません。このため、動ける範囲が１画面上になってしまっています。
	//これをどうにかするためには、カメラをきちんと更新してあげなければいけません。
	//以下のように文字列を打ち込んでみてください。
	//->ってのはカメラに命令するってこと。この場合はカメラに「更新しろ」って言っているんですね。
	//
	//camera->Update();
	//
	//


	//
	//ここまで書いても、まだ当たり判定をしているわけではないので、落ちます。さらに以下のif文の中に
	//地形とプレイヤーの当たり判定を書いてください。
	//

	if(player->IsMoved()){
		//
		//上のif文は、プレイヤーが動かない限り地形との当たり判定は必要ないため
		//動いた時にのみ判定するという意味です。
		//
		//地形とプレイヤーとの当たり判定を行います…⑤
		//
		//detector.CheckGeograph(*player);
		//
		//ちなみにCheckGeographのgeographというのは「地形」という意味です。
		//
	}

	//
	//ここまででは、まだ、敵と当たり判定が行われないので、ゲームになりません
	//当たり判定をつけてゲームにしましょう…⑧
	//
	//当たり判定
	//
	//敵と、自機の当たり判定
	//detector.CheckHitEnemies(*player);
	//
	//


	//自機弾と敵の当たり判定
	//
	//detector.CheckHitEnemiesAndShot(ShotFactory::Instance().TopShot());
	//detector.CheckGeographAndEnemy();
	//
	
	//
	//当たり判定の範囲を限定するために、カメラの位置をセットします。
	//
	//detector.SetCameraPos(camera->CurrentPos());
	//
	//

	
	//
	//
	//ひとりぼっちは寂しいからな、敵の表示を行うぜ…⑦
	//
	//
	//int sizeOfEnemy = _enemies.size();
	//for(int i=0;i<sizeOfEnemy;++i){
	//	_enemies[i]->Update();
	//	_enemies[i]->Draw();
	//}
	//
	//敵は複数いるので、繰り返し処理を意味する「for」というコードを使用して、複数の敵を表示します。
	//

	

	if(player->CurrentPos().y > 880 ){
		player->KillMe();	
	}

	ShotFactory::Instance().DrawShots();
}

void Stage::EnemiesInitialize(){
	LayerData& layer=data[mdtEnemy];
	chipline_t::iterator it=layer.lines.begin();
	int lineno=0;
	for(;it!=layer.lines.end();++it){
		chips_t::iterator cit = it->chips.begin();
		int col=0;
		for(;cit!=it->chips.end();++cit){
			if(*cit==255){
				++col;
				continue;
			}
			switch(*cit){
				case 0://髑髏
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/dokuro_anim.png",enm_dokuro)));
					_enemyCreator->SetAppearAction(enemies.back().Get(),"img/dokuro_appear_anim.png");
					break;
				case 1://イビルアイ
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/eye_anim.png")));
					enemies.back()->SetLife(3);
					break;
				case 2://烏
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/crow_anim.png",enm_crow)));
					break;
				case 3://クチビルゲ
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/enemy_lip.png",enm_lipman)));
					enemies.back()->SetLife(5);
					break;
				case 4://ゲローマン
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/gero.png")));
					break;
				case 5://ぼっすん
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("model/eyeball/eyeball.x")));
					_boss=enemies.back().Get();
					break;
				default:
					++col;
					continue;
			}
			enemies.back()->InitPosition(VectorInt2D(CHIP_SIZE*col,CHIP_SIZE*lineno));
			++col;
		}
		++lineno;
	}
}


void Stage::LaddersInitialize(){
	ladders.clear();
	LayerData& layer=data[mdtEvent];
	chipline_t::iterator it=layer.lines.begin();
	int lineno=0;
	for(;it!=layer.lines.end();++it){
		chips_t::iterator cit = it->chips.begin();
		int col=0;
		for(;cit!=it->chips.end();++cit){
			if(*cit==255){
				++col;
				continue;
			}
			if(*cit%16==0){
				ladders.push_back(SharedPtr<Ladder>(new Ladder()));
			}else{
				++col;
				continue;
			}
			ladders.back()->InitPosition(VectorInt2D(CHIP_SIZE*col,CHIP_SIZE*lineno));
			++col;
		}
		++lineno;
	}
}

Stage::~Stage(void)
{
}

int 
Stage::GetEndXIdx(layers_t& layers,int offsetx){
	return 26+offsetx >= data[0].lines[0].chips.size() ? data[0].lines[0].chips.size()-1 : offsetx+26;
}



void 
Stage::DrawDecoratedBackground(layers_t& layers,int layerNo,int offsetx,int offsety,int endX){
	for(int y=2+offsety;y<20+offsety;++y){
		int itBgn=offsetx > 4 ? offsetx-4 : 0;
		for(int x=itBgn;x<endX;++x){
			unsigned char chip = data[layerNo].lines[y].chips[x];
			if(chip==0){
				DxLib::DrawGraph(-camera->CurrentPos().x+x*32,
					-camera->CurrentPos().y+y*32,
					_tree->Handle(),
					TRUE);

			}else if(chip==4){
				DxLib::DrawGraph(-camera->CurrentPos().x+x*32,
					-camera->CurrentPos().y+y*32,
					_door->Handle(),
					TRUE);
			}
		}
	}
}

void 
Stage::DrawFrontEventObjects(layers_t& layers,int layerNo,int offsetx,int offsety,int endX){
	for(int y=2+offsety;y<20+offsety;++y){
		int itBgn=offsetx > 4 ? offsetx-4 : 0;
		for(int x=itBgn;x<endX;++x){
			unsigned char chip = data[layerNo].lines[y].chips[x];
			if(chip%16==0){
				DxLib::DrawGraph(-camera->CurrentPos().x+x*32,
					-camera->CurrentPos().y+y*32,
					_ladder->Handle(),
					TRUE);

			}
		}
	}
}

//敵との当たり判定をトラバース
void
Stage::UpdateDetectableEnemies(CollisionDetector& detector,enemies_t& enemies,const PositionInt2D& playerpos){
	enemies_t::iterator it = enemies.begin();
	enemies_t::iterator itEnd = enemies.end();
	for(;it!=itEnd;++it){
		if( abs(playerpos.x - (*it)->Position().x) < 420 && abs(playerpos.y - (*it)->Position().y) < 400){
			(*it)->OnWakeUp();
			if((*it)->IsDead()){
				continue;
			}
			detector.StoreDetectableEnemyTarget(it->Get());
		}
	}
}

//はしごとの当たり判定をトラバース
void
Stage::UpdateLadders(CollisionDetector& detector,ladders_t& ladders,const PositionInt2D& playerpos){
	ladders_t::iterator it = ladders.begin();
	ladders_t::iterator itEnd = ladders.end();
	for(;it!=itEnd;++it){
		if( abs(playerpos.x - (*it)->Position().x) < 420 && abs(playerpos.y - (*it)->Position().y) < 400){
			detector.StoreLadder(it->Get());
		}
	}
}



//地形チップデータによるイベント発生(ていうか当たり判定系)
void 
Stage::UpdateDetectableChips(CollisionDetector& detector,
				LayerData& layer,
				int offsetx,
				int offsety,
				int endX,
				PositionInt2D& playerPos){
	for(int y=2+offsety;y<20+offsety;++y){
		for(int x=offsetx;x<endX;++x){
			unsigned char chip = layer.lines[y].chips[x];
			if(chip!=0xFF){
				//水の演出(チップを切り替えることにより、波を表現)
				if(chip==48 && _switch){
					chip=49;
				}else if(chip==49 && _switch){
					chip=48;
				}

				if(x>=playerPos.x-8 && x<=playerPos.x+12 ){
					if(chip==0||chip==1){
						detector.StoreDetectableGeoTarget(
							Rect(
								x*32+16,y*32+16,18,18
								)
							);
					}
				}
				DxLib::DrawRectGraph(-camera->CurrentPos().x+x*32,
					-camera->CurrentPos().y+y*32,
					(chip%16)*32,
					(chip/16)*32,
					32,32,
					_chiphandle,TRUE,FALSE);
			}
		}
	}
}


//背景の表示
void
Stage::DrawBackground(){
	++_croudScroll.x;//雲スクロール値更新
	_far->Draw();//遠くの山描画
	_terra->Draw();//ちょっと近くの地形描画
	_cloud->SetScroll(_croudScroll.x/2 + camera->CurrentPos().x/3,0);//雲のスクロール
	_cloud->Draw();//雲描画
}

void 
Stage::DrawTerrainChips(int offsetx,int offsety , int endX){
	_count=(_count+1)%10;
	if(_count==0){
		_switch=!_switch;
	}

	//木等の装飾用背景最背面(0番)
	DrawDecoratedBackground(data,mdtBack,offsetx,offsety,endX);

	//木等の装飾用背景最前列(1番)
	DrawDecoratedBackground(data,mdtFront,offsetx,offsety,endX);
}



const Player&
Stage::GetPlayer() const{
	return *player;
}

bool
Stage::IsClear()const{
	if(_boss==NULL){
		return false;
	}
	if(_boss->IsDead()){
		return true;
	}
	return false;
}




//この関数(Update)は、ゲームステージに必要な物を表示し、当たり判定等の処理を行います。
void 
Stage::Update(){
	
	//
	//仕上げに、遠景と背景の表示をやって賑やかにします。それぞれDrawBackground関数とDrawTerrainChips関数を
	//呼び出しましょう…⑨
	//
	//ここで背景(遠景)の表示を行います。
	//背景の表示
	DrawBackground();
	//
	//


	int offsetx=camera->CurrentPos().x/32;
	int offsety=camera->CurrentPos().y/32;
	int endX = GetEndXIdx(data,offsetx);

	//
	//ここで背景(近景)の表示を行います。
	DrawTerrainChips(offsetx,offsety,endX);
	//
	//


	int pposX = (player->CurrentPos().x/32);
	_terra->SetScroll(camera->CurrentPos().x/3,0);



	//
	//ここで、当たり判定用の下準備をやっておきます。…②
	//当たり判定を行うためにdetectorという変数に、当たり判定を行うオブジェクトを代入します。
	//
	CollisionDetector& detector=CollisionDetector::Instance();
	//

	//
	//detectorを利用する前に、きちんとクリアします。少し長い文字だけど正確に打ち込んでね
	//
	detector.ClearDetectableEnemyTarget();
	detector.ClearDetectableGeoTarget();
	//
	detector.ClearLadder();
	//
	//敵の表示をおこないます…③
	//いよいよもう少しで当たり判定が復活します、以下のUpdate～というコードを打ち込んでください。
	//
	UpdateDetectableEnemies(detector,enemies,player->CurrentPos());
	//
	//地形の当表示を行います…④
	//
	UpdateDetectableChips(detector,
		data[mdtChip],
		offsetx,
		offsety,
		endX ,
		PositionInt2D(pposX,0));

	//ハシゴの表示準備
	UpdateLadders(detector,ladders,player->CurrentPos());

	//
	//それぞれの関数に、先程取得した、detector等を代入してあげます。
	//
	//この関数呼び出しのように、引数が多く、横に長くなり、見づらくなるときは、,カンマを境に改行することができます。
	//
	
	DrawFrontEventObjects(data,mdtEvent,offsetx,offsety,endX);

	//
	//まず、プレイヤーの描画を行います…①
	//以下のコードを打ち込んでください。
	//playerというのは、プレイヤーを表しています。Drawというのは描画を表します。
	//とにかく同じように打ちんでください。
	//
	player->Draw(*camera);
	//

	//打ち込んだあとで見れば分かると思いますが、プレイヤーはあっという間に落下して死にます。
	//なので、地形との当たり判定が必要です。
	//
	//
	//カメラ位置の調整を行います…⑥
	//
	//さて、今の状態ではスクロールを行いません。このため、動ける範囲が１画面上になってしまっています。
	//これをどうにかするためには、カメラをきちんと更新してあげなければいけません。
	//以下のように文字列を打ち込んでみてください。
	//->ってのはカメラに命令するってこと。この場合はカメラに「更新しろ」って言っているんですね。
	//
	camera->Update();
	//
	//


	//
	//ここまで書いても、まだ当たり判定をしているわけではないので、落ちます。さらに以下のif文の中に
	//地形とプレイヤーの当たり判定を書いてください。
	//

	if(player->IsMoved()){
		//
		//上のif文は、プレイヤーが動かない限り地形との当たり判定は必要ないため
		//動いた時にのみ判定するという意味です。
		//
		//地形とプレイヤーとの当たり判定を行います…⑤
		//
		detector.CheckGeograph(*player);
		//
		//ちなみにCheckGeographのgeographというのは「地形」という意味です。
		//
	}

	//
	//ここまででは、まだ、敵と当たり判定が行われないので、ゲームになりません
	//当たり判定をつけてゲームにしましょう…⑧
	//
	//当たり判定
	//
	//敵と、自機の当たり判定
	detector.CheckHitEnemies(*player);
	//
	//


	//自機弾と敵の当たり判定
	//
	detector.CheckHitEnemiesAndShot(ShotFactory::Instance().TopShot());
	//
	//

	//
	//当たり判定の範囲を限定するために、カメラの位置をセットします。
	//
	detector.SetCameraPos(camera->CurrentPos());
	//
	//

	
	//
	//
	//ひとりぼっちは寂しいからな、敵の表示を行うぜ…⑦
	//
	//
	int sizeOfEnemy = enemies.size();
	for(int i=0;i<sizeOfEnemy;++i){
		enemies[i]->Update();
		enemies[i]->Draw();
	}
	//
	//敵は複数いるので、繰り返し処理を意味する「for」というコードを使用して、複数の敵を表示します。
	//
	detector.CheckGeographAndEnemy();

	//海に落ちたら死亡
	if(player->CurrentPos().y > 880 ){
		player->KillMe();	
	}

	detector.CheckHitLadders(*player);

	ShotFactory::Instance().DrawShots();
}

void 
Stage::KeyStart(){
	_endOfClearAction=false;
	_key->SetPosition(_boss->Position());
	_keyY = _key->GetRect().Top();
	_keyVec=Vector2D<float>((camera->CurrentPos().x-_key->GetRect().Left())/120.f,-10.f);
	_key->Move(VectorInt2D(0,-3));
	_clearFunc=&Stage::KeyAction;
	_key->Draw();
}

void 
Stage::KeyAction(){
	_keyVec.y+=0.5;
	_key->Move(VectorInt2D(_keyVec.x,_keyVec.y));
	_key->SetVisible(true);
	_key->Draw();
	if(_keyY<=_key->GetRect().Top()){
		_clearFunc=&Stage::KeyGround;
	}
	if(CollisionDetector::Instance().IsCollided(player->HitRect(),_key->GetRect())){
		player->OnClear();
		_clearActionTimer=120;
		_clearFunc=&Stage::DoorOpen;
	}
}

void
Stage::KeyGround(){
	_key->Draw();
	if(CollisionDetector::Instance().IsCollided(player->HitRect(),_key->GetRect())){
		player->OnClear();
		_clearActionTimer=120;
		_clearFunc=&Stage::DoorOpen;
	}
}

void
Stage::DoorOpen(){
	if(_clearActionTimer>100){
		_door=_door_0.Get();
	}else if(_clearActionTimer>60){
		_door=_door_1.Get();
	}else{
		_door=_door_2.Get();
	}

	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,250-(_clearActionTimer*2));
	DxLib::DrawGraph(300,400,_columbia,true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	if(--_clearActionTimer==0){
		_endOfClearAction=true;
	}
}

bool
Stage::EndOfClearAction()const{
	return _endOfClearAction;
}

void 
Stage::StageClearAction(){
	(this->*_clearFunc)();
}




