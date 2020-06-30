#include<iostream>
#include<thread>

using namespace std;

void f1(int x)
{
	for(int i=0;i<=x;++i)
	     cout << i<<'\n';
}
void f2(char x)
{
	for (char ch='a'; ch <= x; ++ch)
		cout << ch<<'\n';
}

int main()
{
	thread t1(&f1, 5);
	thread t2(&f2, 'f');
	t1.join();
	t2.join();
	char ch;
	cout << "\n\nPress any key to countinue . . . ";
	cin >>ch;
	cout << ch;
	return 0;
}
