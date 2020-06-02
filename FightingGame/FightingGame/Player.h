#pragma once

#include"CollidableActor.h"
#include"CommandInput.h"

#include<map>
#include<vector>
#include<functional>


struct KeyState;

enum ActionState {
	as_neutral,
	as_jump,
	as_crouch,
	as_tiltcrouch,
	as_punch,//�p���`
	as_kick//�L�b�N
};



class Hadoken;
class Enemy;
class Player : public CollidableActor
{

private:
	//���͏��
	//���Ƃ�include�͏���
	CommandInput _input;
	ActionCommand _lastcmd;
	//�Đ����[�l]�ɂȂ�܂��B
	struct Sequence {
		Sequence():pos(),duration(0),iv(){}
		Sequence(Position2 p, int dur,float vy=0.0f):iv() {
			pos = p;
			duration = dur;
			iv.y = vy;
			size.w = 40;
			size.h = 40;
			centerpos.x = size.w/2;
			centerpos.y = 40;
			_initfunction = []() {};
			_predrawfunction = []() {};
			_postdrawfunction = []() {};
		}
		Sequence(Position2 p,Size s, int dur, float vy = 0.0f) : iv(){
			pos = p;
			duration = dur;
			iv.y = vy;
			size.w = s.w;
			size.h = s.h;
			centerpos.x = size.w/2;
			centerpos.y = 40;
			_initfunction = []() {};
			_predrawfunction = []() {};
			_postdrawfunction = []() {};

		}
		Position2 pos;//������W
		Size size;
		Position2 centerpos;//�A�N�V�������S�_
		int duration;//�\���p������
		Vector2 iv;//�����x
		std::vector<HitRect> rects;//�����蔻��p
		std::function<void(void)> _initfunction;//�V�[�P���X�ؑ֎��ɌĂяo�����֐��I�u�W�F�N�g
		std::function<void(void)> _predrawfunction;//���t���[���`��O�ɌĂяo�����֐��I�u�W�F�N�g
		std::function<void(void)> _postdrawfunction;//���t���[���`���ɌĂяo�����֐��I�u�W�F�N�g

		///�V�[�P���X�ؑ֎��Ɉ�x�����֐��I�u�W�F�N�g���Ăяo��
		void FirstExecute() {
			_initfunction();
		}
		///�V�[�P���X�ؑ֎��Ɉ�x�����֐��I�u�W�F�N�g���Ăяo��
		void PreDrawExecute() {
			_predrawfunction();
		}
		void PostDrawExecute() {
			_postdrawfunction();
		}

		Sequence& SetInitialVector(Vector2& v){
			iv=v;
			return *this;
		}

		///�V�[�P���X�ؑ֎��Ɏ��s�����֐����Z�b�g����
		///@param f �V�[�P���X�ؑ֎��Ɏ��s�����֐�
		Sequence& SetInitialFunction(std::function<void(void)> f) {
			_initfunction = f;
			return *this;
		}

		///���t���[�����s�����֐����Z�b�g����
		///@param f �V�[�P���X�ؑ֎��Ɏ��s�����֐�
		Sequence& SetPreDrawFunction(std::function<void(void)> f) {
			_predrawfunction = f;
			return *this;
		}
		Sequence& SetPostDrawFunction(std::function<void(void)> f) {
			_postdrawfunction = f;
			return *this;
		}

		///�A�N�V�������S�_�̐ݒ�
		Sequence& SetActionCenter(int x, int y) {
			centerpos.x = x;
			centerpos.y = y;
			return *this;
		}

		///�������`��o�^���Ă���
		Sequence& AddRects(HitRect& hrc) {
			rects.push_back(hrc);
			return *this;
		}

		///�������`�̓o�^(�I�[�o�[���[�h)
		Sequence& AddRects(int x,int y , int w, int h ,RectType rt,CollidableActor* ca) {
			rects.push_back(HitRect(x,y,w,h,rt,ca));
			return *this;
		}
	};

	Hadoken* _hadoken = nullptr;


	Rect _baseCollider;

	std::map<InputId, std::vector<Player::Sequence>> _actions;//�A�N�V�������

	////�A�N�V�����f�[�^�̍쐬
	void CreateActionsData();
	void CreateAction(char direction,char button,Sequence s);
	void CreateAction(char direction, char button, unsigned char command ,Sequence s);

	//���͏�񂩂�InputId��Ԃ�
	InputId GetInputIdFromKeyState(const KeyState& key);

	//���ݍĐ����̃A�N�V����
	std::map<InputId, std::vector<Player::Sequence>>::iterator _currentActionIt;
	//���ݍĐ����̃V�[�P���X
	std::vector<Player::Sequence>::iterator _currentSequenceIt;
	bool _lastTurn;//�Z�J�n���̃^�[���t���O
	bool _jumpPrepare;
	int _attackTimer;//�U������
	ActionState _currentActionState;
	Enemy* _enemy;//�G�l�~�[�ւ̎Q��
	float _velY;//Y�������x
	float _velX;//X�������x
	int _walkFrame;//�����Ă�t���[��(�����A�j���[�V�����p)
	int _handle;//�摜�n���h��
	int _jumpSE;//�W�����v��SE
	Position2 _pos;//���ݍ��W
	int _hitStopTimer=0;
	int _effectFrame = 0;

	int _life;//���C�t

	void GotoAction(const KeyState& key);

	bool IsTurn()const;
	
	//�j���[�g����
	void Neutral(const KeyState& key);
	
	//�W�����v
	void Aerial(const KeyState& key);

	//���Ⴊ��
	void Crouch(const KeyState& key);

	//�U��
	void Attack(const KeyState& key);

	//�ړ�
	void Move(const KeyState& key);

	//�I�������p
	void PauseForEnd(const KeyState&);

	//���S
	void Dying(const KeyState& key);

	//�����o�֐��|�C���^
	void (Player::*_updater)(const KeyState& key);
	Rect _rc;

	void ShootHadoken();

	std::function<void(void)> _drawer;
	int _endTimer;
public:
	Player(int handle);
	~Player();
	void Update(const KeyState& key);
	void Update();
	const Position2& Position()const { return _pos; }
	void SetEnemy(Enemy* enemy);
	const Rect& GetRect()const;
	///@note �Q�ƂłȂ��āA�|�C���^��Ԃ��Ă�̂�
	///�k���|�C���^��Ԃ��K�v������ꍇ�����邽��
	std::vector<HitRect>* GetCurrentRects();
	void OnCollided(RectType rc);

	//���o�̂��߂̃|�[�Y
	void PauseForRendering();

	int GetLife()const { return _life; }

	Hadoken* GetMissile() { return _hadoken; }
};

