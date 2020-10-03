
/*****************************************************
This module contains the representation of a variant class.  the variant
maintains a union of values and their types which can be converted to
one another.  It also has several utility functions of conversion.
******************************************************/
#ifndef _SYS_VARIANT_HPP_
#define _SYS_VARIANT_HPP_


#include <stdio.h>
#include <cstring>
#include <string.h>
#include <sstream>
#include "exception_base.hpp"

using namespace std;
namespace dpm
{

	/*************************************
	Exception thrown when type conversion or comparison is not
	possible.
	**************************************/
	class TypeMismatchException : public ExceptionBase
	{
	public:
		TypeMismatchException(const char* _reason, const char* _file = NULL, int _line = -1);
	};

	enum SysVarType
	{
		V_EMPTY,
		V_CHAR,
		V_SHORT,
		V_INT,
		V_FLOAT,
		V_DOUBLE,
		V_STRING,
		V_USTRING,
		V_VOID
	};

	class SysVar
	{
	private:
		unsigned int _size;

	public:
		union
		{
			char vchar;
			short vshort;
			int vint;
			float vfloat;
			double vdouble;
			char* vstring;
			unsigned char* vustring;
			void* vvoid;
		}; // union for holding different types

		SysVarType type; // type of the data held
		const unsigned int& size; // size of the data held. used modtly with unsigned char.

		SysVar(const char _vc);
		SysVar(const short _vs);
		SysVar(const int _vi);
		SysVar(const float _vf);
		SysVar(const double _vd);
		SysVar(const char* _vs);
		SysVar(string _vs);
		SysVar(stringstream& _vss);
		SysVar(const unsigned char* _vus, unsigned int _size);
		SysVar(void* _vv);
		SysVar();
		~SysVar();

		void set_hint(int _vh);
		void clear();
		void set_UCHAR(const unsigned char* _vs, unsigned int _len);
		string get_string() const;
		void set_size(int _s_size);

		SysVar& operator=(const char _vc);
		SysVar& operator=(const short _vs);
		SysVar& operator=(const int _vi);
		SysVar& operator=(const float _vf);
		SysVar& operator=(const double _vd);
		SysVar& operator=(const char* _vs);
		SysVar& operator=(const string& _vs);
		SysVar& operator=(const stringstream& _vss);
		SysVar& operator=(void* _vv);
		SysVar& operator=(const SysVar& _var);
		SysVar(const SysVar& _vex);

		bool operator==(const SysVar& _var);
		bool operator!=(const SysVar& _var);
		SysVar operator+(const SysVar& _var);
		SysVar operator*(const SysVar& _var);
		SysVar operator%(const SysVar& _var);
		SysVar operator-(const SysVar& _var);
		SysVar operator/(const SysVar& _var);
		SysVar& operator+=(const SysVar& _var);
		SysVar& operator*=(const SysVar& _var);
		SysVar& operator%=(const SysVar& _var);
		SysVar& operator-=(const SysVar& _var);
		SysVar& operator/=(const SysVar& _var);
		bool operator<(const SysVar& _var);
		bool operator>(const SysVar& _var);
		bool operator<=(const SysVar& _var);
		bool operator>=(const SysVar& _var);

		string get_type_string();

		// convert from one type to another
		static SysVar convert(const SysVar& _v1, SysVarType type);
		// get the display string for the type of value stored
		static string get_type_string(SysVarType _type);
		// convert a string to the correct variant type.
		static SysVar to_sysvar(SysVarType _type, const char* _str);
	};
}


#endif
