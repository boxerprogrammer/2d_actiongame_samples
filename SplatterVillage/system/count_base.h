#ifndef KFX_COUNT_BASE_H_INCLUDED
#define KFX_COUNT_BASE_H_INCLUDED
///参照カウンタに使用するカウントオブジェクト基底
///
///@note これをマルチスレッドにする場合は、インクリメントやデクリメントの部分を
///InterLocked等の関数で行う必要がある。('A`)マンドクセなのでちょっと今は考えない
///ただし、当たり前の話だが、カウント値も共有化しなければそれぞれが勝手に解放しやがる
///static変数を使うわけにもいかないし、さてイカがなものか…、動的にint型のメモリを
///確保し、Disposeのタイミングにて確保したintにも死んでもらうのが妥当だろう。
class CountBase{
	private:
		mutable volatile int* _usecount;//実際に使用されている数
		mutable volatile int _weakcount;//弱参照カウント(使用が０になったとたんに解放したらばダングリング参照の危険性あり)
	public:
		CountBase():_weakcount(0),_usecount(new int(0)){
		}
		CountBase(const CountBase& countbase):_usecount(countbase._usecount){
		}
		virtual void WeakAddRef(){
			++_weakcount;
		}
		///弱参照解放(弱参照カウントなので遠慮なく死んでいい)
		virtual void WeakRelease()const {
			if( --_weakcount==0){
				delete this;//自殺
			}
		}
		///派生クラス側のイベント
		virtual void Dispose()const=0;

		///参照カウント増加
		virtual void AddRef()const{
			++*_usecount;
		}
		///参照カウント減少
		virtual void ReleaseRef()const{
			if( --*_usecount<=0 ){
				delete _usecount;
				Dispose();//派生クラス側のイベント発生
				WeakRelease();
			}
		}
};


#endif
