/*
Reference:
1주차 강의비디오
2024 데이터구조 김호준 교수님 자료
Blog:
https://velog.io/@kon6443/Data-Structure-%EC%9E%90%EB%A3%8C%EA%B5%AC%EC%A1%B0-C-%ED%9E%99
https://gwlabs.tistory.com/50
*/
#include <iostream>
#include <string>

using namespace std;

#define MAX_SIZE 30

struct Student
{
    string name;
    int score;
    string className;
};

class MaxHeap
{
private:
    Student heap[MAX_SIZE + 1];
    int size;

    int parent(int i) { return i / 2; }
    int leftChild(int i) { return 2 * i; }
    int rightChild(int i) { return 2 * i + 1; }

    void heapifyUp(int index)
    {
        while (index > 1 && heap[parent(index)].score < heap[index].score)
        {
            swap(heap[parent(index)], heap[index]);
            index = parent(index);
        }
    }

    void heapifyDown(int index)
    {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left <= size && heap[left].score > heap[largest].score)
            largest = left;
        if (right <= size && heap[right].score > heap[largest].score)
            largest = right;

        if (largest != index)
        {
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    MaxHeap() { size = 0; }

    void insert(string name, int score, string className)
    {
        if (size >= MAX_SIZE)
        {
            cout << "Error: The queue is full! Cannot insert more elements." << endl;
            return;
        }
        size++;
        heap[size] = {name, score, className};
        heapifyUp(size);
        cout << "New element [" << name << ", " << score << ", " << className << "] has been inserted." << endl;
    }

    void maximum()
    {
        if (size == 0)
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Element with the largest key: [" << heap[1].name << ", " << heap[1].score << ", " << heap[1].className << "]" << endl;
    }

    void extractMax()
    {
        if (size == 0)
        {
            cout << "Cannot delete from an empty queue." << endl;
            return;
        }
        cout << "Deleted element: [" << heap[1].name << ", " << heap[1].score << ", " << heap[1].className << "]" << endl;
        heap[1] = heap[size];
        size--;
        heapifyDown(1);
    }

    void increaseKey(int index, int newScore)
    {
        if (index < 1 || index > size)
        {
            cout << "Invalid index. Please enter a valid index between 1 and " << size << "." << endl;
            return;
        }
        heap[index].score = newScore;
        heapifyUp(index);
        cout << "Key updated. [" << heap[index].name << ", " << heap[index].score << ", " << heap[index].className << "] has been repositioned in the queue." << endl;
    }

    void printQueue()
    {
        if (size == 0)
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Current queue elements:" << endl;
        for (int i = 1; i <= size; i++)
        {
            cout << i << ". [" << heap[i].name << ", " << heap[i].score << ", " << heap[i].className << "]" << endl;
        }
    }
    int getScore(int index)
    {
        if (index >= 1 && index <= size)
            return heap[index].score;
        return -1;
    }
};

void runMenu()
{
    MaxHeap heap;
    char choice;

    while (true)
    {
        cout << "\n*********** MENU ****************" << endl;
        cout << "I : Insert a new element into the queue." << endl;
        cout << "D : Delete the element with the largest key from the queue." << endl;
        cout << "R : Retrieve the element with the largest key." << endl;
        cout << "N : Increase the key of an element in the queue." << endl;
        cout << "P : Print all elements in the queue." << endl;
        cout << "Q : Quit." << endl;
        cout << "Choose menu: ";
        cin >> choice;
        cin.ignore();

        choice = toupper(choice);

        if (choice == 'I')
        {
            string name, className;
            int score;
            cout << "Enter the name of the student: ";
            getline(cin, name);
            cout << "Enter the score of the element: ";
            cin >> score;
            while (score < 0 || score > 100)
            {
                cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                cout << "Enter the score of the element: ";
                cin >> score;
            }
            cin.ignore();
            cout << "Enter the class name: ";
            getline(cin, className);
            heap.insert(name, score, className);
        }
        else if (choice == 'D')
        {
            heap.extractMax();
        }
        else if (choice == 'R')
        {
            heap.maximum();
        }
        else if (choice == 'N')
        {
            int index, newScore;
            cout << "Enter the index of the element: ";
            cin >> index;
            cout << "Enter the new score: ";
            cin >> newScore;
            while (newScore < heap.getScore(index))
            {
                cout << "New score should be larger than current score. Please enter again." << endl;
                cout << "Enter the new score: ";
                cin >> newScore;
            }
            heap.increaseKey(index, newScore);
        }
        else if (choice == 'P')
        {
            heap.printQueue();
        }
        //hi
        else if (choice == 'Q')
        {
            cout << "Program terminated." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a valid menu option." << endl;
        }
    }
}

int main()
{
    runMenu();
    return 0;
}
