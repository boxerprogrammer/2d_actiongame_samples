#include"GameMain.h"
#include<iostream>

using namespace std;

#ifdef DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE,HINSTANCE , LPSTR , int){
#endif
	cout << sizeof(unsigned short) << endl;
	GameMain::Instance().Run();
}

