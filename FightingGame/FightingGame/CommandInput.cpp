#include "CommandInput.h"
#include<DxLib.h>


bool operator<(const InputId& lval, const InputId& rval) {
	return ((unsigned int)lval.direction | (unsigned int)(lval.button << 4)| (unsigned int)(lval.command<<10)) <
		((unsigned int)rval.direction | (unsigned int)(rval.button << 4) | (unsigned int)(rval.command << 10));
}

///�덷�͈͓̔����̃`�F�b�N
///@param data ����ׂ�����
///@param in ���͕���
///@return ���e�͈͓����H
bool CheckWithinMargin(DirectionState data, DirectionState in) {
	const DirectionState dss[] = { ds_leftdown,ds_down,ds_rightdown,
		ds_right,ds_rightup,ds_up,ds_leftup,ds_left };//12369874
	size_t size = sizeof(dss) / sizeof(DirectionState);
	auto it = std::find(dss, dss + size, data);
	int index = (it - dss) + size;
	return (dss[(index + 1) % size] == in || dss[(index - 1) % size] == in);
}

///�R���o�[�g�֐�
///�r�b�g������ԍ��ɕϊ�����(���]�Ή�)
///@param numpadbit �p�b�h���͂��r�b�g���������́B
///@param isTurn ���]�t���O
///@note ���͂����r�b�g�͉E1,��2,��4,��8�ł��B(0b0001,0b0010,0b0100,0b1000)
DirectionState ConvertDirectionalBitToDirectionState(unsigned char numpadbit,bool isTurn=false) {
	if (isTurn) {
		numpadbit = (0b10&numpadbit)>>1 | (0b1 & numpadbit) << 1 | (0b1100&numpadbit);
	}
	switch (numpadbit) {
	case 0b1010://����
		return ds_leftdown;
	case 0b1000://��
		return ds_down;
	case 0b1001://�E��
		return ds_rightdown;
	case 0b0001://�E
		return ds_right;
	case 0b0101://�E��
		return ds_rightup;
	case 0b0100://��
		return ds_up;
	case 0b0110://����
		return ds_leftup;
	case 0b0010://��
		return ds_left;
	default://����������ĂȂ�
		return ds_blank;
	}
}
#include<iostream>

using namespace std;



CommandInput::CommandInput(int handle):_handle(handle)
{
	_inputHistory.reserve(60*100);
	cout << _inputHistory.capacity() << endl;
	_angleTable.fill(0);
	_angleTable[ds_right] = DX_PI / 2.f;
	_angleTable[ds_rightup] = DX_PI / 4.f;
	_angleTable[ds_up] = 0.f;
	_angleTable[ds_leftup] = -DX_PI / 4.f;
	_angleTable[ds_left] = -DX_PI/2.0f;
	_angleTable[ds_leftdown] = -DX_PI*3.0f / 4.0f;
	_angleTable[ds_down] = DX_PI;
	_angleTable[ds_rightdown] = DX_PI*3.0f / 4.0f;

	std::vector<CommandData> hadoken;
	hadoken.push_back(CommandData(ds_down));
	hadoken.push_back(CommandData(ds_rightdown));
	hadoken.push_back(CommandData(ds_right));
	_punchCommands[cmd_hadoken] = hadoken;

	_kickCommands[cmd_tornadokick].push_back(CommandData(ds_down));
	_kickCommands[cmd_tornadokick].push_back(CommandData(ds_leftdown));
	_kickCommands[cmd_tornadokick].push_back(CommandData(ds_left));

	CommandData shoryu[] = { CommandData(ds_right),CommandData(ds_down),CommandData(ds_rightdown) };
	std::copy(shoryu,shoryu+sizeof(shoryu)/sizeof(CommandData),
		std::back_inserter(_punchCommands[cmd_shoryuken]));


	_punchCommands[cmd_psycho].push_back(CommandData(ds_left, 60));
	_punchCommands[cmd_psycho].push_back(CommandData(ds_right));

	_kickCommands[cmd_summer].push_back(CommandData(ds_down, 60));
	_kickCommands[cmd_summer].push_back(CommandData(ds_up));

}


CommandInput::~CommandInput()
{

}

InputInfo
CommandInput::GetInputIdFromKeyState(int inputstate) {
	InputInfo ret = {};
	InputPart& id=ret.part;//�߂�l�p
	
	if (inputstate&PAD_INPUT_1) {
		id.LP = 1;//��p���`�̃r�b�g�t���O
	}
	else if (inputstate&PAD_INPUT_2) {
		id.LK = 1;//��L�b�N�̃r�b�g�t���O
	}
	char numpadbit = 0;//�㉺���E�Ńr�b�g���Ă�
					   //�E1,��2,��4,��8
	if (inputstate&PAD_INPUT_RIGHT) {
		numpadbit |= 1;
	}
	if (inputstate&PAD_INPUT_LEFT) {
		numpadbit |= 2;
	}
	if (inputstate&PAD_INPUT_UP) {
		numpadbit |= 4;
	}
	if (inputstate&PAD_INPUT_DOWN) {
		numpadbit |= 8;
	}
	id.direction = numpadbit;
	return ret;
}
ActionCommand 
CommandInput::CheckInput(bool isTurn){
	ActionCommand retcmd = cmd_none;
	InputInfo input={};
	///�L�[�{�[�h�C�x���g���擾
	char keystate[256];
	//DxLib::GetHitKeyStateAll(keystate);
	int inputstate = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	input = GetInputIdFromKeyState(inputstate);
	DirectionState ds = ConvertDirectionalBitToDirectionState(input.part.direction, isTurn);
	if (!_inputHistory.empty()) {
		InputInfo& last = _inputHistory.back();
		std::vector<CommandData>::reverse_iterator wazaRit;
		std::vector<CommandData>::reverse_iterator wazaRend;
		Commands_t* commands=nullptr;
		if (input.part.LP && !last.part.LP) {
			commands= &_punchCommands;
		}
		else if (input.part.LK && !last.part.LK) {
			commands = &_kickCommands;
		}
		if(commands!=nullptr){
			for(auto& command : *commands){
				wazaRit = command.second.rbegin();
				wazaRend = command.second.rend();
				if (ds == wazaRit->dir || ds==ds_blank || CheckWithinMargin(wazaRit->dir, ds)) {
					std::vector<InputInfo>::reverse_iterator rit = _inputHistory.rbegin();
					for (; wazaRit != wazaRend; ++wazaRit) {
						DirectionState lds = ConvertDirectionalBitToDirectionState(rit->part.direction, isTurn);
						if (lds != wazaRit->dir) {
							if (CheckWithinMargin(wazaRit->dir,lds)||lds==ds_blank) {
								if (++rit == _inputHistory.rend())break;
								if (ConvertDirectionalBitToDirectionState(rit->part.direction, isTurn) != wazaRit->dir) {
									break;
								}
								if (rit->part.length < wazaRit->length) {
									if (++rit == _inputHistory.rend())break;
									if (ConvertDirectionalBitToDirectionState(rit->part.direction, isTurn) != wazaRit->dir) {
										break;
									}
								}
							}
							else {
								break;
							}
						}
						if (++rit == _inputHistory.rend())break;
					}
					if (wazaRit == wazaRend) {
						retcmd = command.first;
						break;
					}
				}
			}
		}
		
		if ((last.whole & 0x3ff )== (input.whole & 0x3ff)) {
			if (last.part.length < 0b111111) {
				++last.part.length;
				return cmd_none;
			}
		}
	}

	if (_inputHistory.size() == _inputHistory.capacity()) {
		_inputHistory.erase(_inputHistory.begin());
	}
	++input.part.length;
	_inputHistory.push_back(input);
	return retcmd;
}

const std::vector<InputInfo>& 
CommandInput::GetInputHistory()const{
	return _inputHistory;
}


InputInfo 
CommandInput::CurrentInput(){
	return _inputHistory.back();
}

InputInfo
CommandInput::LastInput() {
	auto rit= _inputHistory.rbegin();
	if (_inputHistory.size() > 1 && rit->part.length <= 1) {
			return *(++rit);
	}
	return *(rit);
}

InputInfo 
CommandInput::IsTrigger() {
	InputInfo ret = {};
	ret.whole = ~LastInput().whole&CurrentInput().whole;
	return ret;
}

void 
CommandInput::Draw(){
	static float angle=0;
	InputInfo input= CurrentInput();
	DirectionState ds = ConvertDirectionalBitToDirectionState(input.part.direction);
	if (ds == ds_blank || ds == ds_none) {
		DrawRectRotaGraph(300, 420, 8, 540, 9, 9, 2.0f, 0.0f, _handle, true, false);
	}else {
		angle = _angleTable[ds];
		DrawRectRotaGraph(300, 420, 6, 555, 13, 13, 2.0f, angle, _handle, true, false); 
	}

	InputInfo lastInfo = {};
	lastInfo.part.direction = ds_none;
	int count = 0;
	std::vector<InputInfo>::reverse_iterator rit = _inputHistory.rbegin();
	for (; rit != _inputHistory.rend(); ++rit) {
		if (count > 25)continue;
		if (lastInfo.part.direction == rit->part.direction) {
			continue;
		}
		int px = 630 - 15 * 2 *count;//630���獶�ɕ`�悵�Ă���
		ds= ConvertDirectionalBitToDirectionState(rit->part.direction);
		if (ds == ds_blank || ds == ds_none) {
			DrawRectRotaGraph(px, 440, 8, 540, 9, 9, 2.0f, 0.0, _handle, true, false);
		}
		else {
			DrawRectRotaGraph(px, 440, 6, 555, 13, 13, 2.0f,
				_angleTable[ds],
				_handle, true, false);
		}
		lastInfo = *rit;
		++count;
	}
}
