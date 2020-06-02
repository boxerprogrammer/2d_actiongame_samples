#pragma once

///スコープドポインタ
///Scopedに関してはマルチスレはそこまで考慮しなくてもいいだろう
///auto_ptrの所有権移動代入とかは面倒なのでやんない
template <typename T>
class ScopedPtr{
	private:
		T* _ptr;
		ScopedPtr(const ScopedPtr& );
		ScopedPtr& operator=(const ScopedPtr&);
	public:
		ScopedPtr(T* ptr=0):_ptr(ptr){
		}
		~ScopedPtr(){
			delete _ptr;
			_ptr=0;
		}
		///ポインタのリセット
		///@param ptr スコープ管理したいポインタ
		void Reset(T* ptr=0){
			if(_ptr || _ptr!=ptr){
				delete _ptr;
			}
			_ptr=ptr;
		}
		///生ポインタを返す
		///@retval 管理しているポインタ
		///@note 他のライブラリとのやり取り等のために必要
		T* Get() const{
			return _ptr;
		}
		///メソッドを求められたとき
		T* operator->() const{
			return _ptr;
		}
		
		///実体もしくは参照を返す
		T& operator*() const{
			return *_ptr;
		}

		///このオブジェクトの実体はNULL(0)か？
		bool IsNull()const{
			return _ptr==0;
		}

		///ビクーリマークが来たらNULLかどうか調べてるってことだから…
		bool operator!()const{
			return IsNull();
		}
		
		///持っているポインタを交換する
		void swap(ScopedPtr<T>& ptr){
			T* tmp=ptr._ptr;
			ptr._ptr=_ptr;
			_ptr=tmp;
		}
		
};

