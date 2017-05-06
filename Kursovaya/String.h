#define _CRT_SECURE_NO_WARNINGS
#if !defined(MY_STRING_H)

#define MY_STRING_H
#include <iostream>
#include <stdlib.h>
typedef unsigned int ui;
class String
{
private:
	char* str;
	ui length;
public:			
	String();
	String(const char*);
	String(const String&);
	String(char, int);
	~String();
	//methods
	ui GetLength();
	ui GetSize();
	char& GetAt(int);
	void SetAt(int, char);
	int GetInt();
	double GetDouble();
	bool IsThere(char);
	int GetPosition(char);
	int Compare(const char *);	//const --
	ui Find_Letter(const char);//+
	void Conversaly();
	String TrimLeft(char letter);
	String TrimRight(char letter);
	String Trim(char letter, bool from_beg, bool from_end);
	//types
	operator char*();

	//operators
	String& operator=(const String&);
	String& operator=(const char *);
	String& operator=(const int);
	String& operator=(char);

	String operator + (const String&) const;
	String operator + (const char *) const;
	String operator + (char) const;
	friend String operator + (const char *, const String&);
	friend String operator + (char, const String&);

	bool operator==(const char*);
	bool operator==(const String&);
	friend bool operator ==(const char*, const String&);

	String& operator+=(const String&);
	String& operator+=(const char*);
	String& operator+=(char);
	String& operator += (int);
	void operator -- (){ str[length - 1] = NULL; length--; }

	char& operator[](int);
	void operator >>(const char*);
	friend std::istream& operator>>(std::istream&, String&);
	friend std::ostream& operator<<(std::ostream& , String& );	
};
#endif