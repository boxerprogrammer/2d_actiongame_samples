#pragma once
#include"../Geometry/geometry.h"
#include"../system/scoped_ptr.h"
#include"../system/shared_ptr.h"
#include<vector>
#include<map>
#include<string>

class Ladder;
class TileDrawer;
class BaseDrawer;
class Player;
class Camera;
class Enemy;
class EnemyFactory;
class CollisionDetector;
class GameItem;

//�X�e�[�W���A�Q�[���̐i�s���Ǘ�
class Stage
{
	private:
		///FMF�w�b�_�p�\����
		struct FMFHeader{
			char signature[4];
			int datasize;
			int width;
			int height;
			unsigned char chipW;
			unsigned char chipH;
			unsigned char numLayers;
			unsigned char bitCount;
		};
		typedef std::vector<unsigned char> chips_t;
		struct ChipLine{
			chips_t chips;
		};
		
		typedef std::vector<ChipLine> chipline_t;

		struct LayerData{
			chipline_t lines;
		};
		FMFHeader _header;

		//�G����
		typedef std::vector<SharedPtr<Enemy> > enemies_t;
		enemies_t enemies;//OC�̓s����A���_�[�o�[�͏����Ă��܂�

		//�n�V�S����
		typedef std::vector<SharedPtr<Ladder> > ladders_t;
		ladders_t ladders;//OC�̓s����A���_�[�o�[�͏����Ă��܂�


		Enemy* _boss;//�{�X�ւ̎�Q��

		typedef std::vector<LayerData> layers_t ;
		layers_t data;//OC�̓s����A���_�[�o�[�͏����Ă��܂�
		
		ScopedPtr<TileDrawer> _cloud;
		ScopedPtr<TileDrawer> _terra;
		ScopedPtr<BaseDrawer> _far;
		ScopedPtr<BaseDrawer> _tree;
		ScopedPtr<BaseDrawer> _door_0;
		ScopedPtr<BaseDrawer> _door_1;
		ScopedPtr<BaseDrawer> _door_2;
		ScopedPtr<BaseDrawer> _ladder;
		BaseDrawer* _door;
		ScopedPtr<Player> player;//OC�̓s����A���_�[�o�[�͏����Ă��܂�
		ScopedPtr<Camera> camera;//OC�̓s����A���_�[�o�[�͏����Ă��܂�
		ScopedPtr<GameItem> _key;

		//�w�i�̕\�����s��
		void DrawBackground();

		//�`�b�v�w�i�A�`�b�v�n�`�ɂ̕\�����s���B
		void DrawTerrainChips(int offestx,int offsety , int endX);

		PositionInt2D _croudScroll;
		PositionInt2D _position;
		int _chiphandle;
		int _count;
		bool _switch;
		ScopedPtr<EnemyFactory> _enemyCreator;
		
		//�P��ʏ�ɕ\���ł���`�b�v�̍Ō�̃C���f�N�X��Ԃ�
		int GetEndXIdx(layers_t& layers,int offsetx);
		void DrawDecoratedBackground(layers_t& layers,
			int layerNo,
			int offsetx,
			int offsety,
			int endX);

		///�t�����g��(�`�b�v����O�̃I�u�W�F�N�g)��`�悵�܂�
		void DrawFrontEventObjects(layers_t& layers,
			int layerNo,
			int offsetx,
			int offsety,
			int endX);

		void UpdateDetectableEnemies(CollisionDetector& detector,
			enemies_t& enemies,
			const PositionInt2D& playerPosition);
		void UpdateDetectableChips(CollisionDetector& detector,
			LayerData& layer,
			int offsetx,
			int offsety,
			int endX,
			PositionInt2D& playerPosition);

		void EnemiesInitialize();
		
		void LaddersInitialize();
		void UpdateLadders(CollisionDetector& detector,ladders_t& ladders,const PositionInt2D& playerpos);
		
		int _keyY;
		Vector2D<float> _keyVec;
		void (Stage::*_clearFunc)();
		void KeyStart();
		void KeyAction();
		void KeyGround();
		bool _endOfClearAction;
		void DoorOpen();
		int _clearActionTimer;
		int _columbia;
	public:
		Stage(void);
		~Stage(void);
		//�v���C���[�̏�Ԃ�m�点�邽�߂ɓǂݎ���p�̎Q�Ƃ݂̂�����
		const Player& GetPlayer() const;
		void OCUpdate();
		void Update();
		void StageClearAction();
		bool IsClear()const;
		bool EndOfClearAction()const;
};

