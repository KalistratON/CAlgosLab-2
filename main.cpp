#include "Array.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <vector>
#include <type_traits>


using namespace kll;

namespace {

struct DumbClass {

    DumbClass() : myInt (0) {};
    DumbClass (int theInt) : myInt (theInt) {};

    DumbClass (DumbClass&&) = delete;
    DumbClass& operator= (DumbClass&&) = delete;

    DumbClass (const DumbClass&) = default;
    DumbClass& operator= (const DumbClass&) = default;

    bool operator== (const DumbClass& theDC2) const
    {
        return myInt == theDC2.myInt;
    }

private:
    int myInt;
};

template<typename T>
void DoConstruct()
{
    Array<T> anArr1;
    Array<T> anArr2 (10);
    Array<T> anArr3 (anArr1);
    Array<T> anArr4 (std::move (anArr2));

    EXPECT_TRUE (anArr1.size() == 0);
    EXPECT_TRUE (anArr2.size() == 0);
    EXPECT_TRUE (anArr3.size() == 0);
    EXPECT_TRUE (anArr4.size() == 0);
}

template<typename T>
Array<T> GetArray (const std::vector<T>& theVec)
{
    Array<T> anArr;
    for (const auto& anElem : theVec) {
        anArr.insert (anElem);
    }
    return anArr;
}

template<typename T>
void DoInsert (const std::vector<T>& theVec)
{
    Array<T> anArr = GetArray<T> (theVec);
    EXPECT_TRUE (anArr.size() == theVec.size());
    for (int i = 0; i < anArr.size(); ++i) {
        EXPECT_TRUE (anArr[i] == theVec[i]);
    }
}

template<typename T>
void DoAssigment (const std::vector<T>& theVec)
{
    auto anInArr = GetArray (theVec);
    auto anOutArr = anInArr;

    EXPECT_TRUE (anInArr.size() == theVec.size());
    EXPECT_TRUE (anOutArr.size() == anInArr.size());

    for (int i = 0; i < anInArr.size(); ++i) {
        EXPECT_TRUE(anInArr[i] == theVec[i]);
        EXPECT_TRUE(anOutArr[i] == anInArr[i]);
    }

    auto anMoveOutArr = std::move (anOutArr);
    EXPECT_TRUE (anOutArr.size() == 0);
    EXPECT_TRUE (anMoveOutArr.size() == theVec.size());

    for (int i = 0; i < anMoveOutArr.size(); ++i) {
        EXPECT_TRUE(anMoveOutArr[i] == anInArr[i]);
    }
}

template<typename T>
void DoRemove (const std::vector<T>& theVec)
{
    auto anArr = GetArray (theVec);
    EXPECT_TRUE (anArr.size() == theVec.size());

    const int aToRemoveSize = anArr.size() / 2;
    for (int i = 0; i < aToRemoveSize; ++i) {
        anArr.remove (0);
        EXPECT_TRUE (anArr[0] == theVec[i + 1]);
    }

    EXPECT_TRUE (anArr.size() == theVec.size() - aToRemoveSize);

    int anArrSize = anArr.size();
    for (int i = 0; i < anArrSize; ++i) {
        anArr.remove (0);

        if (i != anArrSize - 1) {
            EXPECT_TRUE (anArr[0] == theVec[aToRemoveSize + i + 1]);
        }
    }

    EXPECT_TRUE (anArr.size() == 0);
}

template<typename T>
void DoIterator (const std::vector<T>& theVec)
{
    Array<T> anArr = GetArray (theVec);

    {
        size_t i = 0;
        auto anIt = anArr.iterator();
        while (anIt.hasNext()) {
            EXPECT_TRUE (anIt.get() == theVec[i]);
            anIt.next();
            ++i;
        }
        EXPECT_TRUE (anArr.size() == i);
    }

    {
        auto anIt = anArr.iterator();
        for (auto aVecIt = theVec.rbegin(); aVecIt != theVec.rend(); ++aVecIt) {
            anIt.set (*aVecIt);
            anIt.next();
        }
    }

    {
        auto anIt = anArr.reverseIterator();
        for (auto aVecIt = theVec.begin(); aVecIt != theVec.end(); ++aVecIt) {
            EXPECT_TRUE (anIt.get() == *aVecIt);
            anIt.next();
        }
    }
}

}

TEST(ARRAY, CONSTRUCTOR)
{
    DoConstruct<int>();
    DoConstruct<std::string>();
    DoConstruct<std::vector<int>>();
    DoConstruct<DumbClass>();
}

TEST(ARRAY, INSERT)
{
    DoInsert<int> (std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8});
    DoInsert<std::string> (std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8"});
    DoInsert<std::vector<int>> (std::vector<std::vector<int>> {{1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    DoInsert<DumbClass> (std::vector<DumbClass> { DumbClass (1), DumbClass (2), DumbClass(3), DumbClass (4) });
}

TEST(ARRAY, ASSIGMENT_OPERATOR)
{
    DoAssigment<int>(std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8});
    DoAssigment<std::string>(std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8"});
    DoAssigment<std::vector<int>>(std::vector<std::vector<int>> {{1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    DoAssigment<DumbClass>(std::vector<DumbClass> { DumbClass (1), DumbClass (2), DumbClass(3), DumbClass (4) });
}

TEST(ARRAY, REMOVE)
{
    DoRemove<int> (std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8});
    DoRemove<std::string> (std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8"});
    DoRemove<std::vector<int>> (std::vector<std::vector<int>> {{1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    DoRemove<DumbClass> (std::vector<DumbClass> { DumbClass (1), DumbClass (2), DumbClass(3), DumbClass (4) });
}

TEST(ARRAY, ITERATOR)
{
    DoIterator<int> (std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8});
    DoIterator<std::string> (std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8"});
    DoIterator<std::vector<int>> (std::vector<std::vector<int>> {{1, 2, 3}, {2, 3, 4}, {3, 4, 5}});
    DoIterator<DumbClass> (std::vector<DumbClass> { DumbClass (1), DumbClass (2), DumbClass(3), DumbClass (4) });
}


int main (int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}