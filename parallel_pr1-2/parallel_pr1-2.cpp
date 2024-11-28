#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <vector>
#include <future>
#include "mtxMult.h"
#include "QuickSort.h"

using namespace std::chrono_literals;

constexpr int iterations = 10;
constexpr auto sleepTime = 1000ms;

std::thread::id MAIN_THREAD;

template <class T>
double startAndGetTimer(T (T::* method)(const T&),T& mat1, const T& mat2)
{
    const auto start = std::chrono::high_resolution_clock::now();
    (mat1.*method)(mat2);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

template <class T>
void startThread(T(T::* method)(const T&), T& mat1, const T& mat2, std::thread*& th)
{
    th = new std::thread([&mat1, &mat2, &method]() {(mat1.*method)(mat2);});
}

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

int partition(std::vector<int>& vec, int low, int high) {

    // Selecting last element as the pivot
    int pivot = vec[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (vec[j] <= pivot) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }

    // Put pivot to its position
    std::swap(vec[i + 1], vec[high]);

    // Return the point of partition
    return (i + 1);
}

int partitionMT(std::vector<int>& vec, int low, int high) {

    // Selecting last element as the pivot
    int pivot = vec[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (vec[j] <= pivot) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }

    // Put pivot to its position
    std::swap(vec[i + 1], vec[high]);

    // Return the point of partition
    return (i + 1);
}

void quickSort(std::vector<int>& vec, int low, int high) {

    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (low < high) {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partition(vec, low, high);

        // Separately sort elements before and after the
        // Partition Index pi
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}

void quickSortMT(std::vector<int>& vec, int low, int high, int depth = 0) {

    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (low < high) {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partitionMT(vec, low, high);

        // Separately sort elements before and after the
        // Partition Index pi
        if (depth < 0)
        {
            std::thread th1([&vec, &low, &pi, &depth]() {quickSortMT(vec, low, pi - 1, depth + 1); });
            std::thread th2([&vec, &high, &pi, &depth]() {quickSortMT(vec, pi + 1, high, depth + 1); });
            th1.join();
            th2.join();
        }
        else {
            quickSortMT(vec, low, pi - 1, depth + 1);
            quickSortMT(vec, pi + 1, high, depth + 1);
        }
        //std::thread th1([&vec, &low, &pi]() {quickSortMT(vec, low, pi -1); });
        //std::thread th2([&vec, &high, &pi]() {quickSortMT(vec, pi + 1, high); });
        //th1.join();
        //th2.join();
    }
}



class QuickSortMultiThreading {
public:
    QuickSortMultiThreading(int start, int end, std::vector<int>& arr, int limit, int depth = 0)
        : start_(start), end_(end), arr_(arr),limit(limit), depth(depth) {}

    int partition(int start, int end, std::vector<int>& arr) {
        int i = start;
        int j = end;

        // Decide random pivot
        int pivoted = rand() % (j - i + 1) + i;

        // Swap the pivoted with end
        // element of array
        int t = arr[j];
        arr[j] = arr[pivoted];
        arr[pivoted] = t;
        j--;

        // Start partitioning
        while (i <= j) {
            if (arr[i] <= arr[end]) {
                i++;
                continue;
            }
            if (arr[j] >= arr[end]) {
                j--;
                continue;
            }
            t = arr[j];
            arr[j] = arr[i];
            arr[i] = t;
            j--;
            i++;
        }

        // Swap pivoted to its
        // correct position
        t = arr[j + 1];
        arr[j + 1] = arr[end];
        arr[end] = t;
        return j + 1;
    }

    // Function to implement
    // QuickSort method
    void operator() () {
        // Base case
        //std::cout<<std::this_thread::get_id() << std::endl;;
        if (start_ >= end_) {
            return;
        }

        // Find partition
        int p = partition(start_, end_, arr_);

        // Divide array
        QuickSortMultiThreading left(start_, p - 1, arr_,limit, depth + 1);
        QuickSortMultiThreading right(p + 1, end_, arr_,limit, depth + 1);
        //left();
        //right();
        if (depth < limit)
        {
            std::thread t1(&QuickSortMultiThreading::operator(), left);
            std::thread t2(&QuickSortMultiThreading::operator(), right);
            t1.join();
            t2.join();
        }
        else {
            left();
            right();
        }
    }
    static int countSum(std::vector<int> arr) {
        int res = 0;
        for (auto el : arr) {
            res += el;
        }
        return res;
    }

private:
    int start_;
    int end_;
    int depth;
    int limit;
    std::vector<int>& arr_;
};






int main()
{
    MAIN_THREAD = std::this_thread::get_id();
    /*const auto start = std::chrono::high_resolution_clock::now();*/
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
    //int** a = new int* [3];
    //a[0] = new int[3] {-1, 2, -5};
    //a[1] = new int[3] {3, 4, 1};
    //a[2] = new int[3] {0, 1, 2};
    //
    //int** b = new int* [3];
    //b[0] = new int[3] {-1, 2, -5};
    //b[1] = new int[3] {3, 4, 1};
    //b[2] = new int[3] {0, 1, 2};
    ////Matrix mat3(mat1);

    //Matrix mat1(3,3, true);
    //Matrix mat2(3,3, true);
    //mat1.outMatr();
    //std::cout << std::endl;
    //mat2.outMatr();
    //std::cout << std::endl;

    //const auto startSync = std::chrono::high_resolution_clock::now();
    //double sumTime = startAndGetTimer(&Matrix::operator+, mat1, mat2);
    //std::cout << "Время суммирования " << sumTime<<" ms" << std::endl;
    //sumTime = startAndGetTimer(&Matrix::operator-, mat1, mat2);
    //std::cout << "Время вычитания " << sumTime << " ms" << std::endl;
    //sumTime = startAndGetTimer(&Matrix::operator*, mat1, mat2);
    //std::cout << "Время умножения " << sumTime << " ms" << std::endl;
    //const auto endSync = std::chrono::high_resolution_clock::now();
    //const std::chrono::duration<double, std::milli> elapsedSync = endSync - startSync;
    //std::cout << "Однопоточное выполнение операций заняло: " << elapsedSync.count() << "мс" << std::endl;


    //const auto startAsync = std::chrono::high_resolution_clock::now();
    //std::thread th1([&mat1, &mat2]() {(mat1 + mat2).outMatr(); });
    //std::thread th2([&mat1, &mat2]() {(mat1 - mat2).outMatr(); });
    //std::thread th3([&mat1, &mat2]() {(mat1 * mat2).outMatr(); });
    ////std::thread th4(&Matrix::operator+, std::ref(mat1), std::ref(mat2)); Другой способ создания потока
    //th1.join();
    //th2.join();
    //th3.join();
    //const auto endAsync = std::chrono::high_resolution_clock::now();
    //const std::chrono::duration<double, std::milli> elapsedAsync = endAsync - startAsync;
    //std::cout << "Многопоточное выполнение операций заняло: " << elapsedAsync.count() << "мс" << std::endl;
    /*std::thread* th1 = nullptr;
    std::thread* th2 = nullptr;
    std::thread* th3 = nullptr;*/
    /*startThread(&Matrix::operator+, mat1, mat2, th1); Вопрос со *, надо будет разобратся с более сложны ветвлением потоков
    startThread(&Matrix::operator-, mat1, mat2, th2);   так как вызов потока из функции, которая сама запускается в каком-либо потоке
    startThread(&Matrix::operator*, mat1, mat2, th3);   для решения скорее всего надо использовать более новые библиотеки*/
    /*th1->join();
    th2->join();
    th3->join();*/
    //delete th1;
    //delete th2;
    //delete th3;

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
   
    /*delete[] a[2],
    delete[] a[1],
    delete[] a[0],
    delete[] b[2],
    delete[] b[1],
    delete[] b[0];*/

    QuickSort q;
    int n = 100000;
    std::vector<int> mas;
    srand(1);
    for (int i = 0; i < n; i++) {
        mas.push_back(rand() % 100 + 1);
    }
    //q.outMas(mas);
    std::cout << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();
    QuickSortMultiThreading(0, mas.size() - 1, mas, 5);
    const auto end = std::chrono::high_resolution_clock::now();
    //q.outMas(mas);
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Выполнение программы заняло: " << elapsed.count() << "мс"<<std::endl;

    std::promise<int> threadPromise;
    std::future<int> threadFuture = threadPromise.get_future();

    std::thread t2([&threadPromise, &mas]() {
        threadPromise.set_value(QuickSortMultiThreading::countSum(std::ref(mas)));
        });

    std::cout<<"Сумма элементов массива, полученная с помощью промиса и фьючера из дополнительного потока: "<<threadFuture.get()<<std::endl;
    t2.join();

    //for (int limit = 0; limit <= 12; limit++)
    //{
    //    double rndTimeForLimit = 0;
    //    int limits = 0;
    //    for (int times = 0; times < 5; times++)
    //    {
    //        double rndTime = 0.0;
    //        int l = 0;
    //        for (int k = 0; k <= 100; k++)
    //        {
    //            int n = 100000;
    //            srand(time(NULL));
    //            std::vector<int> arr;
    //            for (int i = 0; i < n; i++) {
    //                arr.push_back(rand() % 100 + 1);
    //                //std::cout << arr[i] << ' ';
    //            }
    //            //std::cout << std::endl;
    //            const auto start = std::chrono::high_resolution_clock::now();
    //            QuickSortMultiThreading(0, arr.size() - 1, arr, limit)();
    //            const auto end = std::chrono::high_resolution_clock::now();
    //            for (int i = 0; i < n; i++) {
    //                // Print sorted elements
    //                //std::cout << arr[i] << " ";
    //            }
    //            //std::cout << std::endl;
    //            const std::chrono::duration<double, std::milli> elapsed = end - start;
    //            //std::cout << "Выполнение программы заняло: " << elapsed.count() << "мс";
    //            rndTime += elapsed.count();
    //            l++;
    //        }
    //        std::cout << "Среднее время выполнения для ограничения глубины " << limit << ", попытка номер "<<times+1<<" : " << rndTime / l << " мс\n";
    //        limits++;
    //        rndTimeForLimit += rndTime / l;
    //    }
    //    std::cout << "Среднее время выполнения для ограничения глубины " << limit << " : " << rndTimeForLimit / limits << std::endl;
    //}
    mas.clear();
    return 0;
}