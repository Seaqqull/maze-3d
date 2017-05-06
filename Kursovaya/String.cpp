#include "String.h"

String::String()
{
	str = NULL;
	length = NULL;
}
//explicit - чтобы запретить конструктору участвовать в преобразованиях
String::String(const char* line){
	str = NULL;
	length = NULL;
	if (line){
		length = strlen(line);
		str = new char[length + 1];
		strcpy(str, line);
		str[length] = '\0';
	}
}

String::String(const String& line_object){
	length = strlen(line_object.str);
	str = new char[length + 1];
	strcpy(str, line_object.str);
	str[length] = '\0';
	
}

String::String(char c, int repeat){
	length = repeat;
	str = new char[length + 1];	
	for (ui i = 0; i<length; i++)
	{
		str[i] = c;
		str[i + 1] = NULL;
	}
}

String::~String(){
	delete str;	
}

//methods

ui String::GetLength(){
	return length;	
}

ui String::GetSize(){
	return length + 1;
}

char& String::GetAt(int position){
	return str[position];
}

void String::SetAt(int position, char c){
	str[position] = c;
}

int String::GetInt(){
	return atoi(str);
}

double String::GetDouble(){
	return atof(str);
}

bool String::IsThere(char letter){
	for (ui i(0); i < length; i++)
		if (str[i] == letter) return true;
	return false;
}

int String::GetPosition(char letter){
	if (IsThere(letter))
		for (ui i(0); i < length; i++)
			if (str[i] == letter)
				return i;
	return -1;
}

int String::Compare(const char* str){
	return strcmp((*this).str, str);
}

void String::Conversaly(){
	char* new_str = new char[length + 1];
	for (int i(length - 1); i >= 0; i--)
		new_str[length - i - 1] = str[i];	
	new_str[length] = '\0';
	strcpy(str, new_str);
}

String String::TrimLeft(char letter){
	String s(str);
	s.Conversaly();
	for (int i(s.length - 1); i >= 0; i--)
		if (s[i] == letter)
			--s;
		else
			break;
	s.Conversaly();
	return s;
}

String String::TrimRight(char letter){
	String s(str);
	for (int i(s.length - 1); i >= 0; i--)
		if (s[i] == letter)
			--s;
		else
			break;	
	return s;
}

String String::Trim(char letter, bool from_beg, bool from_end){
	String s(str);
	if (from_end)
		s = s.TrimRight(letter);
	if (from_beg)
		s = s.TrimLeft(letter);
	return s;
}

//types
String::operator char*(){
	/*char* new_line = new char[this->length + 1];
	for (ui i(0); i < length; i++)
		new_line[i] = this->str[i];
	new_line[length] = '\0';
	return new_line;*/
	/*return str;*/
	char* new_line = new char[this->length + 1];
	for (ui i(0); i < length; i++)
		new_line[i] = this->str[i];
	new_line[length] = '\0';
	return new_line;
}
//operators
//=
String& String::operator=(const String& line_object){
	/*if(str)
		delete[] str;*/
	
	/*length = strlen(line_object.str);
	str = new char[length + 1];
	for (ui i(0); i < length; i++)
		str[i] = line_object.str[i];
	str[length] = '\0';
	return *this;*/

	if (str) delete str;
		str = new char[strlen(line_object.str) + 1];
		strcpy(str, line_object.str);
		length = strlen(str);	

	return *this;
}

String& String::operator=(const char* line_object){
	/*if (str)
		delete[] str;

	length = strlen(line_object);
	str = new char[length + 1];
	for (ui i(0); i < length; i++)
		str[i] = line_object[i];
	str[length] = '\0';
	return *this;*/
	if (str) delete str;
	str = new char[strlen(line_object) + 1];
	strcpy(str, line_object);
	length = strlen(str);
	return *this;
}

String& String::operator=(const int number){
	if (str)
		delete[] str;
	int count = 0;
	char buff[100];
	//_strset(buff, '\0');
	sprintf(buff, "%d", number);
	for (int i(0); i < 100; i++){
		if (buff[i] != '\0') count++;
	}
	length = count;
	str = new char[length + 1];
	strcpy(str, buff);
	str[length] = '\0';
	return *this;
}

String& String::operator=(char line_object){
	if (str)
		delete[] str;

	length = 1;
	str = new char[length + 1];
	str[0] = line_object;
	str[length] = '\0';
	return *this;
}

//+!!!
String String::operator + (const String & object_cl) const
{
	String s(str);
	s += object_cl.str;
	return s;
}

String String::operator + (const char * line_object) const
{
	String s(str);
	s += line_object;
	return s;
}

String String::operator + (char key) const
{
	String s(str);
	s += key;
	return s;
}

String operator + (const char *line_object, const String& object_cl)
{
	String s(line_object);
	s += object_cl.str;
	return s;
}

String operator + (char key, const String& object_cl)
{
	String s(key, 1);
	s += object_cl.str;
	return s;
}

//==
bool String::operator==(const char *str){
	return Compare(str) == 0;
}

bool String::operator==(const String &str){
	return Compare(str.str) == 0;
}

bool operator ==(const char * line_object, const String& object_cl){
	return strcmp(object_cl.str, line_object) == 0;
}
//+=
String& String::operator+=(const String &line_object){
	if (line_object.length == 0) return *this;
	
	char* new_line = new char[((str) ? length : 0) + line_object.length + 1];

	if (str){
		strcpy(new_line, str);
		delete[] str;
	}
	strcat(new_line, line_object.str);
	str = new_line;
	length += line_object.length;
	return *this;	
}

String& String::operator+=(const char *str_)
{
	int count = strlen(str_);
	if (count == 0) return *this;

	char *new_line = new char[((str) ? length : 0) + count + 1];
	
	if (str)
	{
		strcpy(new_line, str);
		delete str;
	}
	strcat(new_line, str_);
	str = new_line;	
	length += count;
	str[length] = '\0';
	return *this;
}

String& String::operator+=(char letter){
	int count = (str) ? length : 0;

	char* new_line = new char[count + 2];
	if (str){
		strcpy(new_line, str);
		delete[] str;
	}
	new_line[count] = letter;
	new_line[count + 1] = '\0';
	str = new_line;
	length = ++count;
	return *this;
}

String& String::operator += (int number){///!!!
	length += 1;
	char* new_line = new char[length + 1];	
	
	strcpy(new_line, str);
	delete[] str;	
	
	new_line[length - 1] = 48 + number;
	new_line[length] = '\0';
	str = new_line;	
	return *this;
}

char& String::operator[](int position){
	return GetAt(position);
}

void String::operator >>(const char *line_object)
{
	(*this) += line_object;
}

std::istream& operator>>(std::istream& ics, String& line_object){
	delete line_object.str;
	char* new_line = new char[50];
	ics >> new_line;
	line_object.str = new char[strlen(new_line) + 1];
	strcpy(line_object.str, new_line);
	line_object.length = strlen(line_object.str);	
	line_object.str[line_object.GetLength()] = '\0';
	delete new_line;
	return ics;
}

std::ostream& operator<<(std::ostream& ocs, String& current_line){
	ocs << current_line.str;
	return ocs;
}
