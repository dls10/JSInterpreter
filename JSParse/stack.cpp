#include "stack.h"

template <class T>
stack<T>::stack()
{
	capacity = 8;
	head = -1;
	data = new T[capacity];
}

template <class T>
stack<T>::~stack()
{
	delete data;
}

template <class T>
void stack<T>::expand()
{
	T *data_exp = new T[capacity * 2];
	for (int i = 0; i < capacity; i++)
		data_exp[i] = data[i];
	capacity *= 2;
	delete data;
	data = data_exp;	
}

template <class T>
void stack<T>::push (T const& item)
{
	if (head == capacity - 2)
		expand();
	data[++head] = item;	
}

template <class T>
T stack<T>::pop ()
{
	if (head == -1)
		return 0;
	return data[head--];	
}

template <class T>
int stack<T>::size ()
{
	return head + 1;
}

template <class T>
bool stack<T>::empty ()
{
	return head < 0;
}