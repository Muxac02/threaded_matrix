#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "mtxMult.h"

using namespace std::chrono_literals;

constexpr int iterations = 10;
constexpr auto sleepTime = 1000ms;


void sleepTest(double* res)
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "work started: \n";
    for (int i = 0; i < iterations; i++)
    {
        std::this_thread::sleep_for(sleepTime+200ms);
        std::cout << "iteration " << i << " id: " << std::this_thread::get_id() << ", ";
    }
    std::cout << "work ended.";
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    *res = elapsed.count();
}
class operations
{
public:
    static void sum()
{
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << "starting sum" << std::endl;
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << 2 << "+" << 5 << std::endl;
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << 2 + 5 << std::endl;
    std::this_thread::sleep_for(sleepTime - 50ms);
}
    void sub(int a, int b)
{
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << "starting sub" << std::endl;
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout<<a << "-" << b << std::endl;
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << a - b << std::endl;
    std::this_thread::sleep_for(sleepTime-50ms);
}
    int pow2(int a)
{
    std::this_thread::sleep_for(sleepTime - 50ms);
    std::cout << "starting pow" << std::endl;
    std::this_thread::sleep_for(sleepTime);
    std::cout << "a^2 = "<<a*a<<std::endl;
    std::this_thread::sleep_for(sleepTime);
    return a * a;
    std::this_thread::sleep_for(sleepTime - 50ms);
}
    void mult(int a, int b, int* res) {
        std::this_thread::sleep_for(sleepTime - 50ms);
        std::cout << "starting mult" << std::endl;
        std::this_thread::sleep_for(sleepTime - 50ms);
        std::cout << a << "*" << b << std::endl;
        std::this_thread::sleep_for(sleepTime - 50ms);
        std::cout << a * b << std::endl;
        std::this_thread::sleep_for(sleepTime - 50ms);
        *res = a * b;
    }
};
int main()
{
    const auto start = std::chrono::high_resolution_clock::now();
    setlocale(LC_ALL, "rus");
    //double *sleeptestTime = new double(0.0);
    //std::thread th(sleepTest, sleeptestTime);//detach - отсоединение потока сразу, join - присоединение потока по завершению
    //                                                            //например ниже идет вызов sleeptestTime, однако он произойдет
    //                                                            //однако он произойдет только по завершению потока
    //int *a = new int(4);
    //int *a2 = new int(0);
    //std::thread th2(sub, 4, 5, a);
    //th2.detach();
    //std::thread th3(sum, 4, 5, a2);
    //th3.detach();
    //std::cout << std::endl;
    //std::cout << "main started: \n";
    //for (int i = 0; i < iterations; i++)
    //{
    //    std::this_thread::sleep_for(sleepTime);
    //    std::cout << "iteration " << i << " id: " << std::this_thread::get_id() << std::endl;
    //}
    //std::cout << "main ended.";
    ////std::cout << sleeptestTime;
    //th.join();
    //std::cout << *a <<" "<<*a2<<" " << *sleeptestTime << "ms"<<std::endl;
    //int* c = new int(0);
    //std::thread th(pow2,11, c);
   // th.join();
    //=======================================
    //int** a = new int* [3];
    //a[0] = new int[3] {-1, 2, -5};
    //a[1] = new int[3] {3, 4, 1};
    //a[2] = new int[3] {0, 1, 2};

    //int** b = new int* [3];
    //b[0] = new int[3] {-1, 2, -5};
    //b[1] = new int[3] {3, 4, 1};
    //b[2] = new int[3] {0, 1, 2};

    //int** res = new int*[3];
    //res[0] = new int[3];
    //res[1] = new int[3];
    //res[2] = new int[3];
    ////std::thread th2(outMatr, pa, 3);
    ////getColumn(std::ref(a), 3, 3);
    ////outMatr(std::ref(a), 3);
    ////matrixMultiply(a, b, 3, res);
    ////std::thread th2(matrixMultiply, std::ref(a), std::ref(b), 3, 3, std::ref(res)); - почитать про std ref
    //std::thread th2([&a, &b, &res]() {matrixMultiply(a, b, 3, res); }); //для лямбда функции в [] указываем какие переменные из области видимости передаются в функции
    //th2.join();                                                         //в () указываем (объявляем) локальные переменные для лямбда функции
                                                                        //в {} указываем тело функции
    //======================================================================
    //int a = 5;
    //int b = 2;
    //int res = 0;
    //operations ops;
    //std::thread t1(ops.sum);//действителен, если метод статический (также можно вызвать operations::sum //без ссылки, как ниже)
    //t1.detach();
    //std::thread t2(&operations::sub,ops,a,b); //такой метод действительный если метод ничего не возвращает (void)  
    //t2.detach();
    //std::thread t3([a, &res,&ops]() {res = ops.pow2(a); });//удобно для методов, которые возвращают значение
    //t3.join();
    //std::this_thread::sleep_for(sleepTime-50ms);
    //std::cout << "\n" << a << ' ' << b << " " << res;
    //======================================================================
    int** a = new int* [3];
    a[0] = new int[3] {-1, 2, -5};
    a[1] = new int[3] {3, 4, 1};
    a[2] = new int[3] {0, 1, 2};
    
    int** b = new int* [3];
    b[0] = new int[3] {-1, 2, -5};
    b[1] = new int[3] {3, 4, 1};
    b[2] = new int[3] {0, 1, 2};

    Matrix mat1(a,3,3);
    Matrix mat2(b,3,3);
    Matrix mat3(mat1);

    mat3 = (mat1 + mat2);
    std::cout << "Mat3 add" << &mat3<<std::endl;
    mat3.outMatr();
    //(mat1 + mat2).outMatr();
    //(mat1 - mat2).outMatr();
    //(mat1 * mat2).outMatr();
    //std::thread t1([&mat1, &mat2]() {(mat1 * mat2).outMatr(); });
    //t1.join();
    //using func = void(Matrix::*)(Matrix&);//фикс для перегруженной фунции (происходит явное указание контекста)
    //std::thread t2(func(&Matrix::outMatr), std::ref(mat1), std::ref(mat2));//крч, лучше использовать лямбда выражение
    //t2.join();
    ////std::thread t4([&mat2]() {
    ////    mat2.outMatr();//       так или
    ////    mat2.outMatr(mat2);});//так, вообще есть идея сделать просто метод статический, 
    ////                          //однако тогда возникает проблема с видом с конструктором потока
    ////t4.detach();              //так что ниже вариант со статическим методом (проблема в том, что в статическом не надо передавтаь вызывающего)
    //std::thread t5(&Matrix::staticOutMatr, std::ref(mat2));
    //t5.join();
    //std::thread t3(&Matrix::matrMult, std::ref(mat1), std::ref(mat1), std::ref(mat2), std::ref(mat3));
    ////Передаю 1:ссылку на функцию, 2:объект, от которого идет вызов, 3:аргументы...
    //t3.join();
    //mat3.outMatr();
    //mat3.matrMult(mat1, mat2, mat3);
    
    //std::thread t10([&mat1, &mat2]() {(mat1 + mat2).outMatr(); });
    //t10.join();
    //std::thread t11([&mat1, &mat2, &mat3] {mat3 = (mat1 + mat2); });
   
    delete[] a[2],
    delete[] a[1],
    delete[] a[0],
    delete[] b[2],
    delete[] b[1],
    delete[] b[0];
    delete[] a,
    delete[] b;
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Выполнение программы заняло: " << elapsed.count() << "мс";
    return 0;
}