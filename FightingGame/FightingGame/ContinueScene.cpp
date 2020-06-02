#include "ContinueScene.h"
#include<DxLib.h>
#include"CharacterSelectScene.h"
#include"GameoverScene.h"
#include"GameMain.h"

unsigned int rights[] = {120,237,351,471,600,115,234,358,450,600 };

using namespace std;

ContinueScene::ContinueScene()
{
	_countdown=9;
	_timer=60;
	_continueNumberSpriteH = DxLib::LoadGraph("img/continue_number_sprite2.png");
	for (int i = 0; i < sizeof(rights) / sizeof(rights[0]); ++i) {
		if (i > 4) {
			_layouts.push_back(make_pair(Position2(i != 5 ? rights[i - 1] : 0, 122), Size(i != 5 ? rights[i] - rights[i - 1] : rights[i], 122)));
		}
		else {
			_layouts.push_back(make_pair(Position2(i != 0 ? rights[i - 1] : 0, 0), Size(i != 0 ? rights[i] - rights[i - 1] : rights[i], 122)));
		}
	}
}


ContinueScene::~ContinueScene()
{
	DeleteGraph(_continueNumberSpriteH);
}


void
ContinueScene::Update(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	char str[] = "コンティニュー 00";
	if (key.state[KEY_INPUT_RETURN] && !key.lastState[KEY_INPUT_RETURN]) {
		GameMain::Instance().ChangeScene(new CharacterSelectScene());
		return;
	}
	else if (key.state[KEY_INPUT_SPACE] && !key.lastState[KEY_INPUT_SPACE]) {
		--_countdown;
		_timer = 60;
	}
	if (--_timer == 0) {
		--_countdown;
		_timer = 60;
	}

	if (_countdown < 0) {
		GameMain::Instance().ChangeScene(new GameoverScene());
		_timer = 300;//5秒でタイトルへ
		return;
	}
	sprintf(str, "コンティニュー %02d", _countdown);
	DxLib::DrawFormatString(100, 400,0xffffffff, "%s", str);

	auto layout = _layouts[9 - _countdown];
	Size& size = layout.second;
	Position2& p = layout.first;
	const Size& ws = GameMain::Instance().WindowSize();
	DrawRectGraph((ws.w - size.w) / 2, (ws.h - size.h) / 2, p.x, p.y, size.w, size.h, _continueNumberSpriteH,true,false);
}
