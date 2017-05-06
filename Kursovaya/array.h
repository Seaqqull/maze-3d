#if !defined(MY_ARRAY_H)

#define MY_ARRAY_H

typedef unsigned int uint;

namespace Project
{
	template <class T>
	class array
	{
		T *p;
		uint size;
		uint count;
		uint grow;
	public:
		array();
		array(const array &);
		~array();
		uint GetCount();
		void SetSize(uint new_size = 0, uint new_grow = 10);
		void RemoveAll();
		void FreeExtra();

		T GetAt(uint);
		void SetAt(uint, T);
		T & ElementAt(uint);
		const T* GetData();

		void Add(T);
		uint Append(const array&);
		array& Copy(const array&);

		void InsertAt(uint, T);
		void RemoveAt(uint);

		array& operator = (const array&);
		T &operator[] (uint);
	};

	template <class T>
	array<T>::array()
	{
		p = 0;
		size = count = 0;
		grow = 10;
	}

	template <class T>
	array<T>::array(const array &v)
	{
		Copy(v);
	}

	template <class T>
	array<T>::~array()
	{
		RemoveAll();
	}

	template <class T>
	uint array<T>::GetCount()
	{
		return count;
	}

	template <class T>
	void array<T>::SetSize(uint new_size, uint new_grow)
	{
		grow = new_grow;
		if (grow == 0)grow = 10;
		if (new_size == 0){ RemoveAll(); return; }
		T *ar = new T[new_size];
		for (uint i = 0; i<new_size && i<count; i++)
			ar[i] = p[i];
		if (p) delete[]p;
		p = ar;
		size = new_size;
		if (size<count) count = size;
	}

	template <class T>
	void array<T>::RemoveAll()
	{
		if (p)delete[]p;
		p = 0; size = count = 0;
	}

	template <class T>
	void array<T>::FreeExtra()
	{
		if (size == count) return;
		SetSize(count, grow);
	}

	template <class T>
	T array<T>::GetAt(uint i)
	{
		return p[i];
	}

	template <class T>
	void array<T>::SetAt(uint i, T t)
	{
		p[i] = t;
	}

	template <class T>
	T & array<T>::ElementAt(uint i)
	{
		return p[i];
	}

	template <class T>
	const T* array<T>::GetData()
	{
		return p;
	}

	template <class T>
	void array<T>::Add(T t)
	{
		if (size == count)
		{
			size += grow;
			T *ar = new T[size];
			for (uint i = 0; i<count; i++)
				ar[i] = p[i];
			if (p) delete[]p;
			p = ar;
		}
		p[count++] = t;
	}

	template <class T>
	uint array<T>::Append(const array& v)
	{
		if (&v == this || v.count == 0) return 0;
		SetSize(count + v.count, grow);
		for (uint i = count; i<count + v.count; i++)
			p[i] = v.p[i - count];
		count += v.count;
		return count - v.count;
	}

	template <class T>
	array<T>& array<T>::Copy(const array& v)
	{
		if (&v == this) return *this;
		p = 0;
		if (v.count == 0) return *this;
		p = new T[v.size];
		size = v.size; count = v.count; grow = v.grow;
		for (uint i = 0; i<count; i++)
			p[i] = v.p[i];
		return *this;
	}

	template <class T>
	void array<T>::InsertAt(uint i, T t)
	{
		if (count == 0) return;

		if (size == count)
		{
			size += grow;
			T *ar = new T[size];
			count++;
			for (uint k = 0; k<count; k++)
			{
				if (k<i) ar[k] = p[k];
				else if (k>i) ar[k] = p[k - 1];
				else ar[k] = t;
			}
			if (p) delete[]p;
			p = ar;
		}
		else
		{
			count++;
			for (uint k = count - 1; k>i; k--)
				p[k] = p[k - 1];
			p[i] = t;
		}
	}

	template <class T>
	void array<T>::RemoveAt(uint i)
	{
		if (count == 0 || i >= count) return;
		for (uint k = i; k<count; k++)
			p[k] = p[k + 1];
		count--;
	}

	template <class T>
	array<T>& array<T>::operator = (const array& v)
	{
		return Copy(v);
	}

	template <class T>
	T & array<T>::operator[] (uint i)
	{
		return ElementAt(i);
	}
}
#endif