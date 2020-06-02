#ifndef KFX_REFCOUNTABLE_H_INCLUDED
#define KFX_REFCOUNTABLE_H_INCLUDED
#include"count_base.h"
namespace KFX{
///参照カウンタ基底クラス
///非マルチスレッド用
///他の変数に代入されるたびに参照カウンタが増え、
///このクラスが死ぬとともに、実体が解放される
template<class T>
class RefCountablePtr<T> : protected CountBase{
	private:
		T* _ptr;
	public:
		RefCountable():CountBase(),_ptr(0),_count(0){
			_ptr = new T();
			AddRef();
		}
		RefCountable(T* ptr):_ptr(ptr),_count(1){
		}
		
		void Reset(T* ptr){
			if( _ptr ){
				ReleaseRef();
			}
			AddRef();
		}

		///生ポインタを返す
		///@retval 生ポインタ
		T* Get() const{
			return _ptr;
		}

		void Dispose(){
			delete _ptr;
		}
		
};

}// end of namespace KFX

#endif
