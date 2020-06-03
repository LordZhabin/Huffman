#include <iostream>
#include <vector>																//вектор
#include <list>																	//список 
#include <map>																	//ассициативный массив
#include <fstream>

using namespace std;


struct Node {																	//класс бинарное дерево
//public:
	int a;																		//число
	char c;																		//символ
	Node* left, * right;														//правый и левый потомок

	Node() { left = right = NULL; }												//перегрузка скобок

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
map < char, vector<bool> > table;

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
	// cчитывание шапки
	int x;

	ifstream F;																	//создание переменную, куда поместим кодировку текст
	ofstream gotov;																//создание переменную, куда поместим раскодированные текст
	gotov.open("gotov.txt", ios::binary);										//открываем файл для результата
	F.open("output.txt", ios::binary);											//открываем файл для декадировки					
	setlocale(LC_ALL, "Russian");												// чтоб русские символы отображались в командной строке

	list<Node*> t;

	int a, b;
	char q;
	map<char, int> m;															//ассоциативный массив для таблицы кодирования

	map<char, int>::iterator i;

	// считывание из файла output.txt и преобразование обратно
	int number = 0;

	F.read((char*)&a, sizeof(a));												//считываем первое число для расшифровки таблциы																//отнимаем значение int
	while (a > 0) {
		F.read((char*)&q, sizeof(q));											//считываем символ
		F.read((char*)&b, sizeof(b));											//считываем его значение
		a -= 40;																//отнимаем 8 бит символа и 32 его значения
		m[q] = b;																//строим таблицу
		number++;
	}

	cout << "Вывод таблицы символ: количество символов" << endl;
	for (i = m.begin(); i != m.end(); ++i)	cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i) {									//записываем узлы в начальный список
		Node* p = new Node;														//создаем в динамической памяти нвоый узел
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);															//указатель скидываем в list
	}

	cout << "Количество разных символов:" << number << endl;

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
	int count = 0;

	Node* p = root;
	BuildTable(root);
	count = 0;
	char byte = F.get();															//считываем с файла пока не коничтся
	while (!F.eof()) {															//если 0 то идем в лево
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right;													//если р = 1, то идем на право
		else p = p->left;														//если р = 0, то идем на лево
		if (p->left == NULL && p->right == NULL) {								//если слева 0 и спарва 0, то ...
			gotov << p->c;
			p = root;
		}
		count++;

		if (count == 8) {
			count = 0;
			F >> byte;
		}
	}
		F.close();
		gotov.close();

		m.clear();
		cout << "Файл декодирован" << endl;
		return 0;
	
}