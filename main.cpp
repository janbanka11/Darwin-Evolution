#include "libs.h"

/**
 * Struktura przyjmujaca czesc poczatkowa i czesc koncowa osobnika
 */
struct Individual
{
	std::vector<int> initial_part;
	std::vector<int> final_part;
};


/**
 * \Funkcja zamieniajaca string w postaci {liczba1 liczba2 liczba3 liczba4} na vector<int> w postaci {liczba1, liczba2, liczba3, liczba4}
 * @param string - argument funkcji wykorzystywany do parsowania
 * @return vector<int> powstaly z parsowania stringa
 */
std::vector<int> string_to_vec_ints(std::string& string)
{
	std::stringstream iss(string);
	int number;
	std::vector<int> numbers;

	while (iss >> number)
	{
		numbers.push_back(number);
	}

	return numbers;
}

/**
 * \konwersja vectora stringow na vector<vector<int>> z uzyciem funkcji string_to_vec_ints
 * @param strings - vector stringow do przeksztalcania w vector<intow>
 * @return vector<vector<int>> powstaly z zamiany stringa na vector<int>
 */
std::vector<std::vector<int>> str_vec_to_double_vec_ints(std::vector<std::string> strings)
{
	std::vector<std::vector<int>> results;

	for (int i = 0; i < strings.size(); ++i)
	{
		results.push_back(string_to_vec_ints(strings[i]));
	}
	return results;
}

/**
 * \ Pobieranie danych z pliku
 * @param filename - nazwa pliku wejsciowego
 * @return zwraca zawartosc pliku tekstowego w formie vector<vector<int>>
 */
std::vector<std::vector<int>> get_data_from_txt(const std::string& filename)
{
	std::ifstream data;
	data.open(filename);
	std::vector<std::string> temporary;
	std::string n;

	if (data.is_open())
	{
		while (!data.eof())
		{
			std::getline(data, n);
			temporary.push_back(n);
		}
		data.close();
	}
	else
	{
		std::cout << "Nie udalo sie otworzyc pliku " << filename << " do odczytu lub nazwa jest nieprawidlowa!" << std::endl;
	}

	return str_vec_to_double_vec_ints(temporary);
}

/**
 * \Dzielenie osobnika na czesc poczatkowa i koncowa
 * @param v - osobnik do podzielenia
 * @return zwraca podzielonego osobnika w formie struktury
 */
Individual dividing_individual(const std::vector<int>& v)
{
	    Individual results;

		const int rand_crack_idx = (rand() % v.size() - 2) + 1; // losowanie miejsca pekniecia
		for (int i = 0; i < rand_crack_idx; i++)
		{
			results.initial_part.push_back(v[i]);
		}

		for (int i = rand_crack_idx; i < v.size(); ++i)
		{
			results.final_part.push_back(v[i]);
		}

	return results;
}

/**
 * \Krzyzowanie dwoch osobnikow
 * @param ind1 - jeden z osobnikow wylosowanych do krzyzowania
 * @param ind2 - drugi z osobnikow wylosowanych do krzyzowania
 */
void crossing(Individual& ind1, Individual& ind2) // laczenie osobnikow
{
	ind1.final_part = ind2.initial_part;
	ind2.final_part = ind1.initial_part;
}

/**
 * \tworzenie nowego vectora przy uzyciu juz polaczonych osobnikow
 * @param ind
 * @return
 */
std::vector<int> new_vec(Individual& ind)
{
	std::vector<int> v;
	v.insert(v.end(), ind.initial_part.begin(), ind.initial_part.end());
	v.insert(v.end(), ind.final_part.begin(), ind.final_part.end());
	return v;
}

/**
 * \Losowanie i krzyzowanie osobnikow
 * @param v - wczytane z pliku osobniki
 * @param k - liczba par osobnikow do krzyzowania
 */
void crossing_of_individuals(std::vector<std::vector<int>>& v, const int k)
{
	const int amount_to_cross = k * 2;

		for (int i = 0; i < amount_to_cross; i += 2)
		{
			int rand1 = 0;
			int rand2 = 0;
			do
			{
				rand1 = rand() % v.size();
				rand2 = rand() % v.size(); // losowanie indeksow osobnikow do krzyzowania
			} while (rand1 == rand2 || v[rand1].empty() || v[rand2].empty());

			Individual ind1 = dividing_individual(v[rand1]);
			Individual ind2 = dividing_individual(v[rand2]); // dzielenie osobnikow na czesc poczatkowa i koncowa
			crossing(ind1, ind2);

			std::vector<int> temp1 = new_vec(ind1);
			std::vector<int> temp2 = new_vec(ind2);

			v[rand1] = temp1; // zamiana wczesniej wylosowanego osobnika na skrzyzowanego
			v[rand2] = temp2;
		}
}


/**
 * \Sprawdzanie adaptacji do zycia osobnikow
 * @param v - skrzyzowane osobniki pobrane z pliku wejsciowego
 * @param w - wspolczynnik wymierania
 * @param r - wspolczynnik rozmnazania
 */
void adaptation(std::vector<std::vector<int>>& v, double w, double r)
{
		for (int i = 0; i < v.size(); ++i)
		{
			double f = rand() % 10; // wyznaczanie wartosci przystosowania do warunkow srodowiska dla osobnika
			f /= 10;
			if (f < w)
			{
				v[i].erase(v[i].begin(), v[i].end()); // usuwanie osobnika
			}
			else if (f > r)
			{
				std::vector<int> temp = v[i]; // klonowanie osobnika
				v.emplace_back(temp);
			}
		}
}

/**
 * \Zapis do pliku i wywolanie wszystkich innych funkcji
 * @param filename_in - plik wejsciowy do odczytu
 * @param filename_out - plik wyjsciowy do zapisu
 * @param w - wspolczynnik wymierania
 * @param r - wspolczynnik rozmnazania
 * @param p - liczba pokolen
 * @param k - liczba par osobnikow do krzyzowania
 */
void save(const std::string& filename_in, const std::string& filename_out, double w, double r, const int p, const int k) // zapis do pliku
{
	std::ofstream save;
	save.open(filename_out);
	std::vector<std::vector<int>> v = get_data_from_txt(filename_in);

	if (save.is_open())
	{
		for (int i = 0; i < p; ++i)
		{
			crossing_of_individuals(v, k);
			adaptation(v, w, r);
		}

		for (int i = 0; i < v.size(); ++i)
		{
			if (!v[i].empty())
			{
				for (int j = 0; j < v[i].size(); ++j)
				{
					save << v[i][j] << " ";
				}
				save << std::endl;
			}
		}
		save.close();
	}
	else
	{
		std::cout << "Nie udalo sie otworzyc pliku " << filename_out << " do zapisu!" << std::endl;
	}
}

/**
 * \param argc - liczba argumentow podanych przez uzytkownika
 * \param argv - argumenty podane przez uzytkownika
 */
int main(int argc, char *argv[])
{
	srand(time(nullptr));
	if(argc != 13 || strcmp(argv[1],"-i") != 0 || strcmp(argv[3], "-o") != 0 || strcmp(argv[5], "-w") != 0 || strcmp(argv[7],"-r") != 0 || strcmp(argv[9],"-p") != 0 || strcmp(argv[11],"-k") != 0)
	{
		std::cout << "Podano bledne parametry wejsciowe, sprobuj ponownie" << std::endl;
		std::cout << "Format parametrow wejsciowych : " << std::endl;
		std::cout << "-i <plik_wejsciowy> -o <plik_wyjsciowy> -w <wspolczynnik_wymierania> -r <wspolczynnik_rozmnazania> -p <liczba_pokolen> -k <liczba_par_osobnikow_do_krzyzowania>" << std::endl;
	}
	else
	{
		const std::string filename_in = argv[2];
		const std::string filename_out = argv[4];
		double w = std::stod(argv[6]);
		double r = std::stod(argv[8]);
		int p = std::stoi(argv[10]);
		int k = std::stoi(argv[12]);

		if (w >= 0 && r > w && r >= 0 && filename_out.length() > 3 && p > 0 && k > 0)
		{
			save(filename_in, filename_out, w, r, p, k);
			std::cout << "Dziekujemy za skorzystanie z programu!" << std::endl;
		}
		else
		{
			std::cout << "Podano bledne parametry wejsciowe, sprobuj ponownie" << std::endl;
		    std::cout << "Format parametrow wejsciowych : " << std::endl;
		    std::cout << "-i <plik_wejsciowy> -o <plik_wyjsciowy> -w <wspolczynnik_wymierania> -r <wspolczynnik_rozmnazania> -p <liczba_pokolen> -k <liczba_par_osobnikow_do_krzyzowania>" << std::endl;
		}
	}
	return 0;
}