//============================================================================
// Name        : callback.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <vector>
#include <functional>
#include <atomic>
#include <thread>
#include <algorithm>

using namespace std;

//1. function template
template <typename T>
T myMax(T x, T y) {
	return (x>y) ? x : y;
}

//2. class template
template <typename T>
class Array{
private:
	T *ptr;
	int size;
public:
	Array(T arr[], int s);
	void print();
};

template <typename T>
Array<T>::Array(T arr[], int s) {
	ptr=new T[s];
	size=s;
	for(int i=0; i<size; i++)
		ptr[i]=arr[i];
}

template <typename T>
void Array<T>::print() {
	for (int i = 0; i < size; i++)
		cout<<" "<<*(ptr + i);
	cout<<endl;
}

//3. more than one arguments
template<class T, class U>
class A  {
    T x;
    U y;
public:
    A() {    cout<<"Constructor Called"<<endl;   }
};

//4. specify default value for template arguments
template<class T, class U = char>
class AD  {
public:
    T x;
    U y;
    AD() {   cout<<"Constructor Called"<<endl;   }
};

//5. pass nontype parameters to templates, they must be const!!!!!!!!!!!
//assign const a value
template <class T, int max>
int arrMin(T arr[], int n)
{
   int m = max;
   for (int i = 0; i < n; i++)
      if (arr[i] < m)
         m = arr[i];

   return m;
}

//6.Variadic function templates
// To handle base case of below recursive
// Variadic function Template
void print()
{
    cout << "I am empty function and "
            "I am called at last.\n" ;
}

// Variadic function Template that takes
// variable number of arguments and prints
// all of them.
template <typename T, typename... Types>
void print(T var1, Types... var2)
{
    cout << var1 << endl ;

    print(var2...) ;
}


/*
template<typename _Res, typename... _ArgTypes>
    class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
      private _Function_base
    {
	}
*/

/*!
 * @class C++ is lack of unregister callback
 *
 * @brief
 */
#if 0

template <typename SEQUENCE, typename _Rp, typename _ArgTypes>
class CallbackContainer {
public:
    /*!
     * @brief return the sequenceID
     */
    int registerCallback(std::function< _Rp(_ArgTypes) > cb);	//error: invalid declarator before ‘cb’???
    std::function< _Rp(_ArgTypes) > find(int sequenceID);
    bool unregister(int sequenceID);

public:
    std::vector< std::function< _Rp(_ArgTypes) > > m_callbackList;
};

#endif

#if 0
template <typename SEQUENCE, typename _Rp, typename... _ArgTypes>
class CallbackContainer {
public:
    /*!
     * @brief return the sequenceID
     */
	int registerCallback(std::function< _Rp(_ArgTypes...) > cb) {
		m_callbackList.push_back(cb);
		return m_callbackList.size();
	}

    std::function< _Rp(_ArgTypes...) > find(SEQUENCE sequenceID) {

    }

    bool unregister(SEQUENCE sequenceID);

public:
    std::vector< std::function< _Rp(_ArgTypes...) > > m_callbackList;
    SEQUENCE m_ID=0;
};

#endif

#if 1
/*!
 * @class Add unregister callback which is lack in C++
 *
 * @brief Add unregister callback support by sequence ID
 *
 * @details this class is not thread safe. The caller should add locker before call member functions when in multiple threads environment
 * FUNCTION_T is something like: void(); int(int, char)...
 *
 */
template <typename FUNCTION_T>
class CallbackContainer {
public:
    struct SUBTASK {
        SUBTASK() = delete;
        SUBTASK(int seq, std::function< FUNCTION_T > cb) : sequenceID(seq), callback(cb) {
        }

        int sequenceID;
        std::function< FUNCTION_T > callback;
    };

public:
    /*!
     * @brief sequenceID is in the circle of [0, 1, ..., limit-1, 0, ...]
     *
     */
    CallbackContainer(int limit=32) : m_limit(limit){
    }

    /*!
     * @brief return the sequenceID
     */
    int registerCallback(std::function< FUNCTION_T > cb) {  //register is a keyword
        if(cb == nullptr) {
            return -1;
        }

        //Overwrite duplicate
        auto it = std::find_if(
                m_callbackList.begin(),
                m_callbackList.end(),
                [this](const SUBTASK &item) -> bool { return item.sequenceID==m_ID; } ) ;

        if (it != m_callbackList.end() ) {
            it->callback = cb;
            int ret = m_ID;
            m_ID = (m_ID+1) % m_limit;  //The caller should lock() before
            return ret;
        }

        SUBTASK task(m_ID, cb);
        m_callbackList.push_back(task);
        m_ID = (m_ID+1) % m_limit;  //The caller should lock() before
        return task.sequenceID;
    }

    bool find(int sequenceID, const SUBTASK* &pFind) const {
        auto it = std::find_if(
                m_callbackList.cbegin(),
                m_callbackList.cend(),
                [sequenceID](const SUBTASK &item) -> bool { return item.sequenceID==sequenceID; } ) ;

        if (it == m_callbackList.cend() ) {
            pFind = nullptr;
            return false;
        }
        else {
            pFind = &*it;
            return true;
        }
    }

    bool unregister(int sequenceID) {
        auto it = std::find_if(
                m_callbackList.cbegin(),
                m_callbackList.cend(),
                [sequenceID](const SUBTASK &item) -> bool { return item.sequenceID==sequenceID; } ) ;

        if (it != m_callbackList.cend() ) {
            m_callbackList.erase(it);
            return true;
        }
        return false;
    }

    /*!
     * @brief if notification should trigger all callbacks
     */
    const std::vector< SUBTASK >& getAll() const {
        return m_callbackList;
    }

private:
    const int m_limit;
    int m_ID=0;
    std::vector< SUBTASK > m_callbackList;
};

#endif

int main() {

	auto f1=[](int x, int y) int{
		return x+y;
	};
	CallbackContainer< int(int, int) > cc1(16);
	int i = cc1.registerCallback(f1);

	const CallbackContainer< int(int, int) >::SUBTASK* pFind;
	cc1.find(i, pFind);
	cout << "1=" << pFind->callback(2,3) << endl;
	cc1.unregister(i);

	auto f2=[](double x) double{
		return x*2;
	};
	CallbackContainer< double(double) > cc2;
	int j = cc2.registerCallback(f2);
	cout << "j=" <<j <<endl;

#if 0
	print(1, 2, 3.14, "Pass me any "
	              "number of arguments",
	                  "I will print\n");
/*
1
2
3.14
Pass me any number of arguments
I will print

I am empty function and I am called at last.
*/
#endif



#if 0	//1
	cout << myMax<int>(3,7) <<endl;
	cout << myMax(3,7) <<endl;		//<int> ingone

	cout << myMax<char>('g','e') <<endl;
	cout << myMax('g','e') <<endl;
#endif

#if 0	//2
	int arr[5] = {1, 2, 3, 4, 5};
	Array<int> a(arr, 5);
	//Array a(arr, 5);	//error: missing template arguments before ‘a’
	a.print();
#endif

#if 0	//3
	A<char, char> a;
	A<int, double> b;
#endif

#if 0	//4
	AD<char> a;
#endif

#if 0	//5
	int arr1[]  = {10, 20, 15, 12};
	int n1 = sizeof(arr1)/sizeof(arr1[0]);

	// Second template parameter to arrMin must be a constant
	cout << arrMin<int, 10000>(arr1, n1) << endl;
#endif

	return 0;
}
