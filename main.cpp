#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

int read_int(const char * question)
{
		int result;
	re:	std::string inp;
		std::cout << question;
		std::getline (std::cin, inp);
		{
			std::stringstream inps {inp};
			inps >> result;
			if (inps.fail())
			{
				if (std::cin.fail())
				{
					std::cerr << "Ошибка!" << std::endl;
					exit(1);
				}
				std::cout << "Некорректный ввод!" << std::endl;
				goto re;
			}
			else if(not inps.eof())
			{
				std::cout << "Введите целое число" << std::endl;
				goto re;
			}
		}
		return result;
}

int read_int_in_range(const char * question, int min, int max)
{
	int result;

	for(;;)
	{
		result = read_int(question);
		if (result >= min and result <= max)
			return result;
		std::cout << "Введите число в интервале от " << min << " до " << max << std::endl;
	}
}

int main(){
	using clock = std::chrono::system_clock;
	std::minstd_rand random
	{
		unsigned(clock::now().time_since_epoch().count())
	};
	std::uniform_int_distribution <> d1{15,25};
	std::uniform_int_distribution <> d2{1,3};
	std::uniform_int_distribution <> d3{1,2};
	int level = 0;
menu:
	unsigned stones = d1(random);
	int menu = read_int_in_range("Меню\n1. Начать игру\n2. Уровень сложности\n3. Выход\n", 1, 3);
	switch(menu)
	{
		case 1: if (level == 0) level = 1; break;
		case 2: level = read_int_in_range("Уровень сложности:\n1. Легкий\n2. Средний\n3. Сложный\n", 1, 3); goto menu; break;
		case 3: std::cout << "Игра завершена!" << std::endl; return 0;
	}
	std::cout << "\nКол-во камешков - " << stones << std::endl;
	unsigned rbu =  d3(random);
	if(rbu == 1)
	{
		std::cout << "Первый ход принадлежит пользователю!\n" << std::endl;
	} else std::cout << "Первый ход принадлежит боту!\n" << std::endl;
	do
	{
		unsigned count;
		if (rbu == 1)
		{
			count = read_int_in_range("Введите кол-во камешков: ", 1, std::min(3u, stones));
			stones -= count;
			if (stones == 0)
			{
				std::cout << "\nВы проиграли!\n" << std::endl;
				goto menu;
				break;
			}
			std::cout << "Кол-во камешков - " << stones << std::endl;
			rbu = 2;
		}
		if (rbu == 2){
			//Уровни сложности
			unsigned botStones;
			if(stones < 5 && level != 1)
			{
				switch(stones)
				{
					case 4: botStones = 3; break;
					case 3: botStones = 2; break;
					case 2: botStones = 1; break;
				}
			}
			else
			{
				if (level == 1) botStones = d2(random);
				else if (level == 2)
				{
					switch(count){
						case 1: botStones = 3; break;
						case 2: botStones = 2; break;
						case 3: botStones = 1; break;
						default: botStones = d2(random); break;
					}
				}
				else
				{
					for(unsigned i = 25; i > 0; i -= 4){
						if (i == stones)
						{
							botStones = d2(random);
							break;
						}
						if (stones > i)
						{
							botStones = stones - i;
							break;
						}
					}
				}
			}
			stones -= botStones;
			std::cout << "Бот взял " << botStones << " камешка" << std::endl;
			if (stones == 0 || stones > 25)
			{
				std::cout << "\nВы выиграли!\n" << std::endl;
				goto menu;
				break;
			}
			std::cout << "Кол-во камешков - " << stones << std::endl;
			rbu = 1;
		}
	} while (true);

	return 0;
}
