#include "CharacterSelectScene.h"
#include<DxLib.h>
#include"GameMain.h"
#include"BeforeBattle.h"

int cx = 0, cy = 0;
const int cw = 82, ch = 66;

const Vector2 _characterSelectPositions[19] = {
	{ 424,32 },//ゴウキ,
	{ 494,14 },//ユン,
	{ 544,46 },//リュウ,//左下(-70,+24),下(-20,+54)
	{ 404,88 },//ユリアン,
	{ 474,68 },//レミー
	{ 524,100 }, //オロ
	{ 384,142 },//ネクロ
	{ 454,120 },//Q
	{ 504,154 },//ダッドリー
	{ 364,196 },//いぶき
	{ 434,174 },//春麗
	{ 484,208 },//エレナ//左下(-70,+24),下(-20,+54)
	{ 344,248 },//ショーン
	{ 414,230 },//まこと
	{ 464,262 },//ヒューゴー
	{ 324,300 },//アレックス
	{ 394,282 },//12
	{ 444,316 },//ケン
	{ 374,336 },//ヤン
};

const Vector2 _portraitOffset[19] = {
	{ -10,0 },//ゴウキ,
	{ 0,0 },//ユン,
	{ 0,0 },//リュウ,//左下(-70,+24),下(-20,+54)
	{ 0,0 },//ユリアン,
	{ 0,0 },//レミー
	{ 0,0 }, //オロ
	{ -100,0 },//ネクロ
	{ 0,0 },//Q
	{ 0,0 },//ダッドリー
	{ 0,0 },//いぶき
	{ 0,0 },//春麗
	{ 0,0 },//エレナ//左下(-70,+24),下(-20,+54)
	{ 0,0 },//ショーン
	{ 0,0 },//まこと
	{ 0,0 },//ヒューゴー
	{ 0,0 },//アレックス
	{ 0,0 },//12
	{ 0,0 },//ケン
	{ 0,0 },//ヤン
};

//キャラセレテーブル
const CharaType _charLayout[7][3] = { { ct_gouki,ct_yun,ct_ryu },
										{ ct_urien,ct_remy,ct_oro },
										{ ct_necro,ct_q,ct_dudley },
										{ ct_ibuki,ct_chunli,ct_elena },
										{ ct_sean,ct_makoto,ct_hugo },
										{ ct_alex,ct_twelve,ct_ken },
										{ ct_invalid,ct_yang,ct_invalid } 
									};

int _selectedM;
int _lightH;
CharacterSelectScene::CharacterSelectScene()
{
	srand(DxLib::GetNowCount());
	_updater = &CharacterSelectScene::FadeInUpdate;
	_fadeTimer = 0;
	_selectedIndex=ct_ryu;
	_charSelH = DxLib::LoadGraph("img/character_select.png");
	_charSelBgH = DxLib::LoadGraph("img/charsel_bg.png");
	_selectedH = DxLib::LoadGraph("img/selected.png");
	_selectedM=DxLib::LoadMask("img/selected_mask.png");
	_lightH = DxLib::LoadGraph("img/light.png");

	_portraitsH[ct_gouki] = DxLib::LoadGraph("img/portgouki.png");
	_portraitsH[ct_yun] = DxLib::LoadGraph("img/portyun.png");
	_portraitsH[ct_ryu] = DxLib::LoadGraph("img/portryu.png");
	_portraitsH[ct_urien] = DxLib::LoadGraph("img/porturien.png");
	_portraitsH[ct_remy] = DxLib::LoadGraph("img/portremy.png");
	_portraitsH[ct_oro] = DxLib::LoadGraph("img/portoro.png");
	_portraitsH[ct_necro] = DxLib::LoadGraph("img/portnecro.png");
	_portraitsH[ct_q] = DxLib::LoadGraph("img/portq.png");
	_portraitsH[ct_dudley] = DxLib::LoadGraph("img/portdudley.png");
	_portraitsH[ct_ibuki] = DxLib::LoadGraph("img/portibuki.png");
	_portraitsH[ct_chunli] = DxLib::LoadGraph("img/portchunli.png");
	_portraitsH[ct_elena] = DxLib::LoadGraph("img/portelena.png");
	_portraitsH[ct_sean] = DxLib::LoadGraph("img/portsean.png");
	_portraitsH[ct_makoto] = DxLib::LoadGraph("img/portmakoto.png");
	_portraitsH[ct_hugo] = DxLib::LoadGraph("img/porthugo.png");
	_portraitsH[ct_alex] = DxLib::LoadGraph("img/portalex.png");
	_portraitsH[ct_twelve] = DxLib::LoadGraph("img/porttwelve.png");
	_portraitsH[ct_ken] = DxLib::LoadGraph("img/portken.png");
	_portraitsH[ct_yang] = DxLib::LoadGraph("img/portyang.png");

	_selectSEH = DxLib::LoadSoundMem("se/select_se.mp3");
	_selectedSEH = DxLib::LoadSoundMem("se/selected_se.mp3");
}


CharacterSelectScene::~CharacterSelectScene()
{
	for (auto handle : _portraitsH) {
		DxLib::DeleteGraph(handle);
	}
	DxLib::DeleteGraph(_charSelH);
	DxLib::DeleteGraph(_charSelBgH);
	DxLib::DeleteGraph(_selectedH);
	
	DxLib::DeleteSoundMem(_selectSEH);
	DxLib::DeleteSoundMem(_selectedSEH);
}


void 
CharacterSelectScene::DrawCharacterSelectParts(int handle, bool transparent) {
	const Size& screensize=GameMain::Instance().WindowSize();
	DrawExtendGraph(0, 0, screensize.w, screensize.h, handle, transparent);

}

void 
CharacterSelectScene::DrawPortrait(int handle, const Vector2& vec) {
	const Size& screensize = GameMain::Instance().WindowSize();

	const int portCenterX = 100;
	const int portScale = 2;
	Size portsize;
	GetGraphSize(handle, &portsize.w, &portsize.h);
	Vector2 portpos;
	portpos.x = (screensize.w - portsize.w * portScale) / 2 - portCenterX + vec.x;
	DrawExtendGraph(portpos.x, screensize.h - portsize.h * portScale, portpos.x + portsize.w * portScale, screensize.h, handle, true);

}


void
CharacterSelectScene::Update(const KeyState& key) {
	(this->*_updater)(key);
}

void 
CharacterSelectScene::FadeInUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int brightness = (255 * _fadeTimer) / 60;
	DrawCharacterSelectParts(_charSelBgH, false);
	//ポートレイト描画
	DrawPortrait(_portraitsH[_selectedIndex],_portraitOffset[_selectedIndex] );
	DrawCharacterSelectParts(_charSelH, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (++_fadeTimer == 60) {
		SetUseMaskScreenFlag(false);
		FillMaskScreen(0);
		_updater = &CharacterSelectScene::NormalUpdate;
		_selectedIndex = ct_ryu;
	}
}
void 
CharacterSelectScene::NormalUpdate(const KeyState& key) {
	if (key.state[KEY_INPUT_RETURN] && !key.lastState[KEY_INPUT_RETURN]) {
		_updater = &CharacterSelectScene::FadeOutUpdate;
		_fadeTimer = 60;
		DxLib::PlaySoundMem(_selectedSEH, DX_PLAYTYPE_BACK);
	}
	DrawCharacterSelectParts(_charSelBgH, false);

	//ポートレイト描画
	DrawPortrait(_portraitsH[_selectedIndex], _portraitOffset[_selectedIndex]);

	DrawCharacterSelectParts(_charSelH, true);

	const Vector2& v = _characterSelectPositions[_selectedIndex];

	static float angle=0.0f;
	//マスク
	SetUseMaskScreenFlag(true);
	FillMaskScreen(1);
	DrawMask(v.x,v.y,_selectedM, DX_MASKTRANS_WHITE);
	SetDrawBlendMode(DX_BLENDMODE_ADD,255);
	DrawRotaGraph2(v.x+36,v.y+32,0,32,1.0f,angle,_lightH,false,false);
	DrawRotaGraph2(v.x + 36, v.y + 32, 0, 32, 1.0f, angle+4.0f/3.0f*DX_PI, _lightH, false, false);
	DrawRotaGraph2(v.x + 36, v.y + 32, 0, 32, 1.0f, angle+2.0f/3.0f*DX_PI, _lightH, false, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetUseMaskScreenFlag(false);
	angle+=0.08f;

	//カーソル描画
	//DrawExtendGraph(v.x, v.y, v.x + cw, v.y + ch, _selectedH, true);

	

	int portnum = sizeof(_portraitsH) / sizeof(int);
	static int idxX = 2;
	static int idxY = 0;
	if (key.state[KEY_INPUT_LEFT] && !key.lastState[KEY_INPUT_LEFT]) {
		if (_selectedIndex == ct_yang) {
			idxX = 1;
			idxY = 0;
		}
		else {
			if (idxX == 2) {
				idxY++;
			}
			idxX = (idxX - 1)<0 ? 2 : (idxX - 1);
		}
		DxLib::PlaySoundMem(_selectSEH,DX_PLAYTYPE_BACK);
	}
	if (key.state[KEY_INPUT_RIGHT] && !key.lastState[KEY_INPUT_RIGHT]) {
		if (_selectedIndex == ct_yun) {
			idxX = 1;
			idxY = 6;
		}
		else {
			if (idxX == 1) {
				idxY--;
			}
			idxX = (idxX + 1) % 3;
		}
		DxLib::PlaySoundMem(_selectSEH, DX_PLAYTYPE_BACK);
	}
	if (key.state[KEY_INPUT_UP] && !key.lastState[KEY_INPUT_UP]) {
		if (idxX == 1) {
			idxY = (idxY - 1 + 7) % 7;
		}
		else {
			idxY = (idxY - 1 + 6) % 6;
		}
		DxLib::PlaySoundMem(_selectSEH, DX_PLAYTYPE_BACK);
	}
	if (key.state[KEY_INPUT_DOWN] && !key.lastState[KEY_INPUT_DOWN]) {
		if (idxX == 1) {
			idxY = (idxY + 1) % 7;
		}
		else {
			idxY = (idxY + 1) % 6;
		}
		DxLib::PlaySoundMem(_selectSEH, DX_PLAYTYPE_BACK);
	}
	_selectedIndex = _charLayout[idxY][idxX];

	int xpos, ypos;
	DxLib::GetMousePoint(&xpos, &ypos);
	DxLib::DrawFormatString(0, 0, 0xffffffff, "X=%d,Y=%d", xpos, ypos);

}


void
CharacterSelectScene::FadeOutUpdate(const KeyState& key) {
	GameMain& gm = GameMain::Instance();
	int brightness = (255 * _fadeTimer) / 60;
	DrawCharacterSelectParts(_charSelBgH, false);
	//ポートレイト描画
	DrawPortrait(_portraitsH[_selectedIndex], _portraitOffset[_selectedIndex]);
	DrawCharacterSelectParts(_charSelH, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 0);
	DxLib::DrawBox(0, 0, gm.WindowSize().w, gm.WindowSize().h, GetColor(brightness, brightness, brightness), true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (--_fadeTimer == 0) {
		GameMain::Instance().ChangeScene(new BeforeBattle(_selectedIndex,rand()%ct_invalid));
	}
}
