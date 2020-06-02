#pragma once

///�V�[�N���
enum SeekType{
	sk_type_begin,//�擪����V�[�N
	sk_type_current,//���݃J�[�\���ʒu����V�[�N
	sk_type_end//��������V�[�N
};

///�t�@�C���֌W�̂��̂����b�v
///@note ����K�����Ă���̂�File���Ɖ����Ƌ���������������
class KFile
{
public:
	KFile();
	virtual ~KFile(void);
	
	///�t�@�C�����V�[�N
	///@param offset �V�[�N�^�C�v�n�_����̃I�t�Z�b�g
	virtual int Seek(int offset,SeekType seektype)=0;
	virtual void Close()=0;

	///���݂̃J�[�\���ʒu(�擪�����)��Ԃ�
	///@return ���݂̃J�[�\���ʒu(�擪���猻�݂̃J�[�\���ʒu�܂ł̃o�C�g��)
	virtual int Tell()=0;

	///���݂̃t�@�C���T�C�Y���o�C�g���P�ʂŕԂ�
	///@return �t�@�C���T�C�Y
	virtual unsigned int FileSize()=0;

	///�f�[�^�ǂݍ���
	///@param address �ǂݍ��݃f�[�^���������ނ��߂̃������A�h���X
	///@retval true �ǂݍ��ݐ���
	///@retval false �ǂݍ��ݎ��s
	virtual bool Read(void* address,unsigned int size)=0;

};

enum FileOpenMode{
	fom_read,
	fom_write
};

class FileOpener{
private:
	FileOpener(){};
	FileOpener(const FileOpener&);
	FileOpener& operator=(const FileOpener& );
	
public:
	static FileOpener& Instance(){
		static FileOpener instance;
		return instance;
	}
	KFile* Open(const char* filename,FileOpenMode fom=fom_read);
};

