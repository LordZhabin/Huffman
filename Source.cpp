#include <iostream>
#include <vector>							//вектор
#include <list>								//список 
#include <map>								//ассициативный массив
#include <fstream>
// иттератор - это такая структура данных, которая используется для обращения к определенному элементу в контейнерах

using namespace std;


class Node {								//бинарное дерево
public:
	int a;									//число
	char c;									//символ
	Node* left, * right;					//правый и левый потомок

	Node() {}								//перегрузка

	Node(Node* L, Node* R) {				//перегрузка
		left = L;
		right = R;
		a = L->a + R->a;
	}
};

struct MyCompare {
	bool operator() (Node* l, Node* r) const
	{
		return l->a < r->a;
	}
};

void print(Node* root, unsigned k = 0) { //визуальное построение дерева
	if (root != NULL) {
		print(root->left, k + 3);
		for (unsigned i = 0; i < k; i++) cout << "  ";

		if (root->c) cout << root->a << "  (" << root->c << ")" << endl;
		else cout << root->a << endl;
		print(root->right, k + 3);
	}
	}

vector<bool> code;							//глобальная переменная, чтобы собрать наш код в вектор
map <char, vector<bool> > table;			//глобальная переменная, ассоциация символа с кодом - таблица

void BuildTable(Node* root) {
	if (root->left != NULL) {				//если слева не ноль(т.е. существует потомок), то заходим в наш вектор
		code.push_back(0);					//пошли по левому ребру - 0
		BuildTable(root->left);				//запускаем рекурсивно дальнейший обход для левого потомка
	}

	if (root->right != NULL) {				//если справа не ноль(т.е. существует потомок), то заходим в наш вектор
		code.push_back(1);					//пошли по правому ребру - 1
		BuildTable(root->right);			//запускаем рекурсивно дальнейший обход для правого потомка
	}

	if (root->left == NULL && root->right == NULL) table[root->c] = code;		//если буква существует, то ассациируем эту букву с нашим кодом
	if (root->c) table[root->c] = code;
	if (!code.empty())
		code.pop_back();					//код сокращаем на единицу, т.к. возвращаемся обратно от потомка к вершине, т.е. код узла на 1 символ меньше
}



int main() {
	int number = 0; //количество частот(т.е. кол-во символов)
	char c;

// считаем частоты символов
	ifstream f("1.txt");
	map<char, int> m;						//массив
		while (f.get(c)) {					//пока не конец файла, считаем кол-во элементов
			m[c]++;							//считываем из файла текст (включая пробелы)
		}


// записываем начальные узлы в список list

	list<Node*> t;							//cписок указателей на Node
	map<char, int>::iterator i;
	
	for (i = m.begin(); i != m.end(); ++i)
		cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i){ //записываем узлы в начальный список
		Node* p = new Node;					//создаем в динамической памяти нвоый узел
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);						//указатель скидываем в list
		number++;
	}
	cout << "kol-vo simvolov:"<< number <<endl;


//  создаем дерево	

	while (t.size() != 1) {					//цикл построения дерева. последний элемент - будет являтся корнем
		t.sort(MyCompare());				//сортировка
		
		Node* SonL = t.front();				//вспомогательный элемент присваю к первому в списке
		t.pop_front();						//удаляем
		Node* SonR = t.front();
		t.pop_front();

		Node* parent = new Node(SonL, SonR); //созднаие узла-родителя двух потомков, где левый элемент будет - 0, а правый элемент - 1
		t.push_back(parent);				 //кладем отца в список ??push_front();
	}

	Node* root = t.front();					 //указатель на корневой элемент
							
	//print(root);

// создание пар символ - код (построение таблицы) и запись в файл

	ofstream g("output.txt");				//вывод файла, т.е. наш закодированные текст
	
	int chet = 0, max = 0;;
	BuildTable(root);			

	for (map<char, int>::iterator it = m.begin(); it != m.end(); it++) {//подсчет кол-ва элементов
		if (it->second != 0) {
			char c = it->first;
			vector<bool> x = table[c];
			chet += 40;//прбавляем к таблице 40 символов стоко раз, скоко частот
			max += (x.size() * (it->second));//Количество памяти, которое будет занимать закодированное сообщение
		}
	}

	chet += 32;

	cout << "shapka ukomplektivana" << endl; //шапка
	g.write((char*)(&chet), sizeof(chet));//записывает кол-во символов элементов (количество частот)
	for (int i = 0; i < 256; i++) {//т.к 256 символа
		if (m[char(i)] > 0) {
			c = char(i);
			g.write((char*)(&c), sizeof(c));
			g.write((char*)(&m[char(i)]), sizeof(m[char(i)]));
		}
	}

	f.clear();								//сброс, чтоб не возникло ошибок
	f.seekg(0);								//перемещение указателя на ноль


	int count = 0;							//счетсчик
	char buf = 0;							// вспомогательная переменная

	
	while (!f.eof()) {
		char c = f.get();								//вспомогательные пременные с и х
		vector<bool> x = table[c];

		for (int n = 0; n < x.size(); n++)	 //вывод массива. преобразует вектор 0 и 1
		{
			buf = buf | x[n] << (7 - count); //записываем его в байт с лева на право
			count++;
			if (count == 8) {				//как только достигли 8, записываем байт в файл output
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}

	f.close();//закрываем файлы
	g.close();
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//cчитывание шапки
	int x;
	ifstream F("output.txt");				//открываем файл
	ofstream gotov("gotov.txt");			//создание файлы текстового - раскодированногоint xx, yy;
	
	int xx, yy;
	char zz;

	F.read((char*)&xx, sizeof(xx));
	xx -= 32;
	while (xx > 0) {
		F.read((char*)&zz, sizeof(zz));
		F.read((char*)&yy, sizeof(yy));
		xx -= 40;
		m[zz] = yy;
	}

// считывание из файла output.txt и преобразование обратно
	
	//F.read((char*)&x, sizeof(x));//читает кол-во символов элементов (количество частот)
	//cout << "sinmvol " << x; //вывод кол-ва частот

	Node* p = root;
	count = 0;
	char byte = F.get();					//считываем с файла пока не коничтся
	while (!F.eof()) {						//если 0 то идем в лево
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right;				//если р = 1, то идем на право
		else p = p->left;					//если р = 0, то идем на лево
		if (p->left == NULL && p->right == NULL) {	//если слева 0 и спарва 0, то ...
			//cout << p->c;
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
	t.clear();


	return 0;
}