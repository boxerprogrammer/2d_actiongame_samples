#pragma once

#include "../Geometry/geometry.h"

const float PI = 3.14159265;

inline float DegreeToRadian(float degree){
	return degree*2.f*PI/360.f;
}


inline float RadianToDigree(float radian){
	return radian*360.f/2.f/PI;
}




///モデルオブジェクト
class Model3D{
private:
	bool _isAutoModel;//自動で破棄(つまり自殺)するべきモデルかどうか(trueならば自動で死ぬ)
	int _handle;//３Dモデルハンドル
	//@note アニメーション番号とアタッチ番号は別モノなので注意するように
	int _currentAnimNo;//現在再生中のアニメーション番号
	int _attachedAnimNo;//現在のアタッチ番号
	float _animTime;//アニメーションの現在地点(時間tね木名)
	float _duration;//アニメーションの尺
	float _animSpeed;
	Vector3D _scale;//拡大縮小率
	Vector3D _pos;//画面表示位置
	Vector3D _vel;//モデル速度
	bool _loop;//アニメーションがループするか
	void Init(int handle);
public:
	Model3D(const char* path);
	Model3D(int handle);
	~Model3D();
	int Handle()const{
		return _handle;
	}
	
	///アニメーションを変更する(名前指定Ver)
	///@param animname 変更先アニメーション名
	void ChangeAnimation(const char* animname,bool loop=false);
	bool IsAnimEnd() const{
		return _animTime>_duration;
	}
	///アニメーションを変更する(インデックス指定Ver)
	///@param animNo 変更先アニメーションインデックス
	void ChangeAnimation(const int animNo,bool loop=false);
	void SetVelocity(const Vector3D& vel){
		_vel = vel;
	}

	void ExecuteVelocity(){
		Move(_vel);
	}

	Vector3D Position()const;
	///位置を変更する
	void SetPosition(const Vector3D& pos);

	///現在位置から移動する
	void Move(Vector3D& mov){
		_pos.x+=mov.x;
		_pos.y+=mov.y;
		_pos.z+=mov.z;
		SetPosition(_pos);
	}

	///現在のXYZ回転情報を得る
	Vector3D Rotation()const;

	///XYZ軸どれかで回転
	///@param vec それぞれの軸でどれくらい回転させたいかを指定する
	///@attention 単位はラジアン
	void RotateXYZ(Vector3D& vec);

	///X軸まわりの回転
	///@attention 単位はラジアンだぞ
	void RotateX(float radian){
		RotateXYZ(Vector3D(radian,0.f,0.f));
	}

	///Y軸まわりの回転
	///@attention 単位はラジアンだぞ
	void RotateY(float radian){
		RotateXYZ(Vector3D(0.f,radian,0.f));
	}

	///Z軸まわりの回転
	///@attention 単位はラジアンだぞ
	void RotateZ(float radian){
		RotateXYZ(Vector3D(0.f,0.f,radian));
	}

	///均等拡大縮小を行う
	void SetScale(float scale);
	
	///他のモデルと動きを合わせる
	///@param model シンクロしたいモデル
	///@note 特定のモデルの(変身)に対応するため
	void Syncronize(const Model3D& model);

	///@usage 毎フレームこの関数をコールしてください。
	void Update(bool isDraw=true);

	void SetAnimSpeed(float speed){
		_animSpeed=speed;
	}
};