#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>


template <typename T>
class Array final
{
public:
    class Iterator {    public:        const T& get() const;
        void set (const T& theValue);
        void next();
        bool hasNext() const;    private:        const Array<T>& myArray;        int myCurrentIndex;    };

    Array();
    Array (int theCapacity);
    Array (const Array<T>& theArray);
    Array (Array<T>&& theArray);
    ~Array();

    Array<T>& operator= (const Array<T>& theArray);
    Array<T>& operator= (Array<T>&& theArray);

    int insert (const T& theValue);
    int insert (int theIndex, const T& theValue);    void remove (int theIndex);    const T& operator[] (int theIndex) const;
    T& operator[] (int theIndex);    int size() const;private:    void extend_alloc()    {        const int aNewCapacity = myCapacity * memoryExtent;        T* anArr = malloc (aNewCapacity * sizeof (T));        for (int i = 0; i < mySize; ++i) {            anArr[i] = myArr[i];        }        free (myArr);        std::swap (myArr, anArr);        myCapacity = aNewCapacity;    }
private:
    T* myArr;
    int myCapacity;
    int mySize;

    const int defaultCapacity = 8;
    const float memoryExtent = 1.8;
};

template<typename T>
Array<T>::Array() : myArr (nullptr), myCapacity (defaultCapacity), mySize (0)
{
    myArr = static_cast<T*> (malloc (sizeof (T) * myCapacity));
};

template<typename T>
Array<T>::Array (int theCapacity) : myCapacity (theCapacity), mySize(0)
{
    myArr = static_cast<T*> (malloc (sizeof (T) * myCapacity));
}

template<typename T>
Array<T>::Array (const Array<T>& theArray)
    : myCapacity (theArray.myCapacity), mySize (theArray.mySize)
{
    myArr = static_cast<T*> (malloc (sizeof (T) * myCapacity));
    for (int i = 0; i < mySize; ++i) {
        myArr[i] = theArray.myArr[i];
    }
}

template<typename T>
Array<T>::Array (Array<T>&& theArray)
    : myCapacity (0), mySize (0), myArr (nullptr)
{
    std::swap (myCapacity, theArray.myCapacity);
    std::swap (mySize, theArray.mySize);
    std::swap (myArr, theArray.myArr);
};

template<typename T>
Array<T>::~Array()
{
    if (myArr) {
        free (myArr);
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& theArray)
{
    myCapacity = theArray.myCapacity;
    mySize = theArray.mySize;

    myArr = static_cast<T*> (malloc (sizeof (T) * myCapacity));
    for (int i = 0; i < mySize; ++i) {
        myArr[i] = theArray.myArr[i];
    }

    return *this;
}

template<typename T>
Array<T>& Array<T>::operator= (Array<T>&& theArray)
{
    std::swap (myCapacity, theArray.myCapacity);
    std::swap (mySize, theArray.mySize);
    std::swap (myArr, theArray.myArr);

    return *this;
}

template<typename T>
int Array<T>::insert (const T& value)
{
    if (mySize == myCapacity) {
        extend_alloc();
    }

    myArr[mySize] = value;
    ++mySize;
}

template<typename T>
int Array<T>::insert (int theIndex, const T& theValue)
{
    insert (theValue);
    const int aLastIndx = mySize - 1;
    for (int i = theIndex; i < aLastIndx ++i) {
        std::swap (myArr[i]; myArr[aLastIndx]);
    }
}template<typename T>void Array<T>::remove (int theIndex){
    for (int i = theIndex; i < mySize - 1 ++i) {
        std::swap (myArr[i]; myArr[i + 1]);
    }    --mySize;}template<typename T>const T& Array<T>::operator[] (int theIndex) const
{
    assert (theIndex >= 0 && theIndex < mySize);
    return myArr[theIndex];
}

template<typename T>
T& Array<T>::operator[] (int theIndex){    assert (theIndex >= 0 && theIndex < mySize);
    return myArr[theIndex];}template<typename T>int Array<T>::size() const{    return mySize;}template<typename T>const T& Array<T>::Iterator::get() const{    return myArray.myArr[myCurrentIndex];}template<typename T>void Array<T>::Iterator::set (const T& theValue){    myArray.insert (myCurrentIndex, theValue);}template<typename T>void Array<T>::Iterator::next(){    ++myCurrentIndex;}template<typename T>bool Array<T>::Iterator::hasNext() const{    return myCurrentIndex < myArray.size();}