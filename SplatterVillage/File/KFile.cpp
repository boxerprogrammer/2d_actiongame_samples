#include "KFile.h"
#include <Windows.h>
#include <cassert>
class WinFile : public KFile
{
private:
	HANDLE _handle;
public:
	WinFile(HANDLE FileHandle);
	virtual ~WinFile(void);
	
	///�t�@�C�����V�[�N
	///@param offset �V�[�N�^�C�v�n�_����̃I�t�Z�b�g
	virtual int Seek(int offset,SeekType seektype);
	virtual void Close();

	///���݂̃J�[�\���ʒu(�擪�����)��Ԃ�
	///@return ���݂̃J�[�\���ʒu(�擪���猻�݂̃J�[�\���ʒu�܂ł̃o�C�g��)
	virtual int Tell();

	///���݂̃t�@�C���T�C�Y���o�C�g���P�ʂŕԂ�
	///@return �t�@�C���T�C�Y
	virtual unsigned int FileSize();

	///�f�[�^�ǂݍ���
	///@param address �ǂݍ��݃f�[�^���������ނ��߂̃������A�h���X
	///@param size �ǂݍ��݂����T�C�Y
	///@retval true �ǂݍ��ݐ���
	///@retval false �ǂݍ��ݎ��s
	virtual bool Read(void* address,unsigned int size);

};

KFile::KFile()
{
}


KFile::~KFile(void)
{

}

WinFile::WinFile(HANDLE handle):_handle(handle){
}

WinFile::~WinFile(){
	if(_handle!=INVALID_HANDLE_VALUE){
		Close();
	}
}

bool
WinFile::Read(void* address,unsigned int size){
	DWORD numread;
	BOOL result = ::ReadFile(_handle,
		address,
		size,
		&numread,
		NULL);
	assert(size==numread);
	return result;
}

unsigned int 
WinFile::FileSize(){
	LARGE_INTEGER filesize;
	::GetFileSizeEx(_handle,&filesize);
	return filesize.QuadPart;
}

void
WinFile::Close(){
	::CloseHandle(_handle);
}

int 
WinFile::Seek(int offset, SeekType type){
	DWORD moveMethod = type==sk_type_current ?  FILE_CURRENT : 
		type==sk_type_begin ? FILE_BEGIN : FILE_END;
	LARGE_INTEGER loffset,resultoffset;
	loffset.QuadPart=offset;
	::SetFilePointerEx(_handle,loffset,&resultoffset,moveMethod);
	return resultoffset.QuadPart;
}

int 
WinFile::Tell(){
	LARGE_INTEGER loffset,resultoffset;
	loffset.QuadPart=0;
	::SetFilePointerEx(_handle,loffset,&resultoffset,FILE_CURRENT);
	return resultoffset.QuadPart;
}


KFile*
FileOpener::Open(const char* filename,FileOpenMode fom){
	DWORD desiredAccess = fom==fom_read ? GENERIC_READ : GENERIC_WRITE;
	DWORD creationDisposition = fom==fom_read ? OPEN_EXISTING : OPEN_ALWAYS;
	//Windows�ˑ��R�[�h
	HANDLE handle = ::CreateFile(filename,
		desiredAccess,
		FILE_SHARE_READ,
		NULL,
		creationDisposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(handle==INVALID_HANDLE_VALUE){
		return NULL;
	}
	return new WinFile(handle);
}







