#pragma once
#include<vector>
#include<array>
#include<map>

//方向キー
enum DirectionState {
	ds_none,//レバー押されてない
	ds_leftdown,//1
	ds_down,//2
	ds_rightdown,//3
	ds_left,//4
	ds_blank,//5(無効)
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


///アクションコマンド
enum ActionCommand {
	cmd_none,//コマンド成立なし
	cmd_shoryuken,//昇竜拳
	cmd_hadoken,//波動拳
	cmd_tornadokick,//竜巻旋風脚
	cmd_psycho,//サイコクラッシャー
	cmd_summer//サマソ
};

//入力情報[キー]になります。
struct InputId {
	InputId() :direction(0), button(0),command(0) {}
	InputId(char dir, char but,unsigned char cmd=0) {
		direction = dir;
		button = but;
		command = cmd;
	}
	char direction;//方向キー
	char button;//ボタン入力
	unsigned char command;//成立済みコマンド
};

///技をマップに登録するためのオペレータオーバーロード
bool operator<(const InputId& lval, const InputId& rval);


///入力情報
///※C言語ではunsigned shortは2バイトを表します。
struct InputPart {
	unsigned short direction : 4;//方向キー
	unsigned short LP : 1;//弱パンチ
	unsigned short MP : 1;//中パンチ
	unsigned short SP : 1;//強パンチ
	unsigned short LK : 1;//弱キック
	unsigned short MK : 1;//中キック
	unsigned short SK : 1;//強キック
	unsigned short length : 6;//0～63の数
};

///まとめて扱いたいがための共用体
union InputInfo {
	unsigned short whole;//まとめて扱う用
	InputPart part;//一つ一つ扱う用
};

///入力⇔コマンドまわりをまとめた
///クラス
class CommandInput
{
private:
	int _handle;

	///入力情報配列(入力履歴)
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
	///入力状況をチェックし_inputに格納
	///※バトル中毎フレーム呼びます
	ActionCommand CheckInput(bool isTurn = false);

	///入力情報配列を返す
	const std::vector<InputInfo>& GetInputHistory()const;
	
	///現在(現フレーム)の入力状況を
	///返す
	InputInfo CurrentInput();
	InputInfo IsTrigger();

	///入力状況を表示する
	void Draw();

	///入力履歴のクリア
	void Clear();

	///入力履歴のセーブ
	void SaveInputHistory();

	///入力履歴のロード
	void LoadInputHistory();
};

