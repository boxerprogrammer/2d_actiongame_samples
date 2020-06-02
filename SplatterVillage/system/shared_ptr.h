#ifndef KFX_SHARED_PTR_H_INCLUDED
#define KFX_SHARED_PTR_H_INCLUDED
#include"count_base.h"

///共有ポインタ
///他の変数に代入されるたびに参照カウンタが増え、
///このクラスが死ぬとともに、実体が解放される
//スマポはマルチスレッド時のことも考慮しなければ非常に危険である…
///が面倒なのと技術力が足りてないのででやってない
template<class T>
class SharedPtr :  protected CountBase
{
	private:
		T* _ptr;
		///CountBaseから呼び出される純粋仮想関数の実装
		///これが呼び出されたらこいつが持っている実体を解放する
		void Dispose()const{
			if(_ptr){
				delete _ptr;
			}
		}
	public:
		SharedPtr(T* ptr=0):CountBase(),_ptr(ptr){
			if(ptr){
				AddRef();
			}
		}
		virtual ~SharedPtr(){
			if(_ptr){
				ReleaseRef();
			}
			_ptr=0;
		}
	
		///コピーコンストラクタ
		SharedPtr(const SharedPtr& sptr) : CountBase(sptr){
			if( sptr.Get()==_ptr ){
				return;
			}
			_ptr=sptr.Get();
			AddRef();
		}


		///代入
		///@note 代入の際に自身が実体を持っている場合は
		///それをいったん解放してあげなくてはならない。
		SharedPtr<T>& operator=(const SharedPtr<T> & sptr){
			if( sptr.Get()==_ptr ){
				return *this;
			}
			if(_ptr){
				ReleaseRef();
			}
			_ptr=sptr._ptr;
			sptr.AddRef();
			return *this;
		}
		bool IsNull()const{
			return _ptr==0;
		}
		bool operator!()const{
			return IsNull();
		}

		///完全リセット
		///実際のポインタセット前に自分自身をリリースしとく
		void Reset(T* ptr){
			if(ptr==_ptr){
				return;
			}
			if(_ptr!=0 ){
				ReleaseRef();
			}
			_ptr=ptr;
			AddRef();
		}

		///ポインタの先を指し示す->演算子のオーバーロード
		///ここでなぜ手持ちのポインタをそのまま返しているのかというと
		///->は後置演算子であるため、返されたポインタ＋右辺が指し示す
		///ポインタを探しにいく。つまりこの場合、shared_ptr->のアドレスに
		///クライアント側で指定している関数オフセットが加算されるために
		///メンバ関数が正しく呼び出され、メンバ変数を正しく参照できる
		T* operator->() const{
			return _ptr;
		}

		///参照を返す
		T& operator*() const{
			return *_ptr;
		}


		///生ポインタを返す
		T* Get()const{
			return _ptr;
		}
		///持っているポインタを交換する
		void swap(SharedPtr<T>& ptr){
			T* tmp=ptr._ptr;
			ptr._ptr=_ptr;
			_ptr=tmp;
		}
};

#endif


