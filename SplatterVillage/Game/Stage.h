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

//ステージ情報、ゲームの進行を管理
class Stage
{
	private:
		///FMFヘッダ用構造体
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

		//敵たち
		typedef std::vector<SharedPtr<Enemy> > enemies_t;
		enemies_t enemies;//OCの都合上アンダーバーは消しています

		//ハシゴたち
		typedef std::vector<SharedPtr<Ladder> > ladders_t;
		ladders_t ladders;//OCの都合上アンダーバーは消しています


		Enemy* _boss;//ボスへの弱参照

		typedef std::vector<LayerData> layers_t ;
		layers_t data;//OCの都合上アンダーバーは消しています
		
		ScopedPtr<TileDrawer> _cloud;
		ScopedPtr<TileDrawer> _terra;
		ScopedPtr<BaseDrawer> _far;
		ScopedPtr<BaseDrawer> _tree;
		ScopedPtr<BaseDrawer> _door_0;
		ScopedPtr<BaseDrawer> _door_1;
		ScopedPtr<BaseDrawer> _door_2;
		ScopedPtr<BaseDrawer> _ladder;
		BaseDrawer* _door;
		ScopedPtr<Player> player;//OCの都合上アンダーバーは消しています
		ScopedPtr<Camera> camera;//OCの都合上アンダーバーは消しています
		ScopedPtr<GameItem> _key;

		//背景の表示を行う
		void DrawBackground();

		//チップ背景、チップ地形にの表示を行う。
		void DrawTerrainChips(int offestx,int offsety , int endX);

		PositionInt2D _croudScroll;
		PositionInt2D _position;
		int _chiphandle;
		int _count;
		bool _switch;
		ScopedPtr<EnemyFactory> _enemyCreator;
		
		//１画面上に表示できるチップの最後のインデクスを返す
		int GetEndXIdx(layers_t& layers,int offsetx);
		void DrawDecoratedBackground(layers_t& layers,
			int layerNo,
			int offsetx,
			int offsety,
			int endX);

		///フロント側(チップより手前のオブジェクト)を描画します
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
		//プレイヤーの状態を知らせるために読み取り専用の参照のみかえす
		const Player& GetPlayer() const;
		void OCUpdate();
		void Update();
		void StageClearAction();
		bool IsClear()const;
		bool EndOfClearAction()const;
};

