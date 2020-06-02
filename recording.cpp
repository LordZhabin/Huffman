#include <iostream>
#include <vector>																//������
#include <list>																	//������ 
#include <map>																	//������������� ������
#include <fstream>


using namespace std;

class Node {																	//����� �������� ������
public:
	int a;																		//�����
	char c;																		//������
	Node* left, * right;														//������ � ����� �������

	Node() {}																	//���������� ������

	Node(Node* L, Node* R) {													//����������
		left = L;
		right = R;
		a = L->a + R->a;
	}
};

struct Compare {																//���������, ������� �������� ���������� 
	bool operator() (Node* l, Node* r) const
	{
		return l->a < r->a;
	}
};


vector<bool> code;																//���������� ����������, ����� ������� ��� ��� � ������
map <char, vector<bool> > table;												//���������� ����������, ���������� ������� � ����� - �������

void BuildTable(Node* root) {
	if (root->left != NULL) {													//���� ����� �� ����(�.�. ���������� �������), �� ������� � ��� ������
		code.push_back(0);														//����� �� ������ ����� - 0
		BuildTable(root->left);													//��������� ���������� ���������� ����� ��� ������ �������
	}

	if (root->right != NULL) {													//���� ������ �� ����(�.�. ���������� �������), �� ������� � ��� ������
		code.push_back(1);														//����� �� ������� ����� - 1
		BuildTable(root->right);												//��������� ���������� ���������� ����� ��� ������� �������
	}

	if (root->left == NULL && root->right == NULL) table[root->c] = code;		//���� ����� ����������, �� ����������� ��� ����� � ����� �����
	if (root->c) table[root->c] = code;
	if (!code.empty())	code.pop_back();										//��� ��������� �� �������, �.�. ������������ ������� �� ������� � �������, �.�. ��� ���� �� 1 ������ ������
}



int main() {
	setlocale(LC_ALL, "Russian");

	int number = 0;																//���������� ������(�.�. ���-�� ��������)
	char c;

	ifstream f;																	//�������� ����� ��� ������
	ofstream g;																	//�������� ����� ��� ��������������� ������

	f.open("2.txt", ios::binary);												//��������� ���� ��� ������
	g.open("output.txt", ios::binary);											//��������� ���� ��� ���������
// ���������� ��������� ���� � ������ list

	list<Node*> t;																//c����� ���������� �� Node
	map<char, int>::iterator i;

	cout << "����� ��������: ���������� �������� � ��������� ���������" << endl;
	// ������� ������� ��������
	map<char, int> m;															//������
	while (f.get(c)) {															//���� �� ����� �����, ������� ���-�� ���������
		m[c]++;																	//��������� �� ����� ����� (������� �������)
	}

	for (i = m.begin(); i != m.end(); ++i)
		cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i) {									//���������� ���� � ��������� ������
		Node* p = new Node;														//������� � ������������ ������ ����� ����
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);															//��������� ��������� � list
		number++;
	}
	cout << "���������� ������ ��������:" << number << endl;

	// ������� ������	

	while (t.size() != 1) {														//���� ���������� ������. ��������� ������� - ����� ������� ������
		t.sort(Compare());													//����������

		Node* SonL = t.front();													//��������������� ������� ������� � ������� � ������
		t.pop_front();															//�������
		Node* SonR = t.front();
		t.pop_front();

		Node* parent = new Node(SonL, SonR);									//�������� ����-�������� ���� ��������, ��� ����� ������� ����� - 0, � ������ ������� - 1
		t.push_back(parent);													//������ ���� � ������ ??push_front();
	}

	Node* root = t.front();														//��������� �� �������� �������


// �������� ��� ������ - ��� (���������� �������) � ������ � ����

	BuildTable(root);															//�������� �������		

	int chet = 0;
	for (map<char, int>::iterator it = m.begin(); it != m.end(); it++) {		// � ������� ��������� �������� �� ��������� � ������ ������� ���-�� ���������
		if (it->second != 0) {
			char s = it->first;
			vector<bool> x = table[s];
			chet += 40;															//��������� � ������� 40 �������� ����� ���, ����� ������. ��� ��� 8 ��� �������� ������ � 32 ���� - ��� ���		
		}
	}

	cout << "������� ������ �������." << endl;									//������� ������� � ������������ ����
	g.write((char*)(&chet), sizeof(chet));										//���������� ���-�� �������� ��������� (���������� ������)
	for (int l = 0; l < 256; l++) {												//�.� 256 �������
		if (m[char(l)] > 0) {
			c = char(l);
			g.write((char*)(&c), sizeof(c));									//������� �������
			g.write((char*)(&m[char(l)]), sizeof(m[char(l)]));					//������� �������� �������
		}
	}

	f.clear();																	//�����, ���� �� �������� ������
	f.seekg(0);																	//����������� ��������� �� ����

	int count = 0;																//��������
	char buf = 0;																// ��������������� ����������

	while (!f.eof()) {															//���������� ��� ����� � ����
		char c = f.get();														//��������������� ��������� � � �
		vector<bool> x = table[c];

		for (int n = 0; n < x.size(); n++)										//����� �������. ����������� ������ 0 � 1
		{
			buf = buf | x[n] << (7 - count);									//���������� ��� � ���� � ���� �� �����
			count++;
			if (count == 8) {													//��� ������ �������� 8, ���������� ���� � ���� output
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}

	cout << "��� ��������� ���� �����������." << endl;

	t.clear();
	m.clear();
	f.close();																	//��������� �����
	g.close();

	return 0;
}