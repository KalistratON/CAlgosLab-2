#ifndef _ARRAY_
#define _ARRAY_

#include <cassert>
#include <cstdlib>
#include <memory>
#include <type_traits>
#include <stdexcept>


namespace kll {

template <typename T>
class Array final
{
private:
    const int defaultCapacity = 8;
    const double memoryExtent = 1.8;

public:

    template <bool IsDirect>
    class Iterator_ {
    public:
        Iterator_ (Array<T>& theArray);

        const T& get() const;
        void set (const T& theValue);
        void next();
        bool hasNext() const;

    private:
        Array<T>& myArray;
        int myCurrentIndex;
        bool myIsDirect;
    };

    template <bool IsDirect>
    class ConstIterator_ {
    public:
        ConstIterator_ (const Array<T>& theArray);

        const T& get() const;
        void next();
        bool hasNext() const;

    private:
        const Array<T>& myArray;
        int myCurrentIndex;
        bool myIsDirect;
    };

    using Iterator              = Iterator_<true>;
    using ConstIterator         = ConstIterator_<true>;
    using ReverseIterator       = Iterator_<false>;
    using ConstReverseIterator  = ConstIterator_<false>;


    Array();
    explicit Array (int theCapacity);
    Array (const Array<T>& theArray);
    Array (Array<T>&& theArray) noexcept;
    ~Array();

    const Array<T>& operator= (const Array<T>& theArray);
    const Array<T>& operator= (Array<T>&& theArray) noexcept;

    int insert (const T& theValue);
    int insert (int theIndex, const T& theValue);

    void remove (int theIndex);

    const T& operator[] (int theIndex) const;
    T& operator[] (int theIndex);

    int size() const;

    Iterator iterator() { return Iterator (*this); };
    ConstIterator iterator() const { return ConstIterator (*this); };

    ReverseIterator reverseIterator() { return ReverseIterator (*this); };
    ConstReverseIterator reverseIterator() const { return ConstReverseIterator (*this); };

private:

    void swap_ (T& theLhs, T& theRhs)
    {
        T aTmp = std::move_if_noexcept (theLhs);
        theLhs = std::move_if_noexcept (theRhs);
        theRhs = std::move_if_noexcept (aTmp);
    }

    void extend_alloc_()
    {
        const int aNewCapacity = static_cast<int> (myCapacity * memoryExtent);
        T* anArr = (T*) malloc (aNewCapacity * sizeof (T));
        for (int i = 0; i < mySize; ++i) {
            std::construct_at (anArr + i, myArr[i]);
        }
        free (myArr);
        std::swap (myArr, anArr);
        myCapacity = aNewCapacity;
    }

private:
    T* myArr;
    int myCapacity;
    int mySize;
};

template<typename T>
Array<T>::Array() : myArr (nullptr), myCapacity (defaultCapacity), mySize (0)
{
    myArr = (T*) (malloc (sizeof (T) * myCapacity));
};

template<typename T>
Array<T>::Array (int theCapacity) : myCapacity (theCapacity), mySize(0)
{
    myArr = (T*) (malloc (sizeof (T) * myCapacity));
}

template<typename T>
Array<T>::Array (const Array<T>& theArray)
    : myCapacity (theArray.myCapacity), mySize (theArray.mySize)
{
    myArr = (T*) (malloc (sizeof (T) * myCapacity));
    for (int i = 0; i < mySize; ++i) {
        std::construct_at (myArr + i, theArray.myArr[i]);
    }
}

template<typename T>
Array<T>::Array (Array<T>&& theArray) noexcept
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
        for (size_t i = 0; i < mySize; ++i) {
            myArr[i].~T();
        }
        free (myArr);
    }
}

template<typename T>
const Array<T>& Array<T>::operator= (const Array<T>& theArray)
{
    Array<T> aTmpArr (theArray);
    std::swap (*this, aTmpArr);
    return *this;
}

template<typename T>
const Array<T>& Array<T>::operator= (Array<T>&& theArray) noexcept
{
    std::swap (*this, theArray);
    return *this;
}

template<typename T>
int Array<T>::insert (const T& theValue)
{
    if (mySize == myCapacity) {
        extend_alloc_();
    }

    std::construct_at (myArr + mySize, theValue);
    ++mySize;
    return mySize - 1;
}

template<typename T>
int Array<T>::insert (int theIndex, const T& theValue)
{
    insert (theValue);
    const int aLastIndx = mySize - 1;
    for (int i = theIndex; i < aLastIndx; ++i) {
        swap_ (myArr[i], myArr[aLastIndx]);
    }
    return theIndex;
}

template<typename T>
void Array<T>::remove (int theIndex)
{
    for (int i = theIndex; i < mySize - 1; ++i) {
        swap_ (myArr[i], myArr[i + 1]);
    }
    --mySize;
}

template<typename T>
const T& Array<T>::operator[] (int theIndex) const
{
    assert (theIndex >= 0 && theIndex < mySize);
    return myArr[theIndex];
}

template<typename T>
T& Array<T>::operator[] (int theIndex)
{
    assert (theIndex >= 0 && theIndex < mySize);
    return myArr[theIndex];
}

template<typename T>
int Array<T>::size() const
{
    return mySize;
}

template<typename T>
template<bool IsDirect>
Array<T>::Iterator_<IsDirect>::Iterator_ (Array<T>& theArray)
    : myArray (theArray), myCurrentIndex (0)
{
    if (!IsDirect) {
        myCurrentIndex = myArray.size() - 1;
    }
}

template<typename T>
template<bool IsDirect>
const T& Array<T>::Iterator_<IsDirect>::get() const
{
    return myArray.myArr[myCurrentIndex];
}

template<typename T>
template<bool IsDirect>
void Array<T>::Iterator_<IsDirect>::set (const T& theValue)
{
    myArray[myCurrentIndex] = theValue;
}

template<typename T>
template<bool IsDirect>
void Array<T>::Iterator_<IsDirect>::next()
{
    IsDirect ? ++myCurrentIndex : --myCurrentIndex;
}

template<typename T>
template<bool IsDirect>
bool Array<T>::Iterator_<IsDirect>::hasNext() const
{
    return IsDirect ? myCurrentIndex < myArray.size() : myCurrentIndex >= 0;
}

template<typename T>
template<bool IsDirect>
Array<T>::ConstIterator_<IsDirect>::ConstIterator_ (const Array<T>& theArray)
    : myArray (theArray), myCurrentIndex (0)
{
    if (!IsDirect) {
        myCurrentIndex = myArray.size() - 1;
    }
}

template<typename T>
template<bool IsDirect>
const T& Array<T>::ConstIterator_<IsDirect>::get() const
{
    return myArray.myArr[myCurrentIndex];
}

template<typename T>
template<bool IsDirect>
void Array<T>::ConstIterator_<IsDirect>::next()
{
    IsDirect ? ++myCurrentIndex : --myCurrentIndex;
}

template<typename T>
template<bool IsDirect>
bool Array<T>::ConstIterator_<IsDirect>::hasNext() const
{
    return IsDirect ? myCurrentIndex < myArray.size() : myCurrentIndex >= 0;
}

};

#endif // _ARRAY_