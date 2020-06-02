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
		mdtBack,//�Ŕw�ʔw�i
		mdtFront,//�Ŕw�ʎ�O
		mdtChip,//�n�`���̔w�i�`�b�v�f�[�^
		mdtEvent,//�n�V�S��󔠌n�̃C�x���g�f�[�^
		mdtEnemy,//�G�z�u�f�[�^
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

	//�}�b�v�f�[�^�ǂݍ���
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
	//�w�i�`�b�v
	_chiphandle = DxLib::LoadGraph("img/bg_chips.png");


	EnemiesInitialize();
	LaddersInitialize();
	
	camera->SetRange(Size2D(32*data[0].lines[0].chips.size(),32*data[0].lines.size()));


}

//���̊֐�(Update)�́A�Q�[���X�e�[�W�ɕK�v�ȕ���\�����A�����蔻�蓙�̏������s���܂��B
void 
Stage::OCUpdate(){
	
	//
	//�d�グ�ɁA���i�Ɣw�i�̕\��������ē��₩�ɂ��܂��B���ꂼ��DrawBackground�֐���DrawTerrainChips�֐���
	//�Ăяo���܂��傤�c�H
	//
	//�����Ŕw�i(���i)�̕\�����s���܂��B
	//�w�i�̕\��
	//DrawBackground();
	//
	//


	int offsetx=camera->CurrentPos().x/32;
	int offsety=camera->CurrentPos().y/32;
	int endX = GetEndXIdx(data,offsetx);

	//
	//�����Ŕw�i(�ߌi)�̕\�����s���܂��B
	//DrawTerrainChips(offsetx,offsety,endX);
	//
	//


	int pposX = (player->CurrentPos().x/32);
	_terra->SetScroll(camera->CurrentPos().x/3,0);



	//
	//�����ŁA�����蔻��p�̉�����������Ă����܂��B�c�A
	//�����蔻����s�����߂�detector�Ƃ����ϐ��ɁA�����蔻����s���I�u�W�F�N�g�������܂��B
	//
	//CollisionDetector& detector=CollisionDetector::Instance();
	//

	//
	//detector�𗘗p����O�ɁA������ƃN���A���܂��B�����������������ǐ��m�ɑł�����ł�
	//
	//detector.ClearDetectableEnemyTarget();
	//detector.ClearDetectableGeoTarget();
	//
	
	//
	//�G�̕\���������Ȃ��܂��c�B
	//���悢����������œ����蔻�肪�������܂��A�ȉ���Update�`�Ƃ����R�[�h��ł�����ł��������B
	//
	//UpdateDetectableEnemies(detector,_enemies,player->CurrentPos());
	//
	//�n�`�̓��\�����s���܂��c�C
	//
	//UpdateDetectableChips(detector,
	//	data[mdtChip],
	//	offsetx,
	//	offsety,
	//	endX ,
	//	PositionInt2D(pposX,0));
	//
	//���ꂼ��̊֐��ɁA����擾�����Adetector���������Ă����܂��B
	//
	//���̊֐��Ăяo���̂悤�ɁA�����������A���ɒ����Ȃ�A���Â炭�Ȃ�Ƃ��́A,�J���}�����ɉ��s���邱�Ƃ��ł��܂��B
	//
	

	//
	//�܂��A�v���C���[�̕`����s���܂��c�@
	//�ȉ��̃R�[�h��ł�����ł��������B
	//player�Ƃ����̂́A�v���C���[��\���Ă��܂��BDraw�Ƃ����͕̂`���\���܂��B
	//�Ƃɂ��������悤�ɑł���ł��������B
	//
	//player->Draw(*camera);
	//

	//�ł����񂾂��ƂŌ���Ε�����Ǝv���܂����A�v���C���[�͂����Ƃ����Ԃɗ������Ď��ɂ܂��B
	//�Ȃ̂ŁA�n�`�Ƃ̓����蔻�肪�K�v�ł��B
	//
	//
	//�J�����ʒu�̒������s���܂��c�E
	//
	//���āA���̏�Ԃł̓X�N���[�����s���܂���B���̂��߁A������͈͂��P��ʏ�ɂȂ��Ă��܂��Ă��܂��B
	//������ǂ��ɂ����邽�߂ɂ́A�J������������ƍX�V���Ă����Ȃ���΂����܂���B
	//�ȉ��̂悤�ɕ������ł�����ł݂Ă��������B
	//->���Ă̂̓J�����ɖ��߂�����Ă��ƁB���̏ꍇ�̓J�����Ɂu�X�V����v���Č����Ă����ł��ˁB
	//
	//camera->Update();
	//
	//


	//
	//�����܂ŏ����Ă��A�܂������蔻������Ă���킯�ł͂Ȃ��̂ŁA�����܂��B����Ɉȉ���if���̒���
	//�n�`�ƃv���C���[�̓����蔻��������Ă��������B
	//

	if(player->IsMoved()){
		//
		//���if���́A�v���C���[�������Ȃ�����n�`�Ƃ̓����蔻��͕K�v�Ȃ�����
		//���������ɂ̂ݔ��肷��Ƃ����Ӗ��ł��B
		//
		//�n�`�ƃv���C���[�Ƃ̓����蔻����s���܂��c�D
		//
		//detector.CheckGeograph(*player);
		//
		//���Ȃ݂�CheckGeograph��geograph�Ƃ����̂́u�n�`�v�Ƃ����Ӗ��ł��B
		//
	}

	//
	//�����܂łł́A�܂��A�G�Ɠ����蔻�肪�s���Ȃ��̂ŁA�Q�[���ɂȂ�܂���
	//�����蔻������ăQ�[���ɂ��܂��傤�c�G
	//
	//�����蔻��
	//
	//�G�ƁA���@�̓����蔻��
	//detector.CheckHitEnemies(*player);
	//
	//


	//���@�e�ƓG�̓����蔻��
	//
	//detector.CheckHitEnemiesAndShot(ShotFactory::Instance().TopShot());
	//detector.CheckGeographAndEnemy();
	//
	
	//
	//�����蔻��͈̔͂����肷�邽�߂ɁA�J�����̈ʒu���Z�b�g���܂��B
	//
	//detector.SetCameraPos(camera->CurrentPos());
	//
	//

	
	//
	//
	//�ЂƂ�ڂ����͎₵������ȁA�G�̕\�����s�����c�F
	//
	//
	//int sizeOfEnemy = _enemies.size();
	//for(int i=0;i<sizeOfEnemy;++i){
	//	_enemies[i]->Update();
	//	_enemies[i]->Draw();
	//}
	//
	//�G�͕�������̂ŁA�J��Ԃ��������Ӗ�����ufor�v�Ƃ����R�[�h���g�p���āA�����̓G��\�����܂��B
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
				case 0://��
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/dokuro_anim.png",enm_dokuro)));
					_enemyCreator->SetAppearAction(enemies.back().Get(),"img/dokuro_appear_anim.png");
					break;
				case 1://�C�r���A�C
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/eye_anim.png")));
					enemies.back()->SetLife(3);
					break;
				case 2://�G
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/crow_anim.png",enm_crow)));
					break;
				case 3://�N�`�r���Q
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/enemy_lip.png",enm_lipman)));
					enemies.back()->SetLife(5);
					break;
				case 4://�Q���[�}��
					enemies.push_back(SharedPtr<Enemy>(_enemyCreator->Create("img/gero.png")));
					break;
				case 5://�ڂ�����
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

//�G�Ƃ̓����蔻����g���o�[�X
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

//�͂����Ƃ̓����蔻����g���o�[�X
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



//�n�`�`�b�v�f�[�^�ɂ��C�x���g����(�Ă����������蔻��n)
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
				//���̉��o(�`�b�v��؂�ւ��邱�Ƃɂ��A�g��\��)
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


//�w�i�̕\��
void
Stage::DrawBackground(){
	++_croudScroll.x;//�_�X�N���[���l�X�V
	_far->Draw();//�����̎R�`��
	_terra->Draw();//������Ƌ߂��̒n�`�`��
	_cloud->SetScroll(_croudScroll.x/2 + camera->CurrentPos().x/3,0);//�_�̃X�N���[��
	_cloud->Draw();//�_�`��
}

void 
Stage::DrawTerrainChips(int offsetx,int offsety , int endX){
	_count=(_count+1)%10;
	if(_count==0){
		_switch=!_switch;
	}

	//�ؓ��̑����p�w�i�Ŕw��(0��)
	DrawDecoratedBackground(data,mdtBack,offsetx,offsety,endX);

	//�ؓ��̑����p�w�i�őO��(1��)
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




//���̊֐�(Update)�́A�Q�[���X�e�[�W�ɕK�v�ȕ���\�����A�����蔻�蓙�̏������s���܂��B
void 
Stage::Update(){
	
	//
	//�d�グ�ɁA���i�Ɣw�i�̕\��������ē��₩�ɂ��܂��B���ꂼ��DrawBackground�֐���DrawTerrainChips�֐���
	//�Ăяo���܂��傤�c�H
	//
	//�����Ŕw�i(���i)�̕\�����s���܂��B
	//�w�i�̕\��
	DrawBackground();
	//
	//


	int offsetx=camera->CurrentPos().x/32;
	int offsety=camera->CurrentPos().y/32;
	int endX = GetEndXIdx(data,offsetx);

	//
	//�����Ŕw�i(�ߌi)�̕\�����s���܂��B
	DrawTerrainChips(offsetx,offsety,endX);
	//
	//


	int pposX = (player->CurrentPos().x/32);
	_terra->SetScroll(camera->CurrentPos().x/3,0);



	//
	//�����ŁA�����蔻��p�̉�����������Ă����܂��B�c�A
	//�����蔻����s�����߂�detector�Ƃ����ϐ��ɁA�����蔻����s���I�u�W�F�N�g�������܂��B
	//
	CollisionDetector& detector=CollisionDetector::Instance();
	//

	//
	//detector�𗘗p����O�ɁA������ƃN���A���܂��B�����������������ǐ��m�ɑł�����ł�
	//
	detector.ClearDetectableEnemyTarget();
	detector.ClearDetectableGeoTarget();
	//
	detector.ClearLadder();
	//
	//�G�̕\���������Ȃ��܂��c�B
	//���悢����������œ����蔻�肪�������܂��A�ȉ���Update�`�Ƃ����R�[�h��ł�����ł��������B
	//
	UpdateDetectableEnemies(detector,enemies,player->CurrentPos());
	//
	//�n�`�̓��\�����s���܂��c�C
	//
	UpdateDetectableChips(detector,
		data[mdtChip],
		offsetx,
		offsety,
		endX ,
		PositionInt2D(pposX,0));

	//�n�V�S�̕\������
	UpdateLadders(detector,ladders,player->CurrentPos());

	//
	//���ꂼ��̊֐��ɁA����擾�����Adetector���������Ă����܂��B
	//
	//���̊֐��Ăяo���̂悤�ɁA�����������A���ɒ����Ȃ�A���Â炭�Ȃ�Ƃ��́A,�J���}�����ɉ��s���邱�Ƃ��ł��܂��B
	//
	
	DrawFrontEventObjects(data,mdtEvent,offsetx,offsety,endX);

	//
	//�܂��A�v���C���[�̕`����s���܂��c�@
	//�ȉ��̃R�[�h��ł�����ł��������B
	//player�Ƃ����̂́A�v���C���[��\���Ă��܂��BDraw�Ƃ����͕̂`���\���܂��B
	//�Ƃɂ��������悤�ɑł���ł��������B
	//
	player->Draw(*camera);
	//

	//�ł����񂾂��ƂŌ���Ε�����Ǝv���܂����A�v���C���[�͂����Ƃ����Ԃɗ������Ď��ɂ܂��B
	//�Ȃ̂ŁA�n�`�Ƃ̓����蔻�肪�K�v�ł��B
	//
	//
	//�J�����ʒu�̒������s���܂��c�E
	//
	//���āA���̏�Ԃł̓X�N���[�����s���܂���B���̂��߁A������͈͂��P��ʏ�ɂȂ��Ă��܂��Ă��܂��B
	//������ǂ��ɂ����邽�߂ɂ́A�J������������ƍX�V���Ă����Ȃ���΂����܂���B
	//�ȉ��̂悤�ɕ������ł�����ł݂Ă��������B
	//->���Ă̂̓J�����ɖ��߂�����Ă��ƁB���̏ꍇ�̓J�����Ɂu�X�V����v���Č����Ă����ł��ˁB
	//
	camera->Update();
	//
	//


	//
	//�����܂ŏ����Ă��A�܂������蔻������Ă���킯�ł͂Ȃ��̂ŁA�����܂��B����Ɉȉ���if���̒���
	//�n�`�ƃv���C���[�̓����蔻��������Ă��������B
	//

	if(player->IsMoved()){
		//
		//���if���́A�v���C���[�������Ȃ�����n�`�Ƃ̓����蔻��͕K�v�Ȃ�����
		//���������ɂ̂ݔ��肷��Ƃ����Ӗ��ł��B
		//
		//�n�`�ƃv���C���[�Ƃ̓����蔻����s���܂��c�D
		//
		detector.CheckGeograph(*player);
		//
		//���Ȃ݂�CheckGeograph��geograph�Ƃ����̂́u�n�`�v�Ƃ����Ӗ��ł��B
		//
	}

	//
	//�����܂łł́A�܂��A�G�Ɠ����蔻�肪�s���Ȃ��̂ŁA�Q�[���ɂȂ�܂���
	//�����蔻������ăQ�[���ɂ��܂��傤�c�G
	//
	//�����蔻��
	//
	//�G�ƁA���@�̓����蔻��
	detector.CheckHitEnemies(*player);
	//
	//


	//���@�e�ƓG�̓����蔻��
	//
	detector.CheckHitEnemiesAndShot(ShotFactory::Instance().TopShot());
	//
	//

	//
	//�����蔻��͈̔͂����肷�邽�߂ɁA�J�����̈ʒu���Z�b�g���܂��B
	//
	detector.SetCameraPos(camera->CurrentPos());
	//
	//

	
	//
	//
	//�ЂƂ�ڂ����͎₵������ȁA�G�̕\�����s�����c�F
	//
	//
	int sizeOfEnemy = enemies.size();
	for(int i=0;i<sizeOfEnemy;++i){
		enemies[i]->Update();
		enemies[i]->Draw();
	}
	//
	//�G�͕�������̂ŁA�J��Ԃ��������Ӗ�����ufor�v�Ƃ����R�[�h���g�p���āA�����̓G��\�����܂��B
	//
	detector.CheckGeographAndEnemy();

	//�C�ɗ������玀�S
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




