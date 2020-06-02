#pragma once
#include "Scene.h"

struct Vector2;

class CharacterSelectScene :
	public Scene
{
private:
	int _selectedIndex;//現在選択中のインデックス
	int _charSelBgH = 0;
	int _charSelH = 0;
	int _selectedH = 0;
	int _fadeTimer = 0;

	int _selectSEH = 0;
	int _selectedSEH = 0;

	//ポートレート画像配列
	int _portraitsH[19] = {};
	void DrawCharacterSelectParts(int handle, bool transparent);
	void DrawPortrait(int handle,const Vector2& vec);

	void (CharacterSelectScene::*_updater)(const KeyState& key);
	void FadeInUpdate(const KeyState& key);
	void NormalUpdate(const KeyState& key);
	void FadeOutUpdate(const KeyState& key);

public:
	CharacterSelectScene();
	~CharacterSelectScene();
	void Update(const KeyState& state);
};

