#pragma once
#include"../Geometry/geometry.h"
///�A�j���[�V����������Ȃɂ�
class Character
{
public:
	Character(void);
	~Character(void);
};

enum CharacterDirection{
	dir_none,
	dir_right,
	dir_left,
	dir_up,
	dir_down
};