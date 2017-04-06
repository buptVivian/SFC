#include "tchar.h"
#include "simulation.h"
int _tmain(int argc, _TCHAR* argv[])
{
	 //写文件
	ofstream ofile;
	ofile.open("E:\Vivian//res.txt", ios::out);
	if (!ofile)
	{
		cerr << "open result.data file error!" << endl;
		exit(-2);
	}
	
	//不断增大业务强度(提高到达率,离去率保持不变)
	for (double lamda = 1; lamda < 10; lamda += 0.5)
	{
		clock_t start = clock();
		
		readUSAEdgeMap( );   //目前不存在错误5/14
		initialize(0.1 / lamda);  
		run(ofile);

		clock_t end = clock();
		double result = (double)(end - start) / CLOCKS_PER_SEC;
		cout << result << "s" << endl;
	}

	

	ofile.close();
	system("pause");
	return 0;
	
}
