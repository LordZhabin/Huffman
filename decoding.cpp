#include <iostream>
#include <vector>																//������
#include <list>																	//������ 
#include <map>																	//������������� ������
#include <fstream>

using namespace std;


struct Node {																	//����� �������� ������
//public:
	int a;																		//�����
	char c;																		//������
	Node* left, * right;														//������ � ����� �������

	Node() { left = right = NULL; }												//���������� ������

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
map < char, vector<bool> > table;

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
	// c��������� �����
	int x;

	ifstream F;																	//�������� ����������, ���� �������� ��������� �����
	ofstream gotov;																//�������� ����������, ���� �������� ��������������� �����
	gotov.open("gotov.txt", ios::binary);										//��������� ���� ��� ����������
	F.open("output.txt", ios::binary);											//��������� ���� ��� �����������					
	setlocale(LC_ALL, "Russian");												// ���� ������� ������� ������������ � ��������� ������

	list<Node*> t;

	int a, b;
	char q;
	map<char, int> m;															//������������� ������ ��� ������� �����������

	map<char, int>::iterator i;

	// ���������� �� ����� output.txt � �������������� �������
	int number = 0;

	F.read((char*)&a, sizeof(a));												//��������� ������ ����� ��� ����������� �������																//�������� �������� int
	while (a > 0) {
		F.read((char*)&q, sizeof(q));											//��������� ������
		F.read((char*)&b, sizeof(b));											//��������� ��� ��������
		a -= 40;																//�������� 8 ��� ������� � 32 ��� ��������
		m[q] = b;																//������ �������
		number++;
	}

	cout << "����� ������� ������: ���������� ��������" << endl;
	for (i = m.begin(); i != m.end(); ++i)	cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i) {									//���������� ���� � ��������� ������
		Node* p = new Node;														//������� � ������������ ������ ����� ����
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);															//��������� ��������� � list
	}

	cout << "���������� ������ ��������:" << number << endl;

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
	int count = 0;

	Node* p = root;
	BuildTable(root);
	count = 0;
	char byte = F.get();															//��������� � ����� ���� �� ��������
	while (!F.eof()) {															//���� 0 �� ���� � ����
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right;													//���� � = 1, �� ���� �� �����
		else p = p->left;														//���� � = 0, �� ���� �� ����
		if (p->left == NULL && p->right == NULL) {								//���� ����� 0 � ������ 0, �� ...
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
		cout << "���� �����������" << endl;
		return 0;
	
}