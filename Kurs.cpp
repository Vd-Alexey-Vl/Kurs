#include <iostream>   
#include <fstream>    
#include <windows.h>
#include <cstdlib>  

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // открытие файла + проверка корректности открытия
    std::ifstream rdrw("in.txt");
    if (!rdrw.is_open())
    {
        std::cout << "Ошибка открытия файла!" << std::endl;
        return EXIT_FAILURE;
    }

    // создание переменных и считывание в них значений из файла
    int rows;
    int cols;
    rdrw >> rows >> cols;

    // создание динамических массивов
    int** current = new int* [rows];
    int** next = new int* [rows];
    for (int i = 0; i < rows; ++i)
    {
        current[i] = new int[cols](); // инициализация нулями
        next[i] = new int[cols]();
    }

    // считывание живых клеток из файла
    int x, y;
    while (rdrw >> x >> y)
    {
        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            current[x][y] = 1;
        }
    }
    rdrw.close();

    // количество поколений жизни
    int generation = 0;
    while (true)
    {
        // очистка экрана
        system("cls");

        // подсчёт живых клеток
        int alive = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (current[i][j] == 1)
                    alive++;
            }
        }

        // вывод массива в консоль
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (current[i][j] == 1)
                    std::cout << "*";
                else
                    std::cout << "-";
                if (j < cols - 1)
                    std::cout << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << "Поколение: " << generation << ", Живых клеток: " << alive << "\n\n";

        // если все клетки мертвы, конец
        if (alive == 0)
        {
            std::cout << "Все клетки мертвы. Конец симуляции." << std::endl;
            break;
        }
 
        // следующее поколение
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < cols; j++) 
            {
                // подсчет живых соседей для клетки
                int neighbors = 0;
                for (int ni = -1; ni <= 1; ni++) // смещение по строкам 
                {      
                    for (int nj = -1; nj <= 1; nj++) // смещение по столбцам
                    {  
                        if (ni == 0 && nj == 0) // пропускаем саму клетку
                            continue; 
                        int ci = i + ni;
                        int cj = j + nj;
                        if (ci >= 0 && ci < rows && cj >= 0 && cj < cols)// сосед в пределах поля или нет
                        {
                            if (current[ci][cj] == 1)
                                neighbors++;
                        }
                    }
                }

                // правила игры «Жизнь»
                if (current[i][j] == 1)      // клетка жива
                {   
                    if (neighbors == 2 || neighbors == 3)
                        next[i][j] = 1;      // остаётся живой
                    else
                        next[i][j] = 0;      // умирает
                }
                else                         // клетка мертва
                {                      
                    if (neighbors == 3)
                        next[i][j] = 1;      // рождается
                    else
                        next[i][j] = 0;      // остаётся мёртвой
                }
            }
        }
        //сравнение текущего и следующего поколений
        bool stable = true;
        for (int i = 0; i < rows && stable; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (current[i][j] != next[i][j])
                {
                    stable = false;
                    break;
                }
            }
        }

        if (stable)
        {
            std::cout << "Достигнута стабильная конфигурация. Конец симуляции." << std::endl;
            break;
        }

        std::swap(current, next);

        generation++;
        Sleep(1000);
    }

    // освобождение памяти
    for (int i = 0; i < rows; ++i)
    {
        delete[] current[i];
        delete[] next[i];
    }
    delete[] current;
    delete[] next;

    return EXIT_SUCCESS;
}