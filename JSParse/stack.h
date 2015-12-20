#ifndef _STACK_H_
#define _STACK_H_
template <class T>
class stack
{
	private:
		int capacity;
		int head;
		T *data;
		
		void expand();
	public:
		stack();
		~stack();
		void push(T const& item);
		T pop();
		int size();
		bool empty();
};
#endif