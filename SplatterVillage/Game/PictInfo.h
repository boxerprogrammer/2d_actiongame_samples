#pragma once

struct PictInfo{
	int num;//�A�j���[�V�����̍ۂɉ���������Ă���̂�
	int handle;//�摜�̃n���h��
	int interval;//�C���^�[�o��
	PictInfo():num(1),handle(0),interval(6){}
	PictInfo(int inum,int ihandle,int ininterval=6):num(inum),handle(ihandle),interval(ininterval){}
};