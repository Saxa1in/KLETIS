#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

int Beg(int& ByteOffset, char*& Signat);
int PathReq(std::string& adr);
int FolderScan(std::string& adr, int& ByteOffset, char*& Signat);
int CheckFile(std::filesystem::directory_entry AdrFile, int& ByteOffset, char*& Signat);
int Atoi(std::string StrNum, int& NumNum);
int BinryToDec(std::string& StrNum, int& NumNum, const size_t Capcty);

int main()
{
	int ChIErr = 0;
	std::string adr;
	int ByteOffset = 0;
	char* Signat;
	ChIErr = Beg(ByteOffset,Signat);
	if (ChIErr > 0)
	{
		std::cerr << "Error 1\n";
		return 1;
	}
	ChIErr = PathReq(adr);
	if (ChIErr > 0)
	{
		std::cerr << "Error 2\n";
		delete[] Signat;
		return 2;
	}
	ChIErr = FolderScan(adr, ByteOffset, Signat);
	if (ChIErr > 0)
	{
		std::cerr << "Error 3\n";
		delete[] Signat;
		return 3;
	}
	delete[] Signat;
	getchar(); //Слышь, удоли !!!
	getchar();
	return 0;
}

int Beg(int& ByteOffset, char*& Signat)
{
	char* ChCPErr;
	int ChIErr = 0;
	size_t bn = 0;
	size_t en = 0;
	std::string SigStr;
	std::ios::sync_with_stdio(false);  //Подумой
	ChCPErr = setlocale(LC_ALL, "");
	if (ChCPErr == NULL)
	{
		std::cerr << "Error 3\n";
		return 3;
	}
	std::ifstream SigFile("siga.txt");
	if (!SigFile.is_open())
	{
		ByteOffset = 0x000FEA0;  
		try
		{
			Signat = new char[7];
		}
		catch (...)
		{
			std::cerr << "Error 4\n";
			return 4;
		}
		for (size_t i = 0; i < 7; ++i) Signat[i] = 0;
		ChCPErr = strcpy(Signat, "KrNULL");
		if (ChCPErr == Signat)
		{
			std::cerr << "Error 5\n";
			delete[] Signat;
			return 5;
		}
	}
	else
	{
		try
		{
			SigFile >> SigStr;
		}
		catch (...)
		{
			std::cerr << "Error 6\n";
			SigFile.close();
			return 6;
		}
		ChIErr = Atoi(SigStr, ByteOffset);
		if (ChIErr > 0)
		{
			std::cerr << "Error 7\n";
			SigFile.close();
			return 7;
		}
		try
		{
			SigFile.close();
		}
		catch (...)
		{
			std::cerr << "Error 8\n";
			return 8;
		}
		try
		{
			SigFile.open("siga.txt", std::ios_base::binary);
		}
		catch (...)
		{
			std::cerr << "Error 9\n";
			return 9;
		}
		bn = SigStr.size() + (size_t)2;
		try
		{
			SigFile.seekg(0, std::ios_base::end);
		}
		catch (...)
		{
			std::cerr << "Error 10\n";
			SigFile.close();
			return 10;
		}
		try
		{
			en = SigFile.tellg();
		}
		catch (...)
		{
			std::cerr << "Error 11\n";
			SigFile.close();
			return 11;
		}
		try
		{
			SigFile.seekg(bn, std::ios_base::beg);
		}
		catch (...)
		{
			std::cerr << "Error 12\n";
			SigFile.close();
			return 12;
		}
		bn = en - bn;
		try
		{
			Signat = new char[bn + 1];
		}
		catch (...)
		{
			std::cerr << "Error 13\n";
			SigFile.close();
			return 13;
		}
		for (size_t i = 0; i < bn + 1; ++i) Signat[i] = 0;
		try
		{
			SigFile.read(Signat, bn);
		}
		catch (...)
		{
			std::cerr << "Error 14\n";
			SigFile.close();
			delete[] Signat;
			return 14;
		}
		try
		{
			SigFile.close();
		}
		catch (...)
		{
			std::cerr << "Error 15\n";
			delete[] Signat;
			return 15;
		}
	}
	return 0;
}

int PathReq(std::string& adr)
{
	bool ChBErr = 0;
	try
	{
		std::cout << "Введите путь к файлу или директории, пожалуйста\n";
	}
	catch (...)
	{
		std::cerr << "Error 2\n";
		return 2;
	}
	do
	{
		std::cin >> adr;
		try
		{
			ChBErr = std::filesystem::exists(adr);
		}
		catch (...)
		{
			std::cerr << "Error 3\n";
			return 3;
		}
		if (!ChBErr)
		{
			try
			{
				std::cout << "Некорректный путь, попробуйте ещё раз!\n";
			}
			catch (...)
			{
				std::cerr << "Error 4\n";
				return 4;
			}
		}
		try
		{
			std::cin.ignore();
		}
		catch (...)
		{
			std::cerr << "Error 5\n";
			return 5;
		}
	} while (!ChBErr);
	return 0;
}

int FolderScan(std::string& adr, int& ByteOffset, char*& Signat)
{
	int ChIErr = 0;
	bool ChBErr = 0;
	size_t ChStErr = 0;
	char dva[2] = { 0 };
	ChStErr = adr.size();
	if (ChStErr <= 0)
	{
		std::cerr << "Error 1\n";
		return 1;
	}
	if (ByteOffset < 0)
	{
		std::cerr << "Error 2\n";
		return 2;
	}
	if (Signat==NULL)
	{
		std::cerr << "Error 3\n";
		return 3;
	}	
	try
	{
		ChBErr = std::filesystem::is_directory(adr);
	}
	catch (...)
	{
		std::cerr << "Error 4\n";
		return 4;
	}
	if (ChBErr)
	{
		try
		{
			for (const auto& EntryFile : std::filesystem::recursive_directory_iterator(adr))
			{
				std::filesystem::directory_entry AdrFile;
				try
				{
					AdrFile = EntryFile;
				}
				catch (...)
				{
					std::cerr << "Error 6\n";
					return 6;
				}
				try
				{
					ChBErr = AdrFile.is_directory();
				}
				catch (...)
				{
					std::cerr << "Error 7\n";
					return 7;
				}
				if (!ChBErr)
				{
					std::ifstream FileChck;
					try
					{
						FileChck.open(AdrFile.path(), std::ios_base::binary);
					}
					catch (...)
					{
						std::cerr << "Error 8\n";
						return 8;
					}
					try
					{
						FileChck.read(dva, 2);
					}
					catch (...)
					{
						std::cerr << "Error 9\n";
						FileChck.close();
						return 9;
					}
					if ((dva[0] == 0x0000004D) && (dva[1] == 0x0000005A))
					{
						ChIErr = CheckFile(AdrFile, ByteOffset, Signat);
						if (ChIErr > 0)
						{
							std::cerr << "Error 10\n";
							FileChck.close();
							return 10;
						}
					}
					try
					{
						FileChck.close();
					}
					catch (...)
					{
						std::cerr << "Error 11\n";
						return 11;
					}
				}
			}
		}
		catch (...)
		{
			std::cerr << "Error 5\n";
			return 5;
		}
	}
	else
	{
		std::ifstream FileChck;
		try
		{
			FileChck.open(adr, std::ios_base::binary);
		}
		catch (...)
		{
			std::cerr << "Error 12\n";
			return 12;
		}
		try
		{
			FileChck.read(dva, 2);
		}
		catch (...)
		{
			std::cerr << "Error 13\n";
			FileChck.close();
			return 13;
		}
		if ((dva[0] == 77) && (dva[1] == 90))
		{
			ChIErr = CheckFile(std::filesystem::directory_entry(adr), ByteOffset, Signat);
			if (ChIErr > 0)
			{
				std::cerr << "Error 14\n";
				FileChck.close();
				return 14;
			}
		}
		else
		{
			try
			{
				std::cout << "Это не формат Portable Executable\n";
			}
			catch (...)
			{
				std::cerr << "Error 15\n";
				FileChck.close();
				return 15;
			}
		}
		try
		{
			FileChck.close();
		}
		catch (...)
		{
			std::cerr << "Error 16\n";
			return 16;
		}
	}
	return 0;
}

int CheckFile(std::filesystem::directory_entry AdrFile, int& ByteOffset, char*& Signat)
{
	int ChIErr = 0;
	bool ChBErr = 0;
	size_t ChStErr = 0;
	bool itog = 0;
	char* FileStr;
	try
	{
		ChBErr = AdrFile.exists();
		if (!ChBErr) throw 1;
	}
	catch (...)
	{
		std::cerr << "Error 1\n";
		return 1;
	}
	if (ByteOffset < 0)
	{
		std::cerr << "Error 2\n";
		return 2;
	}
	if (Signat==NULL)
	{
		std::cerr << "Error 3\n";
		return 3;
	}		
	size_t SigLen = strlen(Signat);
	try
	{
		FileStr = new char[SigLen + 1];
	}
	catch (...)
	{
		std::cerr << "Error 4\n";
		return 4;
	}
	for (size_t i = 0; i < SigLen + 1; ++i) FileStr[i] = 0;
	std::ifstream CurrFile; 
	try
	{
		CurrFile.open(AdrFile.path(), std::ios_base::binary);
	}
	catch (...)
	{
		std::cerr << "Error 5\n";
		delete[] FileStr;
		return 5;
	}
	try
	{
		CurrFile.seekg(ByteOffset, std::ios_base::beg);
	}
	catch (...)
	{
		std::cerr << "Error 6\n";
		CurrFile.close();
		delete[] FileStr;
		return 6;
	}
	try
	{
		CurrFile.read(FileStr, SigLen);
	}
	catch (...)
	{
		std::cerr << "Error 7\n";
		CurrFile.close();
		delete[] FileStr;
		return 7;
	}
	try
	{
		CurrFile.close();
	}
	catch (...)
	{
		std::cerr << "Error 8\n";
		delete[] FileStr;
		return 8;
	}
	if (strcmp(Signat, FileStr) == 0)
	{
		try
		{
			std::cout << "СИГНАТУРА ОБНАРУЖЕНА В " << AdrFile.path() << '\n';
		}
		catch (...)
		{
			std::cerr << "Error 9\n";
			delete[] FileStr;
			return 9;
		}
	}
	delete[] FileStr;
	return 0;
}

int Atoi(std::string StrNum, int& NumNum)
{
	size_t Capcty = StrNum.size();
	int ChIErr = 0;
	char ChCErr = 0;
	double ChDErr = 0.0;
	int Sign = 1;
	if ((Capcty <= 0) || ((sizeof(int) * 8 + 2) < Capcty))
	{
		std::cerr << "Error 1\n";
		return 1;
	}
	NumNum = 0;
	if (Capcty > 1)
	{
		try
		{
			ChCErr = StrNum.at(0);
		}
		catch (...)
		{
			std::cerr << "Error 3\n";
			return 3;
		}
		if ((ChCErr == '-') || (ChCErr == '+'))
		{
			if (ChCErr == '-') Sign = -1;
			try
			{
				StrNum.erase(0, 1);
			}
			catch (...)
			{
				std::cerr << "Error 4\n";
				return 4;
			}
			Capcty = Capcty - 1;
		}
	}
	if (Capcty > 2)
	{
		try
		{
			ChCErr = StrNum.at(0);
		}
		catch (...)
		{
			std::cerr << "Error 5\n";
			return 5;
		}
		if (ChCErr == '0')
		{
			ChIErr = BinryToDec(StrNum, NumNum, Capcty);
			if (ChIErr > 0)
			{
				std::cerr << "Error 6\n";
				return 6;
			}
		}
		else if (!((ChCErr >= '1') && (ChCErr <= '9')))
		{
			std::cerr << "Error 1\n";
			return 1;
		}
	}
	else if (Capcty >= 1)
	{
		for (size_t i = Capcty; i > 0; --i)
		{
			try
			{
				ChCErr = StrNum.at(i - 1);
			}
			catch (...)
			{
				std::cerr << "Error 7\n";
				return 7;
			}
			if (((ChCErr >= '0') && (ChCErr <= '9')))
			{
				ChDErr = pow(10, (Capcty - i));
				if ((ChDErr == HUGE_VAL) || (ChDErr == -HUGE_VAL))
				{
					std::cerr << "Error 8\n";
					return 8;
				}
				NumNum = NumNum + (ChCErr - 48) * ChDErr;
			}
			else
			{
				std::cerr << "Error 1\n";
				return 1;
			}
		}
	}
	NumNum = Sign * NumNum;
	return 0;
}

int BinryToDec(std::string& StrNum, int& NumNum, const size_t Capcty)
{
	char ChCErr = 0;
	double ChDErr = 0;
	if (StrNum.size() <= 1)
	{
		std::cerr << "Error 1\n";
		return 1;
	}
	if (Capcty < 2)
	{
		std::cerr << "Error 3\n";
		return 3;
	}
	try
	{
		ChCErr = StrNum.at(0);
	}
	catch (...)
	{
		std::cerr << "Error 4\n";
		return 4;
	}
	if (ChCErr == '0')
	{
		try
		{
			ChCErr = StrNum.at(1);
		}
		catch (...)
		{
			std::cerr << "Error 5\n";
			return 5;
		}
		if (ChCErr == 'x')
		{
			if (sizeof(int) * 2 + 2 < Capcty)
			{
				std::cerr << "Error 1\n";
				return 1;
			}
			for (size_t i = Capcty; i > 2; --i)
			{
				try
				{
					ChCErr = StrNum.at(i - 1);
				}
				catch (...)
				{
					std::cerr << "Error 6\n";
					return 6;
				}
				if (((ChCErr >= '0') && (ChCErr <= '9')) || ((ChCErr >= 'A') && (ChCErr <= 'F')) || ((ChCErr >= 'a') && (ChCErr <= 'f')))
				{
					ChDErr = pow(16, (Capcty - i));
					if ((ChDErr == HUGE_VAL) || (ChDErr == -HUGE_VAL))
					{
						std::cerr << "Error 7\n";
						return 7;
					}
					if ((ChCErr >= '0') && (ChCErr <= '9')) NumNum = NumNum + (ChCErr - 48) * ChDErr;
					else if ((ChCErr >= 'A') && (ChCErr <= 'F')) NumNum = NumNum + (ChCErr - 55) * ChDErr;
					else if ((ChCErr >= 'a') && (ChCErr <= 'f')) NumNum = NumNum + (ChCErr - 87) * ChDErr;
				}
				else
				{
					std::cerr << "Error 1\n";
					return 1;
				}
			}
		}
		else if (ChCErr == 'b')
		{
			for (size_t i = Capcty; i > 2; --i)
			{
				try
				{
					ChCErr = StrNum.at(i - 1);
				}
				catch (...)
				{
					std::cerr << "Error 8\n";
					return 8;
				}
				if ((ChCErr >= '0') && (ChCErr <= '1'))
				{
					ChDErr = pow(2, (Capcty - i));
					if ((ChDErr == HUGE_VAL) || (ChDErr == -HUGE_VAL))
					{
						std::cerr << "Error 9\n";
						return 9;
					}
					NumNum = NumNum + (ChCErr - 48) * ChDErr;
				}
				else
				{
					std::cerr << "Error 1\n";
					return 1;
				}
			}
		}
		else
		{
			if (sizeof(int) * 3 < Capcty)
			{
				std::cerr << "Error 1\n";
				return 1;
			}
			for (size_t i = Capcty; i > 1; --i)
			{
				try
				{
					ChCErr = StrNum.at(i - 1);
				}
				catch (...)
				{
					std::cerr << "Error 10\n";
					return 10;
				}
				if ((ChCErr >= '0') && (ChCErr <= '7'))
				{
					ChDErr = pow(8, (Capcty - i));
					if ((ChDErr == HUGE_VAL) || (ChDErr == -HUGE_VAL))
					{
						std::cerr << "Error 11\n";
						return 11;
					}
					NumNum = NumNum + (ChCErr - 48) * ChDErr;
				}
				else
				{
					std::cerr << "Error 1\n";
					return 1;
				}
			}
		}
	}
	return 0;
}