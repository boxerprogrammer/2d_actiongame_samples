#pragma once
#include<vector>
#include<array>
#include<map>

//�����L�[
enum DirectionState {
	ds_none,//���o�[������ĂȂ�
	ds_leftdown,//1
	ds_down,//2
	ds_rightdown,//3
	ds_left,//4
	ds_blank,//5(����)
	ds_right,//6
	ds_leftup,//7
	ds_up,//8
	ds_rightup,//9
};

struct CommandData {
	CommandData() :dir(ds_none), length(0) {}
	CommandData(DirectionState d, unsigned int l=0) :dir(d), length(l) {}
	DirectionState dir;
	unsigned int length;
};


///�A�N�V�����R�}���h
enum ActionCommand {
	cmd_none,//�R�}���h�����Ȃ�
	cmd_shoryuken,//������
	cmd_hadoken,//�g����
	cmd_tornadokick,//���������r
	cmd_psycho,//�T�C�R�N���b�V���[
	cmd_summer//�T�}�\
};

//���͏��[�L�[]�ɂȂ�܂��B
struct InputId {
	InputId() :direction(0), button(0),command(0) {}
	InputId(char dir, char but,unsigned char cmd=0) {
		direction = dir;
		button = but;
		command = cmd;
	}
	char direction;//�����L�[
	char button;//�{�^������
	unsigned char command;//�����ς݃R�}���h
};

///�Z���}�b�v�ɓo�^���邽�߂̃I�y���[�^�I�[�o�[���[�h
bool operator<(const InputId& lval, const InputId& rval);


///���͏��
///��C����ł�unsigned short��2�o�C�g��\���܂��B
struct InputPart {
	unsigned short direction : 4;//�����L�[
	unsigned short LP : 1;//��p���`
	unsigned short MP : 1;//���p���`
	unsigned short SP : 1;//���p���`
	unsigned short LK : 1;//��L�b�N
	unsigned short MK : 1;//���L�b�N
	unsigned short SK : 1;//���L�b�N
	unsigned short length : 6;//0�`63�̐�
};

///�܂Ƃ߂Ĉ������������߂̋��p��
union InputInfo {
	unsigned short whole;//�܂Ƃ߂Ĉ����p
	InputPart part;//�������p
};

///���́̃R�}���h�܂����܂Ƃ߂�
///�N���X
class CommandInput
{
private:
	int _handle;

	///���͏��z��(���͗���)
	std::vector<InputInfo> _inputHistory;
	InputInfo GetInputIdFromKeyState(int inputstate);
	std::array<float, 10> _angleTable;

	typedef std::map<ActionCommand, std::vector<CommandData>> Commands_t;
	Commands_t _punchCommands;
	Commands_t _kickCommands;

	InputInfo LastInput();
public:
	CommandInput(int handle);
	~CommandInput();
	///���͏󋵂��`�F�b�N��_input�Ɋi�[
	///���o�g�������t���[���Ăт܂�
	ActionCommand CheckInput(bool isTurn = false);

	///���͏��z���Ԃ�
	const std::vector<InputInfo>& GetInputHistory()const;
	
	///����(���t���[��)�̓��͏󋵂�
	///�Ԃ�
	InputInfo CurrentInput();
	InputInfo IsTrigger();

	///���͏󋵂�\������
	void Draw();

	///���͗����̃N���A
	void Clear();

	///���͗����̃Z�[�u
	void SaveInputHistory();

	///���͗����̃��[�h
	void LoadInputHistory();
};

