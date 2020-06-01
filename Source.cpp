#include <iostream>
#include <vector>							//������
#include <list>								//������ 
#include <map>								//������������� ������
#include <fstream>
// ��������� - ��� ����� ��������� ������, ������� ������������ ��� ��������� � ������������� �������� � �����������

using namespace std;


class Node {								//�������� ������
public:
	int a;									//�����
	char c;									//������
	Node* left, * right;					//������ � ����� �������

	Node() {}								//����������

	Node(Node* L, Node* R) {				//����������
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

void print(Node* root, unsigned k = 0) { //���������� ���������� ������
	if (root != NULL) {
		print(root->left, k + 3);
		for (unsigned i = 0; i < k; i++) cout << "  ";

		if (root->c) cout << root->a << "  (" << root->c << ")" << endl;
		else cout << root->a << endl;
		print(root->right, k + 3);
	}
	}

vector<bool> code;							//���������� ����������, ����� ������� ��� ��� � ������
map <char, vector<bool> > table;			//���������� ����������, ���������� ������� � ����� - �������

void BuildTable(Node* root) {
	if (root->left != NULL) {				//���� ����� �� ����(�.�. ���������� �������), �� ������� � ��� ������
		code.push_back(0);					//����� �� ������ ����� - 0
		BuildTable(root->left);				//��������� ���������� ���������� ����� ��� ������ �������
	}

	if (root->right != NULL) {				//���� ������ �� ����(�.�. ���������� �������), �� ������� � ��� ������
		code.push_back(1);					//����� �� ������� ����� - 1
		BuildTable(root->right);			//��������� ���������� ���������� ����� ��� ������� �������
	}

	if (root->left == NULL && root->right == NULL) table[root->c] = code;		//���� ����� ����������, �� ����������� ��� ����� � ����� �����
	if (root->c) table[root->c] = code;
	if (!code.empty())
		code.pop_back();					//��� ��������� �� �������, �.�. ������������ ������� �� ������� � �������, �.�. ��� ���� �� 1 ������ ������
}



int main() {
	int number = 0; //���������� ������(�.�. ���-�� ��������)
	char c;

// ������� ������� ��������
	ifstream f("1.txt");
	map<char, int> m;						//������
		while (f.get(c)) {					//���� �� ����� �����, ������� ���-�� ���������
			m[c]++;							//��������� �� ����� ����� (������� �������)
		}


// ���������� ��������� ���� � ������ list

	list<Node*> t;							//c����� ���������� �� Node
	map<char, int>::iterator i;
	
	for (i = m.begin(); i != m.end(); ++i)
		cout << i->first << ":" << i->second << endl;

	for (i = m.begin(); i != m.end(); ++i){ //���������� ���� � ��������� ������
		Node* p = new Node;					//������� � ������������ ������ ����� ����
		p->c = i->first;
		p->a = i->second;
		t.push_back(p);						//��������� ��������� � list
		number++;
	}
	cout << "kol-vo simvolov:"<< number <<endl;


//  ������� ������	

	while (t.size() != 1) {					//���� ���������� ������. ��������� ������� - ����� ������� ������
		t.sort(MyCompare());				//����������
		
		Node* SonL = t.front();				//��������������� ������� ������� � ������� � ������
		t.pop_front();						//�������
		Node* SonR = t.front();
		t.pop_front();

		Node* parent = new Node(SonL, SonR); //�������� ����-�������� ���� ��������, ��� ����� ������� ����� - 0, � ������ ������� - 1
		t.push_back(parent);				 //������ ���� � ������ ??push_front();
	}

	Node* root = t.front();					 //��������� �� �������� �������
							
	//print(root);

// �������� ��� ������ - ��� (���������� �������) � ������ � ����

	ofstream g("output.txt");				//����� �����, �.�. ��� �������������� �����
	
	int chet = 0, max = 0;;
	BuildTable(root);			

	for (map<char, int>::iterator it = m.begin(); it != m.end(); it++) {//������� ���-�� ���������
		if (it->second != 0) {
			char c = it->first;
			vector<bool> x = table[c];
			chet += 40;//��������� � ������� 40 �������� ����� ���, ����� ������
			max += (x.size() * (it->second));//���������� ������, ������� ����� �������� �������������� ���������
		}
	}

	chet += 32;

	cout << "shapka ukomplektivana" << endl; //�����
	g.write((char*)(&chet), sizeof(chet));//���������� ���-�� �������� ��������� (���������� ������)
	for (int i = 0; i < 256; i++) {//�.� 256 �������
		if (m[char(i)] > 0) {
			c = char(i);
			g.write((char*)(&c), sizeof(c));
			g.write((char*)(&m[char(i)]), sizeof(m[char(i)]));
		}
	}

	f.clear();								//�����, ���� �� �������� ������
	f.seekg(0);								//����������� ��������� �� ����


	int count = 0;							//��������
	char buf = 0;							// ��������������� ����������

	
	while (!f.eof()) {
		char c = f.get();								//��������������� ��������� � � �
		vector<bool> x = table[c];

		for (int n = 0; n < x.size(); n++)	 //����� �������. ����������� ������ 0 � 1
		{
			buf = buf | x[n] << (7 - count); //���������� ��� � ���� � ���� �� �����
			count++;
			if (count == 8) {				//��� ������ �������� 8, ���������� ���� � ���� output
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}

	f.close();//��������� �����
	g.close();
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//c��������� �����
	int x;
	ifstream F("output.txt");				//��������� ����
	ofstream gotov("gotov.txt");			//�������� ����� ���������� - ����������������int xx, yy;
	
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

// ���������� �� ����� output.txt � �������������� �������
	
	//F.read((char*)&x, sizeof(x));//������ ���-�� �������� ��������� (���������� ������)
	//cout << "sinmvol " << x; //����� ���-�� ������

	Node* p = root;
	count = 0;
	char byte = F.get();					//��������� � ����� ���� �� ��������
	while (!F.eof()) {						//���� 0 �� ���� � ����
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right;				//���� � = 1, �� ���� �� �����
		else p = p->left;					//���� � = 0, �� ���� �� ����
		if (p->left == NULL && p->right == NULL) {	//���� ����� 0 � ������ 0, �� ...
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