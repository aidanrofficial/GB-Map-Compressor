#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <windows.h>

using namespace std;

ifstream inputFile;
ofstream outputFile;
string fileName;

int mapSizeX = 0;

string getFileName(string fileName);

string int_to_hex(int i)
{
	stringstream stream;

	if(i < 16)
	{
		stream << "0x0" << std::hex << std::uppercase << i;
	}
	else
	{
		stream << "0x" << std::hex << std::uppercase << i;
	}

	return stream.str();
}

void compress()
{
	string compressOutput = "  ";
	string lookupOutput = "  ";

	string lastOutput;
	string fileOutput;
	int outputCounter = 1;
	int lineCounter = 0;
	int lookupCounter = 0;
	int lookupLineCounter = 0;
	int compressCounter = 1;

	fileOutput.clear();
	inputFile >> fileOutput;
	fileOutput.resize(4);

	lastOutput = fileOutput;

	lookupOutput += to_string(lookupCounter) + ",";
	lookupLineCounter++;

	while(inputFile.peek() != EOF)
	{
		fileOutput.clear();
		inputFile >> fileOutput;
		fileOutput.resize(4);

		if(fileOutput == lastOutput)
		{
			outputCounter++;
			compressCounter++;

			if(compressCounter == mapSizeX)
			{
				compressOutput += lastOutput + ",";
				compressOutput += int_to_hex(outputCounter) + ",";

				outputCounter = 0;
				compressCounter = 0;

				lineCounter++;
				if(lineCounter == 5)
				{
					lineCounter = 0;
					compressOutput += '\n';
					compressOutput += "  ";
				}

				lookupCounter += 2;

				lookupOutput += to_string(lookupCounter) + ",";

				lookupLineCounter++;
				if(lookupLineCounter == 10)
				{
					lookupLineCounter = 0;
					lookupOutput += '\n';
					lookupOutput += "  ";
				}
			}
			else if(outputCounter == 255)
			{
				compressOutput += lastOutput + ",";
				compressOutput += int_to_hex(outputCounter) + ",";

				outputCounter = 1;

				lineCounter++;
				if(lineCounter == 5)
				{
					lineCounter = 0;
					compressOutput += '\n';
					compressOutput += "  ";
				}

				lookupCounter += 2;
			}
		}
		else
		{
			compressOutput += lastOutput + ",";
			compressOutput += int_to_hex(outputCounter) + ",";

			outputCounter = 1;
			lastOutput = fileOutput;

			compressCounter++;

			lineCounter++;
			if(lineCounter == 5)
			{
				lineCounter = 0;
				compressOutput += '\n';
				compressOutput += "  ";
			}

			lookupCounter += 2;
		}
	}

	if(outputCounter > 0)
	{
		compressOutput += lastOutput + ",";
		compressOutput += int_to_hex(outputCounter);
	}

	compressOutput.pop_back();
	
	outputFile << "const unsigned char " << getFileName(fileName) << "[] =" << endl;
	outputFile << "{" << endl;
	outputFile << compressOutput << endl;
	outputFile << "};" << endl << endl;

	lookupOutput.pop_back();
	outputFile << "const UINT16 Lookup_" << getFileName(fileName) << "[] =" << endl;
	outputFile << "{" << endl;
	outputFile << lookupOutput << endl;
	outputFile << "};";

	compressOutput.clear();
	lookupOutput.clear();

	inputFile.close();
	outputFile.close();
}

string getFileName(string fileName)
{
	string tempFileName = fileName;
	return tempFileName.erase(tempFileName.length() - 2);
}

int main()
{
	SetConsoleTitle(L"GB Map Compressor");
	
	HWND console = GetConsoleWindow();
	RECT r;

	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 450, 250, TRUE);

	cout << "Enter file name: ";

	cin >> fileName;

	cout << "Enter map size X: ";
	cin >> mapSizeX;

	inputFile.open(fileName);

	if(inputFile.is_open())
	{
		cout << fileName << " was found." << endl;
		outputFile.open("Compressed_" + getFileName(fileName) + ".c");
	}
	else
	{
		cout << fileName << " was not found." << endl;
		system("pause");
		return 0;
	}

	compress();

	cout << "Outputed to " << "Compressed_" << fileName << endl;

	system("pause");

	return 0;
}