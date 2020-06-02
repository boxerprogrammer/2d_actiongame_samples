#pragma once
#include "character.h"
#include "../system/scoped_ptr.h"
class BaseDrawer;
class Camera;
//�󓙂̃A�C�e���Ɏg�p����A�����蔻�肠��̃L�����N�^�N���X
class GameItem : public Character
{
private:
	Rect _rect;
	ScopedPtr<BaseDrawer> _itemG;
	bool _visible;
	const Camera& _camera;
public:
	GameItem(const char* filename,Camera& camera,Rect& rect);
	virtual ~GameItem(void);
	void SetPosition(const PositionInt2D& pos);
	void Move(VectorInt2D& vec);
	void SetVisible(bool visible);
	const Rect& GetRect()const;
	void Draw();
};

