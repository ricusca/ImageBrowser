
#include "json/json.h"
#include <string>
#include <iostream>
#include "HttpClient.h"
#include "DownloadFile.h"

using std::string;

using std::cout;
using std::endl;

int main(int argc, const char * argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		printf("arg%d: %s\n", i, argv[i]);
	}

	HttpClient::GetInstance()->Initialize();

	std::vector<DownloadFile> files(1);
	files[0].Download(argv[1]);


	do 
	{
		Sleep(400);
	} while (true);

	return 1;
}