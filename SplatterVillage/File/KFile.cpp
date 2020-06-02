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
	
	///ファイル内シーク
	///@param offset シークタイプ地点からのオフセット
	virtual int Seek(int offset,SeekType seektype);
	virtual void Close();

	///現在のカーソル位置(先頭からの)を返す
	///@return 現在のカーソル位置(先頭から現在のカーソル位置までのバイト数)
	virtual int Tell();

	///現在のファイルサイズをバイト数単位で返す
	///@return ファイルサイズ
	virtual unsigned int FileSize();

	///データ読み込み
	///@param address 読み込みデータを書き込むためのメモリアドレス
	///@param size 読み込みたいサイズ
	///@retval true 読み込み成功
	///@retval false 読み込み失敗
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
	//Windows依存コード
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







