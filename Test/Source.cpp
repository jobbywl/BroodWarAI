#include <Windows.h>
#include <list>
#include <iostream>
#include <string>

std::wstring toWCHAR(std::string dir)
{
	if (dir.empty())
		return NULL;
	int sz = MultiByteToWideChar(GetACP(), 0, &dir[0], (int)dir.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(GetACP(), 0, &dir[0], (int)dir.size(), &res[0], sz);
	return res;
}

std::list<std::string> *getDirContents(std::string dir)
{
	WIN32_FIND_DATA found;

	HANDLE hFind = FindFirstFile(toWCHAR(dir+"*").c_str(), &found);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return  new std::list<std::string>;
	}
	// List all the files in the directory with some info about them.
	std::list<std::string> *fileList;
	fileList = new std::list<std::string>;
	do
	{
		if (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			
			std::list<std::string> *temp;
			std::wstring tempWstr;
			tempWstr = found.cFileName;
			std::string tempstr(tempWstr.begin(), tempWstr.end());
			
			if ((tempstr.find("..") == std::string::npos) && (tempWstr.find(L".") == std::string::npos))
			{
				temp = getDirContents(dir + tempstr+ "\\");
				fileList->splice(fileList->end(), *temp);
				delete temp;
			}
		}
		else
		{
			std::wstring temp(found.cFileName);
			std::string tempstr(temp.begin(), temp.end());
			tempstr.insert(0, dir);
			fileList->push_back(tempstr);
		}
	} while (FindNextFile(hFind, &found) != 0);
	return fileList;
}

int main()
{
	std::list<std::string> *temp;
	temp = getDirContents("C:\\StarCraft\\Maps\\Replays\\");

	for (auto i = temp->begin(); i != temp->end(); i++)
	{
		std::cout << (*i) << std::endl;
	}

	return 0;
}