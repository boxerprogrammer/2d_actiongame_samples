#include "GraphicsUpdater.h"
#include <algorithm>

GraphicsUpdater::GraphicsUpdater(void)
{
}


GraphicsUpdater::~GraphicsUpdater(void)
{
}


void 
GraphicsUpdater::Update(){
	class BeforeFunc{
		public:
			void operator()(Drawer* drawer){
				drawer->BeforeDraw();
			}
	};
	class DrawFunc{
		public:
			void operator()(Drawer* drawer){
				drawer->Draw();
			}
	};
	class AfterFunc{
		public:
			void operator()(Drawer* drawer){
				drawer->AfterDraw();
			}
	};

	std::for_each(_drawers.begin(),_drawers.end(),BeforeFunc());
	std::for_each(_drawers.begin(),_drawers.end(),DrawFunc());
	std::for_each(_drawers.begin(),_drawers.end(),AfterFunc());
}