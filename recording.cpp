#include <iostream>
#include <vector>																//вектор
#include <list>																	//список 
#include <map>																	//ассициативный массив
#include <fstream>


using namespace std;

class Node {																	//класс бинарное дерево
public:
	int a;																		//число
	char c;																		//символ
	Node* left, * right;														//правый и левый потомок

	Node() {}																	//перегрузка скобок

	Node(Node* L, Node* R) {													//перегрузка
		left = L;
		right = R;
		a = L->a + R->a;
	}
};

struct Compare {																//структура, которая помогает сравнивать 
	bool operator() (Node* l, Node* r) const
	{
		return l->a < r->a;
	}
};


vector<bool> code;																//глобальная переменная, чтобы собрать наш код в вектор
map <char, vector<bool> > table;												//глобальная переменная, ассоциация символа с кодом - таблица

void BuildTable(Node* root) {
	if (root->left != NULL) {													//если слева не ноль(т.е. существует потомок), то заходим в наш вектор
		code.push_back(0);														//пошли по левому ребру - 0
		BuildTable(root->left);													//запускаем рекурсивно дальнейший обход для левого потомка
	}

	if (root->right != NULL) {													//если справа не ноль(т.е. существует потомок), то заходим в наш вектор
		code.push_back(1);														//пошли по правому ребру - 1
		BuildTable(root->right);												//запускаем рекурсивно дальнейший обход для правого потомка
	}

	if (root->left == NULL && root->right == NULL) table[root->c] = code;		//если буква существует, то ассациируем эту букву с нашим кодом
	if (root->c) table[root->c] = code;
	if (!code.empty())	code.pop_back();										//код сокращаем на единицу, т.к. возвращаемся обратно от потомка к вершине, т.е. код узла на 1 символ меньше
}



int main() {
	setlocale(LC_ALL, "Russian");

	int number = 0;																//количество частот(т.е. кол-во символов)
	char c;

	ifstream f;																	//созданем файла для чтения
	ofstream g;																	//созданем файла для закодированного текста

	f.open("2.txt", ios::binary);												//открываем файл для чтения
	g.open("output.txt", ios::binary);											//открываем файл для кодировки
// записываем начальные узлы в список list

	list<Node*> t;																//cписок указателей на Node
	map<char, int>::iterator i;

	cout << "Вывод символов: количество символов в текстовом документе" << endl;
	// считаем частоты символов
	map<char, int> m;															//массив
	while (f.get(c)) {															//пока не конец файла, считаем кол-во элементов
		m[c]++;																	//считываем из файла текст (включая пробелы)
	}

	for (i = m.begin(); i != m.end(); ++i)
		cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i) {									//записываем узлы в начальный список
		Node* p = new Node;														//создаем в динамической памяти нвоый узел
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);															//указатель скидываем в list
		number++;
	}
	cout << "Количество разных символов:" << number << endl;

	// создаем дерево	

	while (t.size() != 1) {														//цикл построения дерева. последний элемент - будет являтся корнем
		t.sort(Compare());													//сортировка

		Node* SonL = t.front();													//вспомогательный элемент присваю к первому в списке
		t.pop_front();															//удаляем
		Node* SonR = t.front();
		t.pop_front();

		Node* parent = new Node(SonL, SonR);									//созднаие узла-родителя двух потомков, где левый элемент будет - 0, а правый элемент - 1
		t.push_back(parent);													//кладем отца в список ??push_front();
	}

	Node* root = t.front();														//указатель на корневой элемент


// создание пар символ - код (построение таблицы) и запись в файл

	BuildTable(root);															//создание таблицы		

	int chet = 0;
	for (map<char, int>::iterator it = m.begin(); it != m.end(); it++) {		// с помощью итератора проходим по элементам и делаем подсчет кол-ва элементов
		if (it->second != 0) {
			char s = it->first;
			vector<bool> x = table[s];
			chet += 40;															//прбавляем к таблице 40 символов стоко раз, скоко частот. так как 8 бит занимает символ и 32 бита - его год		
		}
	}

	cout << "Таблица частот создана." << endl;									//забивка таблицы в кодирвоанный файл
	g.write((char*)(&chet), sizeof(chet));										//записывает кол-во символов элементов (количество частот)
	for (int l = 0; l < 256; l++) {												//т.к 256 символа
		if (m[char(l)] > 0) {
			c = char(l);
			g.write((char*)(&c), sizeof(c));									//забивка символа
			g.write((char*)(&m[char(l)]), sizeof(m[char(l)]));					//забивка значения символа
		}
	}

	f.clear();																	//сброс, чтоб не возникло ошибок
	f.seekg(0);																	//перемещение указателя на ноль

	int count = 0;																//счетсчик
	char buf = 0;																// вспомогательная переменная

	while (!f.eof()) {															//записываем наш текст в файл
		char c = f.get();														//вспомогательные пременные с и х
		vector<bool> x = table[c];

		for (int n = 0; n < x.size(); n++)										//вывод массива. преобразует вектор 0 и 1
		{
			buf = buf | x[n] << (7 - count);									//записываем его в байт с лева на право
			count++;
			if (count == 8) {													//как только достигли 8, записываем байт в файл output
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}

	cout << "Ваш текстовый файл закодирован." << endl;

	t.clear();
	m.clear();
	f.close();																	//закрываем файлы
	g.close();

	return 0;
}