#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef unsigned char BASE;
typedef unsigned short DBASE;

class BigNumber {
    BASE* number;
    int len, maxlen;
public:
    BigNumber(int n = 0, int mode = 0) { // конструктор
        maxlen = n;
        number = new BASE[maxlen + 1];

        switch (mode) {//в зависимости от поступаемого параметра заполняем либо 0 либо рандомом
        case 0: {
            for (int i = 0; i < maxlen + 1; i++) number[i] = 0;
            len = 1;
            break;
        }
        case 1: {
            for (int i = 0; i < maxlen; i++) number[i] = rand();
        }
              len = maxlen;
              zerominus(); //убираем лишний 0
        }
    }

    BigNumber(BigNumber& A) { //конструктор копирования
        number = new BASE[A.maxlen]; //создаем число
        len = A.len;
        for (maxlen = 0; maxlen < A.maxlen; number[maxlen] = A.number[maxlen], maxlen++);//заполняем число
    }

    ~BigNumber() { //деструктор
        if (number != NULL)
            delete[] number;
    }

    void obnulen() { // обнуление числа (заполнение нулями)
        for (int i = 0; i < len; i++) number[i] = 0;
        len = 1;
    }

    BigNumber& operator =(const BigNumber& A) { // оператор присвоения
        if (this != &A) { //проверка на равенство чисел
            delete[] number; //очищаем вектор
            number = new BASE[A.maxlen]; //создаем новый вектор заданной длинны
            len = A.len;
            for (maxlen = 0; maxlen < A.maxlen; number[maxlen] = A.number[maxlen], maxlen++); //заполняем
        }
        return *this;
    }

    void zerominus() {//убираем лишние 0
        while ((number[len - 1] == 0) && (len > 0)) //если значение вектора равно 0 и длина больше 0
            len--;

        if (len == 0) //если длинна равна 0, увеличиваем длинну до 1
            len++;

    }

    friend istream& operator >>(istream& r, BigNumber& A) {
        string str; //создаем строку и заполняем ее с клавиатуры
        cin >> str;
        BigNumber B(str.size(), 0); //cоздаем число с размером введенной строки и заполняем нулями
        int k = 0, l = 0;
        BASE tmp;
        for (int i = str.size() - 1; i >= 0; i--) {         //считываем строку и записываем в  tmp
            try { // начало блока "исключение", т.е. пытаться сделать
                if ((str[i] >= '0') && (str[i] <= '9'))         //заполнение в зависимости от символа
                    tmp = (int)str[i] - '0';
                else if (str[i] >= 'A' && str[i] <= 'F')
                    tmp = (int)str[i] - 'A' + 10;
                else if (str[i] >= 'a' && str[i] <= 'f')
                    tmp = (int)str[i] - 'a' + 10;
                else
                    throw - 1; //throw (бросить)-ключевое слово
            }
            catch (int j) { //начало блока, "ловящего" исключение, т.е. поймать
                cout << "Число введено неверно" << endl;
                break;
            }
            k = (str.size() - 1 - i) / (sizeof(BASE) * 2);//рассмтариваем коэффицент
            l = (str.size() - 1 - i) % (sizeof(BASE) * 2) * 4;//сдивг, длина сдвига
            B.number[k] |= tmp << l;
        }
        B.len = k + 1;
        A = B;
        A.zerominus();
        return r;
    }

    friend ostream& operator <<(ostream& r, BigNumber& A) {//вывод числа справа на лево
        int j = A.len - 1; //задаем длинну для поэлементного вывода
        cout << hex << (int)A.number[j];
        for (int i = j - 1; i >= 0; i--) {
            cout.width(sizeof(BASE) * 2);//ширина поля
            cout.fill('0'); //устанавливает символ заполнитель
            cout << hex << (int)A.number[i];//вывод   величин   в шестнадцатеричной системе   счисления 
        }
        return r;
    }

    //операции сложения и вычитания, сложения-присвоения и вычитания-присвоения
    BigNumber operator +(BigNumber& A) {//оператор сложения
        int j = 0;
        DBASE tmp;
        DBASE k = 0;
        BigNumber B(max(len, A.len) + 1, 0); //создание числа с размером мах длины и присвоение 0
        while (j < min(len, A.len)) {
            tmp = k + number[j] + A.number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
            B.len++;
        }
        while (j < len) {
            tmp = k + number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
            B.len++;
        }
        while (j < A.len) {
            tmp = k + A.number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
            B.len++;
        }
        B.len--;
        if (k) {
            B.len++;
            B.number[j] = k;
        }
        B.zerominus();
        return B;
    }

    BigNumber operator +=(BigNumber& A) {
        *this = *this + A;
        return *this;
    }

    BigNumber operator -(BigNumber& A) {//оператор вычитания
        int j = 0;
        int l = min(len, A.len), i;
        DBASE tmp;
        BASE k = 0;//////DBASE
        BigNumber B(len, 0); //создание числа с размером len и присвоение 0
        B.len = len;
        A.zerominus();
                for (i = 0; i < l; i++) {
                tmp = 1 << (sizeof(BASE) * 8);
                tmp += number[i];
                B.number[i] = tmp -= (A.number[i] + k);
                k = !(tmp >> sizeof(BASE) * 8);
            }
            for (; i < len; i++) {
                tmp = 1 << (sizeof(BASE) * 8);
                tmp += number[i];
                B.number[i] = tmp-= k;
                k = !(tmp >> sizeof(BASE) * 8);
            }
            B.zerominus();
            return B;
     }

    BigNumber operator -=(BigNumber& A) {
        *this = *this - A;
        return *this;
    }

    //операции сравнения БЧ
    bool operator <(BigNumber& A) {
        if (len < A.len)
            return true;
        if (len > A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) {//сравниваем поэлиментно
            if (number[i] > A.number[i])
                return false;
            if (number[i] < A.number[i])
                return true;
        }
        return false;
    }

    bool operator >(BigNumber& A) {
        if (len > A.len)
            return true;
        if (len < A.len)
            return
            false;
        for (int i = len - 1; i >= 0; i--) { //сравниваем поэлиментно
            if (number[i] > A.number[i])
                return true;
            if (number[i] < A.number[i])
                return false;
        }
        return false;
    }

    bool operator <=(BigNumber& A) {
        if (len < A.len)
            return true;
        if
            (len > A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) { //сравниваем поэлиментно
            if (number[i] > A.number[i])
                return false;
            if (number[i] < A.number[i])
                return true;
        }
        return true;
    }

    bool operator >=(BigNumber& A) {
        if (len > A.len)
            return true;
        if (len < A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) { //сравниваем поэлиментно
            if (number[i] > A.number[i])
                return true;
            if (number[i] < A.number[i])
                return false;
        }
        return true;
    }

    bool operator ==(BigNumber& A) {
        if (len == A.len) {
            for (int i = len - 1; i >= 0; i--) { //сравниваем поэлиментно
                if (number[i] != A.number[i])
                    return false;
            }
        }
        else
            return false;
        return true;
    }

    bool operator !=(BigNumber& A) {
        if (len == A.len) {
            for (int i = len - 1; i >= 0; i--) { //сравниваем поэлиментно
                if (number[i] != A.number[i])
                    return true;
            }
        }
        else
            return true;
        return false;
    }

    //оператор умножения
    BigNumber operator *(BigNumber& A) {
        int j = 0, i = 0;
        DBASE k;
        DBASE tmp;
        BigNumber B(len + A.len, 0);
        B.len = len + A.len;
        while (j < A.len) {
            i = 0;
            k = 0;
            while (i < len) {
                tmp = (DBASE)number[i] * A.number[j] + k + B.number[i + j];
                B.number[i + j] = tmp;
                k = tmp >> (int)sizeof(BASE) * 8;
                i++;
            }
            B.number[i + j] = k;
            j++;
        }
        B.zerominus();
        return B;
    }

    // умножение и деление, сложения и вычитания
    BigNumber mul(BASE a) {//умнажение чисел на базу
        int i = 0;
        DBASE tmp = 0;
        DBASE r = 0;
        BigNumber B(len + 1, 0);
        B.len = len + 1;
        while (i < len) {
            tmp = (DBASE)r + number[i] * a;
            B.number[i] = tmp;
            r = tmp >> sizeof(BASE) * 8;
            i++;
        }
        if (r != 0) {
            B.number[i] = r;
        }
        B.zerominus();
        return B;
    }

    BigNumber div(BASE a, BASE& R) { //деление на числа
        int j = len - 1;        //j-счётчик
        BASE r = 0;             // остаток от деления коэф БЧ на число
        DBASE tmp;              //результат
        BigNumber B(len, 0);       //Создаём BigNumber и зануляем
        B.len = len;
        while (j >= 0) {
            tmp = (r << (int)sizeof(BASE) * 8) + number[j];//number[j] + ((DBASE)r << (int)sizeof(BASE) * 8);  //сдвиг остатка на разряд и прибавление следующего коэффицента
            B.number[j] = tmp / a;                                  //деление на число
            r = (DBASE)tmp % a;                                     //разность
            j--;
        }
        R = r;
        B.zerominus();
        return B;
    }

    BigNumber sumBASE(BASE A) {
        int j = 0;
        DBASE k = 0;
        DBASE tmp = 0;
        BigNumber B(len + 1, 0);
        B.len = len + 1;
        tmp = (DBASE)k + A + number[j];
        B.number[j] = tmp;
        k = tmp >> sizeof(BASE) * 8;
        j++;
        while (j < len) {
            tmp = k + number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
        }
        B.number[j] = k;
        B.zerominus();
        return B;
    }

    int subBN(BigNumber& A, int i, int t) {//вычитание
        DBASE tmp = 0;
        DBASE k = 0;
        int j = i;
        int l = 0;

        while (j <= i + t) {
            tmp = (DBASE)1 << sizeof(BASE) * 8;
            tmp += number[j];
            tmp -= A.number[l] + k;
            number[j] = tmp;
            k = !(tmp >> sizeof(BASE) * 8);
            j++;
            l++;
        }
        zerominus();
        int f = k;
        return f;
    }

    void sumBN(BigNumber& A, int i) {
        int j = i;
        int l = 0;
        DBASE k = 0;
        DBASE tmp;
        while (j <= i + A.len) {
            tmp = (DBASE)k + number[j] + A.number[l]; 
            number[j] = tmp; 
            k = tmp >> sizeof(BASE) * 8;
            j++;
            l++;
        }
        number[j] += k;
        zerominus();
    }

     //операторы деления
     BigNumber operator /(BigNumber& A) {        //деление
        BigNumber B(len, 0);
        BigNumber copyTHIS;             //частное от деления
        BigNumber copyTHIS2;                //создание бч для копии
        BigNumber copyA;
        int flag = 0;
        int del = len - A.len;//длина
        if (A.len >= 1 && A != B && *this >= A) {    //Проверка перед делением Т.К. B - нулевой

            DBASE qz = 0; //временное частное
            DBASE d = ((DBASE)1 << sizeof(BASE) * 8) / (A.number[A.len - 1] + 1);//нормализатор последняя цифра делителя +1
            copyTHIS = *this;
            copyTHIS2 = *this;
            copyA = A;
            if (d != 1) {
                copyTHIS = copyTHIS.mul(d);             // нормализируем
                copyTHIS2 = copyTHIS2.mul(d);
                copyA = A.mul(d);
            }
            if (len == copyTHIS.len) {//проверка на длинну чисел получшившего делимого не изменилась то в последний коэфицент доабвить 0
                delete[] copyTHIS.number;
                copyTHIS.number = new BASE[copyTHIS2.len + 1];//увеличиваем массив коэфицентов
                copyTHIS.maxlen++;
                copyTHIS.len++; //увеличиваем сами переменные
                for (int i = 0; i < copyTHIS2.len; i++) {//цикл работает столько раз сколько длина цикла
                    copyTHIS.number[i] = copyTHIS2.number[i];
                }
                copyTHIS.number[copyTHIS.len - 1] = 0;
            }
            int j = del;
            int k = copyA.len;   //длина вторго числа
            while (j >= 0) {
                qz = (((DBASE)(copyTHIS.number[j + k] << sizeof(BASE) * 8)) + copyTHIS.number[j + k - 1]) / copyA.number[k - 1];
             
                while (qz * copyA.number[k - 2] > (copyTHIS.number[j + k] * ((DBASE)1 << sizeof(BASE) * 8) + copyTHIS.number[j + k - 1] - qz * copyA.number[k - 1]) * ((DBASE)1 << sizeof(BASE) * 8) + copyTHIS.number[j + k - 2]) {
                     qz--;  
                    
                     
                }// находим qz и записываем в частное

                BigNumber copyA2;
                copyA2 = copyA;
                int t = copyA.len;
                copyA2 = copyA.mul(qz);
                flag = copyTHIS.subBN(copyA2, j, t);// вычитаем из части числа часть другого числа
                B.number[j] = qz;
                B.len++;
                if (flag) {
                    copyTHIS.sumBN(copyA, j);// если отриц возвращем число  в положит
                    B.number[j]--;// если был заим частное уменьшить на 1
                }
                j--;
                flag = 0;
            }
            B.zerominus();
        }
        return B;
    }

    BigNumber operator %(BigNumber& A) {//взятие остатка
        BigNumber R;
        BigNumber a;
        a = *this / A;
        R = a * A;
        R =  *this - R;
        R.zerominus();
        return R;
    }
   
    void printlen() {
        cout << dec << (int)len << " " << (int)maxlen << endl;
        for (int i = len - 1; i >= 0; i--) {
            cout << hex << (int)number[i] << " ";
        }
    }

    void change(int n) {
        BigNumber A(n, 1);
        *this = A;
    }

    BigNumber decCin(string str) {
        DBASE tmp;
        int j = 0;
        BigNumber A(str.length(), 0);
        A.len = str.length();
        while (j < str.length()) {
            tmp = str[j] - '0';
            A = A.mul((BASE)10);
            A = A.sumBASE(tmp);
            j++;
        }
        A.zerominus();
        return A;
    }

    void decCout() {
        int j = 0;
        BASE R = 0;
        string str;
        BigNumber A;
        int y;
        A = *this;
       
        char* s = new char[1000];
         while ((int)A.number[0] != 0) {
            A = A.div((BASE)10, R);
            y = (int)R + '0';
            s[j] = y;
            j++;
        }
        
        for (int i = j - 1; i >= 0; i--) {
            cout << s[i];
        }
        cout << endl;
    }
};

int main() {
  /* setlocale(LC_ALL, "Russian");
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    BigNumber A, B, C, D, P, L;

    cout << "A = ";
    cin >> A;
    P = A; L = P;
    cout << "B = ";
    cin >> B;
    C = A + B;
    cout << "A + B = " << C << endl;
    A += B;
    cout << "A+=B = " <<C << endl;

    C = P - B;
    cout << "A - B = " << P << " - " << B << C << endl;
    P -= B;
    cout << "A-=B = " << C << endl;
    C = A * B;
    cout << "A * B = " << C << endl;
    C = P / B;
    cout << "A / B = " << P << " / " << B <<" = " << C << endl;
    C = A % B;
    cout << "A % B = " << C << endl;
    if (A == B) cout << "A == B" << endl;
    else if (A > B) cout << "A > B" << endl;
    else if (A < B) cout << "A < B" << endl;

    cout << endl << "BASE" << endl;
    int p;
    cin >> p;
    C = A.mul(p);
    cout << "A * p = " << C << endl;
    BASE R = 0;
    C = A.div((BASE)p, R);
    cout << "A / p = " << C << endl;

    BigNumber X;
    string x1;
    cin >> x1;
    size_t found = x1.find_first_not_of("1234567890");
    if (found != string::npos) {
        std::cout << "Вы ввели некоректное 10тичное число " << std::endl;
    }
    else {
        X = X.decCin(x1);
        cout << "Число в 16 сс - " << X << endl;
        X.decCout();
    }
    return 0;*/
    
    srand(unsigned(time(0)));
    int M = 1000;
    int T = 1000;
    BigNumber F;
    BigNumber E;
    BigNumber C;
    BigNumber R;
    BigNumber D;
    int n = rand() % M + 1;
    int m = rand() % M + 1;
    BigNumber A(n + 1, 1);
    BigNumber B(m + 1, 1);
    
   do {
        n = rand() % M + 1;
        m = rand() % M + 1;
        A.change(n + 1);
        B.change(m + 1);
        if (A > B) {
            C = A / B;
            cout << C;
            R = A % B;
            F = C * B;
            E = F + R;
            D = A - R;
        }
        if (B > A) {
            C = B / A;
            cout << C;
            R = B % A;
            F = C * A;
            E = F + R;
            D = B - R;
        }
        T--;
        cout << endl;
        cout << "T = ";
        cout << dec << T;
        cout << endl;
    } while (((A == E) && (D == F) && (B > R) && (T > 0)) || ((B == E) && (D == F) && (A > R) && (T > 0)));
    return 0;
}
