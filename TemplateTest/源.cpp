#include <iostream>
#include "basewin.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow() : a(-1) {}
private:
	int a;
};


int main(void)
{
	BaseWindow<int> bw;
	MainWindow mw;
	return 0;
}