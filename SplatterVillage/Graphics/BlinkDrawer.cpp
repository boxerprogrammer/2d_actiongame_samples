#include "BlinkDrawer.h"


BlinkDrawer::BlinkDrawer(void){
}

BlinkDrawer::BlinkDrawer(const char* filename,int cycle) : BaseDrawer(filename),_cycle(cycle),_blinkframe(cycle/2){
}

BlinkDrawer::~BlinkDrawer(void){
}

void 
BlinkDrawer::Draw(){
	if((++_blinkframe%_cycle)-(_cycle/2)>0){
		BaseDrawer::Draw();
	}
}

void
BlinkDrawer::BlinkTime(unsigned int cycle){
	_cycle=cycle;
}