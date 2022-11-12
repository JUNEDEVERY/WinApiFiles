#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#define INPUT L"INPUT.txt" // файл ввода
#define OUTPUT L"Output.txt" // файл вывода
#define BUFF 512 //объем буфера



int WINAPI WinMain(HINSTANCE hlnstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	LPSTR  a1 = "", b1 = "", c1 = ""; // инициализируем из файла
	OVERLAPPED olf = { 0 }; // cтруктура, в которой задана позиция в файле, ради ассинхронности 
	TCHAR otvFile[BUFF];
	DWORD razmer, d = 0, d1 = 0, len = 0; // счетчики и длина слова (сколько фактически байт было прочитано)
	BOOL FlagS = FALSE;
	HANDLE inFile = CreateFile(INPUT, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE outFile = CreateFile(OUTPUT, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LPSTR str = malloc(BUFF + 1); //куда считывать
	BOOL F = ReadFile(inFile, str, BUFF, &d, &olf); // считываем файл и если он есть записываем true
	if (F == FALSE) // если нету
	{
		swprintf(otvFile, BUFF, TEXT("Файл не найден или пуст."));
	}
	else {
		free(str);
		olf.Offset = 0; // задаем смещение (позицию в файле)
		LPSTR str1 = malloc(d + 1); // строка
		// в infile передали handle, str1 - строка, d, d1 - количество байтов которое было прочитано, olf - offset изначально он ноль
		BOOL F = ReadFile(inFile, str1, d, &d1, &olf); // считали файл по размеру строки, чтобы не считывать лишние буферы 
		if (F == FALSE)
		{
			swprintf(otvFile, BUFF, TEXT("Файл не найден или пуст."));
		}
		else // начинаем считывать файл и проверять на корректность
		{
			str1[d1] = '\0'; //  ставим символ окончания строки, чтобы не использовать буфер
			CloseHandle(inFile); // закрываем стрим к инфайлу
			while (str1[len] != '\0') // цикл до конца файла с счетиком лен
			{
				// проверяем, что в файле были только цифры, не были - . и т.д, т.к. они не влезут в atof
				// проверка на количество байтов. условно говоря 47 байтов в юникоде - это "/", 48 - 0, а 49 - 1. 57 - это 9.,а 58 - ":" (проверка на корректность)
				if ((str1[len] < 47 || str1[len] > 58) && (str1[len] != '-' && str1[len] != '.' && str1[len] != ' ' && str1[len] != '\n' && str1[len] != '\0'))
					FlagS = TRUE;
				len++;
				if (FlagS == TRUE)
				{
					break;
				}
			}
			if (FlagS == TRUE) 

			{
				swprintf(otvFile, BUFF, TEXT("Файл ввода не соответствует шаблону \"3 числа через пробелы\""));
			}
			else
			{
				for (int k = 0; k <= 3; k++) // цикл до 3 элементов т.к a b c 
				{
					LPSTR a = malloc(d1);
					for (int i = 0; i < d1; (str1)++) // прогоняю строку посимвольно до пробела
					{
						CHAR bufferstr = *str1;
						if (bufferstr == ' ' || bufferstr == '\n') // ищем не пробел и не конец строки. то есть мы вводим 1 2 3, он начинает с 1 проверяет, есть ли пробел и конец ли строки, записывает и так до конца строки
						{
							a[i] = '\0'; // записываем буферный символ который к нам пришел
							(str1)++;
							break;
						}
						a[i] = bufferstr; 
						i++;
					}
					if (k == 0) a1 = a; // записываем цифры полученные в переменные для решения
					if (k == 1) b1 = a;
					if (k == 2) c1 = a;
				}
				char* stroka1 = a1, * stroka2 = b1, * stroka3 = c1; // делем переменные, результаты которых получены выше для решения уравнения
				double a2 = atof(stroka1), b2 = atof(stroka2), c2 = atof(stroka3); // конвертируем в атоф в дабл
				double  D, x1 = 0, x2 = 0; // определения корни и дискриминант

				//  блок решения
				if (a2 == 0) {
					swprintf(otvFile, BUFF, TEXT("Не квадратное уравнение\0"));
				}

				else {
					D = pow(b2, 2) - (4 * a2 * c2);
					if (D < 0)
					{
						swprintf(otvFile, BUFF, TEXT("Уравнение не имеет вещественных корней!\0")); // записали в буфер ответы
					}

					else if (D == 0) {
						x1 = -b2 / (2 * a2);
						if (b2 == 0 && c2 == 0) x1 = 0;
						{

							swprintf(otvFile, BUFF, TEXT("Уравнение имеет один корень:\nx = %.10f\nd= %.10f\0"), x1, D, '\0'); // записали в буфер ответы
						}

					}
					else {
						x1 = (-b2 - sqrt(D)) / (2 * a2);
						x2 = (-b2 + sqrt(D)) / (2 * a2);
						swprintf(otvFile, BUFF, TEXT("Уравнение имеет два корня:\nx1 = %.10f\nx2 = %.10f\nd= %.10f\0"), x1, x2, D); // записали в буфер ответы
					}
				}

			}
		}
	}
	len = 0;
	while (otvFile[len] != '\0') // в конец файла вставляем символ конца
	{
		len++;
	}
	DWORD dz = 0;
	WriteFile(outFile, // дальше записываем результат
		otvFile,
		len * (sizeof(char) * 2), // чаровский символ весит 2 байта, а не один
		&dz,
		NULL // ассинхронность не нужна в данном случае, поэтому убрали
	);
	CloseHandle(outFile); // закрываем стрим к файлу
}