#pragma once

///シーク種別
enum SeekType{
	sk_type_begin,//先頭からシーク
	sk_type_current,//現在カーソル位置からシーク
	sk_type_end//末尾からシーク
};

///ファイル関係のものをラップ
///@note 頭にKがついているのはFileだと何かと競合しそうだから
class KFile
{
public:
	KFile();
	virtual ~KFile(void);
	
	///ファイル内シーク
	///@param offset シークタイプ地点からのオフセット
	virtual int Seek(int offset,SeekType seektype)=0;
	virtual void Close()=0;

	///現在のカーソル位置(先頭からの)を返す
	///@return 現在のカーソル位置(先頭から現在のカーソル位置までのバイト数)
	virtual int Tell()=0;

	///現在のファイルサイズをバイト数単位で返す
	///@return ファイルサイズ
	virtual unsigned int FileSize()=0;

	///データ読み込み
	///@param address 読み込みデータを書き込むためのメモリアドレス
	///@retval true 読み込み成功
	///@retval false 読み込み失敗
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

