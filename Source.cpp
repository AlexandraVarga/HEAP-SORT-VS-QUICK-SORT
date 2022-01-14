/*
    
    Tema 3 - Varga Alexandra-Ioana, grupa 30223

*/

#include "Profiler.h"
#include <iostream>
using namespace std;

Profiler p("Quick + Heap Sort");

void afisare(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/*

Din reprezentarea grafica se observa ca:

quickSort face mai putine asignari decat heapSort
quickSort face mai putine comparatii decat heapSort
=> quickSort per total  este mai eficient decat heapSort

-----------------------------------------------------------------------------------------------

Strategie Quick Sort:
Se alege un anumit element pe post de pivot, si se compara fiecare element cu acesta
Daca un element este mai mic sau egal cu pivotul il pozitionam in partea stanga altfel
il pozitionam in partea dreapta
Acesta operatie se numeste partitionare si se apeleaza recursiv pana cand vectorul va fi sortat
Este un algoritm de tip divide-et-impera care se bazeaza pe operatia de partitionare 
Nu este stabil

-----------------------------------------------------------------------------------------------
*/
int partition(int *a, int low, int high, int n, int x) 
{
    if (x == 0) 
    {
        Operation comparatii = p.createOperation("QUICKSORT_COMP", n);
        Operation asignari = p.createOperation("QUICKSORT_ASIG", n);
        Operation total = p.createOperation("QUICKSORT_TOTAL", n);
        
        asignari.count();
        total.count();
        
        int pivot = a[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            
            comparatii.count();
            total.count();
            
            if (a[j] < pivot) 
            {
                i++;
                int aux = a[i];
                a[i] = a[j];
                a[j] = aux;
                asignari.count(3);
                total.count(3);
            }
        }
        
        
        int aux = a[i + 1];
        a[i + 1] = a[high];
        a[high] = aux;

        asignari.count(3);
        total.count(3);
        
        return i + 1;
    }
    else if (x == 1) 
    {
        Operation comparatii = p.createOperation("QUICKSORT_COMP_WORST", n);
        Operation asignari = p.createOperation("QUICKSORT_ASIG_WORST", n);
        Operation total = p.createOperation("QUICKSORT_TOTAL_WORST", n);
        
        asignari.count();
        total.count();
        
        int pivot = a[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            
            comparatii.count();
            total.count();
            
            if (a[j] < pivot) {
                i++;
                int aux = a[i];
                a[i] = a[j];
                a[j] = aux;
                asignari.count(3);
                total.count(3);
            }
        }
        
        int aux = a[i + 1];
        a[i + 1] = a[high];
        a[high] = aux;

        asignari.count(3);
        total.count(3);
        
        return i + 1;
    }
}

/*

CAZURI QUICKSORT

AVERAGE CASE --> O(n*log n) ---- se fac O(log n) partitii O(n) 
Cazul mediu este mult mai apropiat de cazul favorabil decat de cel defavorabil

WORST CASE --> O(n^2). 
Pentru cazul in care partitia se face dupa ultimul element o intrare de date in care sirul este deja sortat
conduce la urmatoarea desfasurare: dupa fiecare partitionare avem o lista de n-1 elemente si o lista de 0 elemente

BEST CASE --> O(n*log n)
Cazul acesta se obtine cand pivotul imparte vectorul in 2 parti egale, adica fiind de fiecare data
elementul din mijlocul vectorului

*/

void quickSort(int *a, int low, int high, int n, int x) 
{
    if (low < high) 
    {
        int pivot = partition(a, low, high, n, x);
        quickSort(a, low, pivot - 1, n, x);
        quickSort(a, pivot + 1, high, n, x);
    }
}

/*

CAZURI QUICKSELECT

AVERAGE CASE --> O(n). Spre deosebire de quickSort in loc sa apeleze recursiv 2 metode, algoritmul apeleaza doar partea
in care se afla elementul cautat si reduce astfel complexitatea de la ----  O(n*log n)
WORST CASE --> O(n^2).
BEST CASE --> O(n).

*/

int quickSelect(int *a, int low, int high, int k)
{

    if (k > 0 && k <= high - low + 1)
    {
        int pivot = partition(a, low, high, 0, 0);
        if (pivot - low == k - 1)
            return a[pivot];
        if (pivot - low > k - 1)
            return quickSelect(a, low, pivot - 1, k);
        if (pivot - low < k - 1)
            return quickSelect(a, pivot + 1, high, k - pivot + low - 1);
    }
    else
        return -1;
}

/*

O(lg n)
Fiecare apel al lui heapify() se face in O(lg n) si se fac O(n) astfel de apeluri => construct_heap_bottom_up()
se executa in O(n*lg n) , iar cazul defavorabil se intalneste atunci cand numerele sunt descrescatoare in sir

*/

void heapify(int *sir, int size_sir, int i, int n) 
{
    Operation comparatii = p.createOperation("HEAPSORT_COMP", n);
    Operation asignari = p.createOperation("HEAPSORT_ASIG", n);
    Operation total = p.createOperation("HEAPSORT_TOTAL", n);

    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    comparatii.count();
    total.count();

    if (l < size_sir && sir[l] < sir[smallest])
        smallest = l;
    comparatii.count();
    total.count();
    if (r < size_sir && sir[r] < sir[smallest])
        smallest = r;

    if (smallest != i)
    {
        int aux = sir[i];
        sir[i] = sir[smallest];
        sir[smallest] = aux;
        asignari.count(3);
        total.count(3);
        
        heapify(sir, size_sir, smallest, n);
    }
}

/*
------------------------------------------------------------------------------------------------------

Strategie Heap Sort:
Algoritmul priveste tabloul ce dorim sa-l sortam ca pe un arbore binar: primul element este radacina
al doilea element este descendent al primului element etc.
Proprietati ce trebuie sa le verifice arborele binar creat:
-diferenta maxima dintre 2 frunze este 1 (toate frunzele se afla pe ultimul sau penultimul nivel)
-frunzele care au adancime maxima sunt puse in stanga
-fiecare nod are o valoare mai mare decat cei 2 copii ai sai (sortare crescatoare)
Nu este stabil

-------------------------------------------------------------------------------------------------------

Fiecare apel al lui heapify() se face in O(lg n) si se fac n-1 astfel de apeluri => heapSort()
se executa in O(n*lg n)

*/

void heapSort(int *sir, int size_sir, int n) 
{
    Operation comparatii = p.createOperation("HEAPSORT_COMP", n);
    Operation asignari = p.createOperation("HEAPSORT_ASIG", n);
    Operation total = p.createOperation("HEAPSORT_TOTAL", n);

    for (int i = size_sir / 2 - 1; i >= 0; i--) 
    {
        heapify(sir, size_sir, i, n);
    }
    
    int schimb[10000];
    
    for (int i = size_sir - 1; i >= 0; i--)
    {
        int aux = sir[0];
        sir[0] = sir[i];
        sir[i] = aux;
        
        asignari.count(3);
        total.count(3);
        heapify(sir, i, 0, n);
    }
    
    for (int i = 0; i < size_sir; i++) 
    {
        asignari.count();
        total.count();
        schimb[i] = sir[size_sir - 1 - i];
    }
    
    for (int i = 0; i < size_sir; i++) 
    {
        asignari.count();
        total.count();
        sir[i] = schimb[i];
    }
}

void demo()
{
    int b[1000];
    int size = 10;
    
    FillRandomArray(b, size, 0, 10, 0);
    //sir initial
    afisare(b, size);
    
    quickSort(b, 0, size, 0, 0);
    
    //sir sortat
    afisare(b, size);

}

void perf()
{
    int a[10000];
    for (int n = 100; n <= 10000; n += 100) 
    {
        for (int k = 0; k < 5; k++) 
        {
            FillRandomArray(a, n);
            heapSort(a, n, n);
        }
    }
    for (int n = 100; n <= 1000; n += 100) 
    {
        for (int k = 0; k < 5; k++) 
        {
            FillRandomArray(a, n);
            quickSort(a, 0, n - 1, n, 0);
        }
    }
    p.divideValues("COMPARATII_Q", 5);
    p.divideValues("COMPARATII_H", 5);
    p.divideValues("ASIGNARI_Q", 5);
    p.divideValues("ASIGNARI_H", 5);

    p.createGroup("1. ASIG AVERAGE CASE", "QUICKSORT_ASIG", "HEAPSORT_ASIG");
    p.createGroup("2. COMP AVERAGE CASE", "QUICKSORT_COMP", "HEAPSORT_COMP");
    p.createGroup("3. TOTAL CAZ MEDIU", "QUICKSORT_TOTAL", "HEAPSORT_TOTAL");

    for (int n = 100; n <= 10000; n += 100) 
    {
        for (int k = 0; k < 5; k++) 
        {
            FillRandomArray(a, n, -1000, 1000, false, 1);
            quickSort(a, 0, n - 1, n, 1);
        }
    }
    p.createGroup("4. QUICKSORT WORST CASE", "QUICKSORT_ASIG_WORST", "QUICKSORT_COMP_WORST", "QUICKSORT_TOTAL_WORST");
    p.showReport();

}
int main()
{
    //demo();
    //perf();
    return 0;
}
