enum Color
{
    BLACK,
    WHITE
};
#include <iostream>
#include <fstream>
#include <istream>
#include <conio.h>
#include <list>
#include <limits>
#include <vector>
using namespace std;
string colores[]={"gray38","aliceblue"};

template <class T>
class NodoF
{
public:
    int m_Grado;
    T m_Dato;
    NodoF<T> *m_Padre;
    list<NodoF<T> *> m_Sons;
    bool m_Color;

    NodoF(T d)
    {
        m_Dato = d;
        m_Padre = 0;
        m_Color = WHITE;
        m_Grado = 0;
        m_Sons.clear();
    }
};

template <class T>
class Fibonaci_Heap
{
public:
    list<NodoF<T> *> m_Roots;
    NodoF<T> *m_pMin;

public:
    void Compactar(); // O(log(n))
    void PrintArbol(NodoF<T> *, ofstream &archivo);

    NodoF<T> *Unir(NodoF<T> *p, NodoF<T> *q); // O(1)
    void fuuuuusioon(NodoF<T> *arreglo[], NodoF<T> *aux)
    {
        if (arreglo[0] == NULL)
        {
            arreglo[0] = aux;
            *(arreglo - 1) = NULL;
            return;
        }
        NodoF<T> *algo = Unir(arreglo[0], aux);
        fuuuuusioon(arreglo + 1, algo);
        (*(arreglo - 1)) = NULL;
    }

public:
    Fibonaci_Heap()
    {
        m_pMin = 0;
    };
    ~Fibonaci_Heap(){};
    /**************************************/
    void Insert(T d);                      // O(1))
    void Extract_Min();                    // O(log(n))
    void Delete(NodoF<T> *e);              // O(log(n))
    void Decrease_Key(NodoF<T> *e, T val); // O(1)
    NodoF<T> *GetMin();                    // O(1)
    void PrintRaices(typename list<NodoF<T> *>::iterator it, typename list<NodoF<T> *>::iterator end, ofstream &archivo);
    NodoF<T> *mergeFibonacciTrees(NodoF<T> *b1, NodoF<T> *b2);

    void Show_Dot(string filename);
    /**************************************/
};

template <class T>
void Fibonaci_Heap<T>::Insert(T d) // O(1)
{
    NodoF<T> *pNew = new NodoF<T>(d);
    // m_Roots.push_front(pNew);

    if (m_Roots.empty())
    {
        m_Roots.push_front(pNew);
        m_pMin = pNew;
        return;
    }
    m_Roots.push_front(pNew);
    if (d < m_pMin->m_Dato)
    {
        m_pMin = pNew;
    }
}

template <class T>
void Fibonaci_Heap<T>::Extract_Min() // O(log(n))
{
    if (!this->m_pMin)
    {
        return;
    }
    if (m_pMin->m_Sons.empty())
    {
        m_Roots.remove(m_pMin);
        delete this->m_pMin;
        m_pMin = 0;

        typename list<NodoF<T> *>::iterator it2 = m_Roots.begin();
        m_pMin = (*it2);
        for (; it2 != m_Roots.end(); it2++)
        {
            if ((*(it2))->m_Dato < m_pMin->m_Dato)
            {
                m_pMin = (*it2);
            }
        }
        Compactar();
        return;
    }
    else
    {
        typename list<NodoF<T> *>::iterator it = m_pMin->m_Sons.end();
        it--;
        for (; it != (m_pMin->m_Sons).begin(); it--)
        {
            (*it)->m_Padre = NULL;
            (*it)->m_Color = WHITE;
            m_Roots.push_front(*it);
        }
        m_Roots.push_front(*it);
        m_Roots.remove(m_pMin);
        delete this->m_pMin;

        typename list<NodoF<T> *>::iterator it2 = m_Roots.begin();
        m_pMin = (*it2);
        for (; it2 != m_Roots.end(); it2++)
        {
            if ((*(it2))->m_Dato < m_pMin->m_Dato)
            {
                m_pMin = (*it2);
            }
        }
        Compactar();
        return;
    }
}

template <class T>
void Fibonaci_Heap<T>::Decrease_Key(NodoF<T> *e, T val) // O(1)
{
    
    if(e){
        NodoF<T>* n, *p;
        n = e;
        n->m_Dato = val;
        if(!n->m_Padre)return;
        if(n->m_Dato > n->m_Padre->m_Dato) return;
        
        do{
            p = n->m_Padre;
            p->m_Sons.remove(n);
            n->m_Color = WHITE;
            m_Roots.push_front(n);
            if(n->m_Dato < m_pMin->m_Dato){
                m_pMin = n;
            }
            n = p;
        }while(((p)&&(p->m_Color != WHITE)));
        if(p->m_Padre) p->m_Color = BLACK;
    }
}

template <typename T>
NodoF<T> *Fibonaci_Heap<T>::mergeFibonacciTrees(NodoF<T> *b1, NodoF<T> *b2)
{
    if (b1->m_Dato > b2->m_Dato)
        swap(b1, b2);
    b2->m_Padre = b1;
    b1->m_Sons.push_front(b2);
    b1->m_Grado++;
    return b1;
}

template <typename T>
void Fibonaci_Heap<T>::PrintArbol(NodoF<T> *nodo, ofstream &archivo)
{
    if (!nodo)
        return;
    typename list<NodoF<T> *>::iterator it = nodo->m_Sons.begin();
    for (; it != nodo->m_Sons.end(); it++)
    {
        if ((*it)->m_Color)//color es 1
        {
            archivo << (*it)->m_Dato << "[style=\"filled\"color= "<<colores[(*it)->m_Color]<<" ] " << endl;
        }
        else//el color es 0
        {
            archivo << (*it)->m_Dato << "[style=\"filled\"color= "<<colores[(*it)->m_Color] <<" ] " << endl;
        }
        archivo << nodo->m_Dato << " -> " << (*it)->m_Dato << ";" << endl;
        PrintArbol((*it), archivo);
    }
}
template <class T>
void Fibonaci_Heap<T>::Delete(NodoF<T> *e) // O(log(n))
{
    Decrease_Key(e, INT_MIN);
    Extract_Min();
}

template <class T>
NodoF<T> *Fibonaci_Heap<T>::GetMin() // O(1)
{
    cout << "Dato minimo: " << m_pMin->m_Dato << "\n";
    return m_pMin;
}


template <class T>
void Fibonaci_Heap<T>::Compactar() // O(log(n))
{
    if (m_Roots.size() < 1)
        return;
    if (m_Roots.size() == 1)
    {
        m_pMin = *(m_Roots.begin());
        return;
    }
    vector<NodoF<T> *> grados(30, nullptr);
    for (auto it = m_Roots.begin(); it != m_Roots.end(); it++)
    {
        if (grados[(*it)->m_Grado] == nullptr)
        {
            grados[(*it)->m_Grado] = *it;
            continue;
        }
        else
        {
            while (grados[(*it)->m_Grado] != nullptr)
            {
                NodoF<T> *aux = grados[(*it)->m_Grado];
                grados[(*it)->m_Grado] = nullptr;
                NodoF<T> *union_ = Unir(*it, aux);
                m_Roots.remove(aux);
                *it = union_;
            }
            grados[(*it)->m_Grado] = *it;
        }
    }

    auto it_min = m_Roots.begin();
    m_pMin = *it_min;
    for (; it_min != m_Roots.end(); it_min++)
    {
        if ((*it_min)->m_Dato < m_pMin->m_Dato)
            m_pMin = *it_min;
    }
}

// O(1)
template <class T>
NodoF<T> *Fibonaci_Heap<T>::Unir(NodoF<T> *p, NodoF<T> *q) // O(1)
{
    if (p->m_Dato < q->m_Dato)
    {
        p->m_Grado += 1;
        p->m_Sons.push_front(q);
        q->m_Padre = p;
        m_Roots.remove(q);
        // q = NULL;
        return p;
    }
    q->m_Grado += 1;
    q->m_Sons.push_front(p);
    // p = NULL;
    p->m_Padre = q;
    m_Roots.remove(p);
    return q;
}

template <class T>
void Fibonaci_Heap<T>::PrintRaices(typename list<NodoF<T> *>::iterator it, typename list<NodoF<T> *>::iterator end, ofstream &archivo)
{
    if (it == end)
    {
        return;
    }
    archivo << "subgraph " << (*it)->m_Dato << "{ label = " << (*it)->m_Dato << " ;" << endl;
    // cout << (*it)->m_Dato;
    archivo << (*it)->m_Dato << "[label= " << (*it)->m_Dato << "];" << endl;
    archivo << (*it)->m_Dato << "[label= " << (*it)->m_Dato <<" style= filled color= "<<colores[(*it)->m_Color] << "];" << endl;
    PrintArbol(*it, archivo);
    if (((next(it, 1))) != end)
    {
        archivo << "{rank=same; " << (*it)->m_Dato << "; " << (*(next(it, 1)))->m_Dato << "}";
        archivo << (*it)->m_Dato << " -> " << (*(next(it, 1)))->m_Dato << endl;
    }
    archivo << "}" << endl;
    PrintRaices((++it), end, archivo);
}

template <class T>
void Fibonaci_Heap<T>::Show_Dot(string filename)
{
    ofstream archivo;
    archivo.open(filename.c_str(), ios::out);
    archivo << "digraph FibonacciHeap {" << endl;
    archivo << "label= \"Fibonacci Heap\";" << endl;
    // archivo << "node [shape = record];" << endl;
    typename list<NodoF<T> *>::iterator it = m_Roots.begin();
    PrintRaices(it, m_Roots.end(), archivo);
    archivo << "}" << endl;
    // system("dot -Tjpg -O Fibonacci.dot");        //Genera la Imagen
    // system("Fibonacci.dot.jpg");                 //Muestra la Imagen(en Windows)
    // system("open Fibonacci.dot.jpg");            //Muestra la Imagen(en Linux)
}

int main()
{
    Fibonaci_Heap<int> test;

    // test.Insert(1);
    // test.Insert(2);
    for (int i = 0; i < 65; i++)
    {
        test.Insert(i + 1);
    }
    // test.Extract_Min(); //A travez del Extract_Min() se forma los arboles de nuestro FB Heap por la función Compactar()


    // NodoF<int> *n = test.GetMin();
    // NodoF<int> *son = *(n->m_Sons.begin());
    // NodoF<int> *son_2 = *(son->m_Sons.begin());
    // // test.Delete(son_2);
    // test.Decrease_Key(son_2, 1);

    test.Extract_Min();
    test.Show_Dot("Fibonacci.dot");
    return 1;
}
