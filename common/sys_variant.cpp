#include "sys_variant.hpp"

using namespace dpm;

TypeMismatchException::TypeMismatchException(const char* _reason, const char* _file, int _no)
	: ExceptionBase(_reason, _file, _no)
{
}

SysVar::SysVar(const char _vc)
	: type(V_CHAR)
	, vchar(_vc)
	, _size(sizeof(char))
	, size(_size)
{
}

SysVar::SysVar(const short _vs)
	: type(V_SHORT)
	, vshort(_vs)
	, _size(sizeof(short))
	, size(_size)
{
}

SysVar::SysVar(const int _vi)
	: type(V_INT)
	, vint(_vi)
	, _size(sizeof(int))
	, size(_size)
{
}

SysVar::SysVar(const float _vf)
	: type(V_FLOAT)
	, vfloat(_vf)
	, _size(sizeof(float))
	, size(_size)
{
}

SysVar::SysVar(const double _vd)
	: type(V_DOUBLE)
	, vdouble(_vd)

	, _size(sizeof(double))
	, size(_size)
{
}

SysVar::SysVar(const char* _vs)
	: type(V_STRING)
	, size(_size)
{

	if (_vs == NULL)
		return;
	int len = (int)strlen(_vs);
	vstring = new char[len + 1];
	// strcpy(const_cast<char*>(vstring),_vs);
	memcpy(const_cast<char*>(vstring), _vs, len);
	vstring[len] = 0;
	_size = len;
}

SysVar::SysVar(string _vs)
	: type(V_STRING)

	, size(_size)
{
	int len = _vs.size();
	if (len == 0)
		return;
	vstring = new char[len + 1];
	// strcpy(const_cast<char*>(vstring),_vs);
	memcpy(const_cast<char*>(vstring), _vs.c_str(), len);
	vstring[len] = 0;
	_size = len;
}

SysVar::SysVar(stringstream& _vss)
	: type(V_STRING)
	, size(_size)
{
	if (_vss.str().size() == 0)
		return;
	int len = _vss.str().size();
	vstring = new char[len + 1];
	// strcpy(const_cast<char*>(vstring),_vs);
	memcpy(const_cast<char*>(vstring), _vss.str().c_str(), len);
	vstring[len] = 0;
	_size = len;
}

SysVar::SysVar(const unsigned char* _vs, unsigned int _len)
	: type(V_STRING)
	, _size(_len)
	, vdouble(0.0)
	, size(_size)
{
	set_UCHAR(_vs, _len);
}

SysVar::SysVar(void* _vv)
	: type(V_VOID)
	, vvoid(_vv)
	, _size(sizeof(void*))
	, size(_size)
{
}

SysVar::SysVar()
	: type(V_EMPTY)
	, _size(0)
	, vdouble(0)
	, size(_size)
{
}


void SysVar::clear()
{

	if (type == V_STRING)
	{
		if (vstring != NULL)
		{
			delete[](char*)vstring;
			vstring = NULL;
		}
	}
	else if (type == V_VOID)
	{
		//      delete vvoid;
	}
	else if (type == V_USTRING)
	{
		if (vustring != NULL)
		{
			delete[](unsigned char*)vustring;
			vustring = NULL;
		}
	}

	// size = 0;
	type = V_EMPTY;
}

void SysVar::set_size(int _s_size)
{

	if (type == V_STRING)
	{
		if (vstring != NULL)
		{
			delete[] vstring;
		}

		vstring = new char[_s_size + 1];
		memset(vstring, 0, _s_size + 1);
		_size = _s_size + 1;
	}
	else if (type == V_USTRING)
	{
		if (vustring != NULL)
		{

			delete[] vustring;
		}

		vustring = new unsigned char[_s_size];
		memset(vustring, 0, _s_size);
		_size = _s_size;
	}
}

SysVar::~SysVar()
{
	clear();
}

SysVar& SysVar::operator=(const SysVar& _vex)
{

	clear();

	switch (_vex.type)
	{
	case V_EMPTY:
		break;
	case V_CHAR:
		vchar = _vex.vchar;
		break;
	case V_SHORT:
		vshort = _vex.vshort;
		break;
	case V_INT:
		vint = _vex.vint;
		break;
	case V_FLOAT:
		vfloat = _vex.vfloat;
		break;
	case V_DOUBLE:
		vdouble = _vex.vdouble;
		break;
	case V_STRING:
		vstring = new char[_vex.size + 1];
		strcpy(const_cast<char*>(vstring), _vex.vstring);
		break;
	case V_USTRING:
		vustring = new unsigned char[_vex.size];

		memcpy(const_cast<unsigned char*>(vustring), _vex.vustring, _vex.size);
		break;
	case V_VOID:
		vvoid = _vex.vvoid;
		break;
	}
	type = _vex.type;
	_size = _vex.size;

	return *this;
}

SysVar& SysVar::operator=(const char _vc)
{
	clear();
	type = V_CHAR;
	vchar = _vc;
	_size = sizeof(char);
	return *this;
}

SysVar& SysVar::operator=(const short _vs)
{
	clear();
	type = V_SHORT;
	vshort = _vs;
	_size = sizeof(short);
	return *this;
}

SysVar& SysVar::operator=(const int _vi)
{
	clear();
	type = V_INT;
	vint = _vi;
	_size = sizeof(int);
	return *this;
}

SysVar& SysVar::operator=(const float _vf)
{
	clear();
	type = V_FLOAT;
	vfloat = _vf;
	_size = sizeof(float);
	return *this;
}

SysVar& SysVar::operator=(const double _vd)
{
	clear();
	type = V_DOUBLE;
	vdouble = _vd;
	_size = sizeof(double);
	return *this;
}

SysVar& SysVar::operator=(const char* _vs)
{
	clear();
	type = V_STRING;
	int len = (int)strlen(_vs);
	vstring = new char[len + 1];
	// strcpy((char*)vstring,_vs);
	memcpy(vstring, _vs, len);
	vstring[len] = 0;
	_size = len;
	return *this;
}

SysVar& SysVar::operator=(const string& _vs)
{
	clear();
	type = V_STRING;
	int len = (int)_vs.size();
	vstring = new char[len + 1];
	memcpy(vstring, _vs.c_str(), len);
	vstring[len] = 0;
	_size = len;
	return *this;
}

SysVar& SysVar::operator=(const stringstream& _vss)
{
	clear();
	type = V_STRING;
	int len = (int)_vss.str().size();
	vstring = new char[len + 1];
	memcpy(vstring, _vss.str().c_str(), len);
	vstring[len] = 0;
	_size = len;
	return *this;
}

SysVar& SysVar::operator=(void* _vv)
{
	clear();
	type = V_VOID;
	vvoid = _vv;
	_size = sizeof(void*);
	return *this;
}

SysVar::SysVar(const SysVar& _vex)
	: size(_size)
{

	switch (_vex.type)
	{
	case V_EMPTY:
		break;
	case V_CHAR:
		vchar = _vex.vchar;
		break;
	case V_SHORT:
		vshort = _vex.vshort;
		break;
	case V_INT:
		vint = _vex.vint;
		break;
	case V_FLOAT:
		vfloat = _vex.vfloat;
		break;
	case V_DOUBLE:
		vdouble = _vex.vdouble;
		break;
	case V_STRING:
		vstring = new char[_vex.size + 1];
		strcpy(const_cast<char*>(vstring), _vex.vstring);
		break;
	case V_USTRING:
		vustring = new unsigned char[_vex.size];
		memcpy(const_cast<unsigned char*>(vustring), _vex.vustring, _vex.size);
		break;
	case V_VOID:
		vvoid = _vex.vvoid;
		break;
	}

	type = _vex.type;
	_size = _vex.size;
}

void SysVar::set_UCHAR(const unsigned char* _vs, unsigned int _len)
{
	clear();
	vustring = new unsigned char[_len];
	memcpy(const_cast<unsigned char*>(vustring), _vs, _len);
	_size = _len;
	type = V_USTRING;
}

bool SysVar::operator==(const SysVar& _vex)
{
	double _d1 = 0;
	double _d2 = 0;
	if (_vex.type == V_STRING)
	{
		if (type == V_STRING)
			return strcmp(_vex.vstring, vstring) == 0;
		else
			return false;
	}
	if (_vex.type == V_USTRING)
	{
		if (type == V_USTRING)
			return memcmp(_vex.vustring, vustring, size) == 0;
		else
			throw TypeMismatchException("Comparing string with non-string data type");
	}
	if (_vex.type == V_VOID)
	{
		if (type == V_VOID)
			return _vex.vvoid == vvoid;
		else
			throw TypeMismatchException("Comparing void* with different data type");
	}
	if (_vex.type == V_EMPTY)
	{
		if (type == V_EMPTY)
			return true;
		else
			throw TypeMismatchException("Comparing empty with non-empty data type");
	}
	switch (_vex.type)
	{
	case V_EMPTY:
		if (type != V_EMPTY)
			throw TypeMismatchException("Comparing empty with non-empty data type");
		return true;
		break;
	case V_CHAR:
		_d1 = _vex.vchar;
		break;
	case V_SHORT:
		_d1 = _vex.vshort;
		break;
	case V_INT:
		_d1 = _vex.vint;
		break;
	case V_FLOAT:
		_d1 = _vex.vfloat;
		break;
	case V_DOUBLE:
		_d1 = _vex.vdouble;
		break;
	default:
		return false;
		break;
	}
	switch (type)
	{
	case V_EMPTY:
		if (_vex.type != V_EMPTY)
			return false;
		return true;
		break;
	case V_CHAR:
		_d2 = vchar;
		break;
	case V_SHORT:
		_d2 = vshort;
		break;
	case V_INT:
		_d2 = vint;
		break;
	case V_FLOAT:
		_d2 = vfloat;
		break;
	case V_DOUBLE:
		_d2 = vdouble;
		break;
	default:
		return false;
	}
	return _d1 == _d2;
}
string SysVar::get_type_string()
{
	return SysVar::get_type_string(type);
}

SysVar SysVar::convert(const SysVar& _v1, SysVarType type)
{
	SysVar _ans;
	if (_v1.type == type)
		return _v1;
	if (_v1.type == SysVarType::V_STRING || _v1.type == SysVarType::V_USTRING
		|| _v1.type == SysVarType::V_VOID)
	{
		if (_v1.type == type)
			return _v1;
		throw TypeMismatchException("Converting string with non-string data type");
	}
	if (type == SysVarType::V_STRING || type == SysVarType::V_USTRING || type == SysVarType::V_VOID)
	{
		if (_v1.type == type)
			return _v1;
		throw TypeMismatchException("Converting string with non-string data type");
	}
	if (_v1.type == SysVarType::V_EMPTY)
	{
		_ans.type = type;
		return _ans;
	}
	double _d2 = 0;
	switch (_v1.type)
	{
	case SysVarType::V_CHAR:
		_d2 = (unsigned char)_v1.vchar;
		break;
	case SysVarType::V_SHORT:
		_d2 = _v1.vshort;
		break;
	case SysVarType::V_INT:
		_d2 = _v1.vint;
		break;
	case SysVarType::V_FLOAT:
		_d2 = _v1.vfloat;
		break;
	case SysVarType::V_DOUBLE:
		_d2 = _v1.vdouble;
		break;
	default:
		break;
	}
	switch (type)
	{
	case SysVarType::V_CHAR:
		_ans = (char)_d2;
		break;
	case SysVarType::V_SHORT:
		_ans = (short)_d2;
		break;
	case SysVarType::V_INT:
		_ans = (int)_d2;
		break;
	case SysVarType::V_FLOAT:
		_ans = (float)_d2;
		break;
	case SysVarType::V_DOUBLE:
		_ans = _d2;
		break;
	default: break;
	}
	return _ans;
}

SysVar& SysVar::operator+=(const SysVar& _var)
{
	if (_var.type == V_EMPTY)
		return *this;
	if (type == V_VOID || _var.type == V_VOID)
		return *this;
	if (type == V_STRING)
	{
		if (_var.type != V_STRING)
			return *this;
		else
		{
			char* _temp = new char[sizeof(size + _var.size)];

			strcpy(_temp, vstring);
			strcat(_temp, _var.vstring);
			delete[] vstring;
			*this = _temp;
			return *this;
		}
	}
	if (type == V_USTRING)
	{
		if (_var.type != V_USTRING)
			return *this;
		else
		{
			unsigned char* _temp = new unsigned char[sizeof(size + _var.size)];
			memcpy(_temp, vustring, size);
			for (unsigned int _index = 0; _index < _var.size; _index++)
				_temp[_index + size] = _var.vustring[_index];
			delete[] vustring;
			*this = _temp;
			return *this;
		}
	}
	SysVar _ans1;
	SysVar _ans2;
	if (type > _var.type)
	{
		_ans1 = convert(_var, type);
		_ans2 = *this;
	}
	else
	{
		_ans1 = convert(*this, _var.type);
		_ans2 = _var;
	}
	switch (_ans1.type)
	{
	case V_CHAR:
		*this = (char)(_ans1.vchar + _ans2.vchar);
		break;
	case V_SHORT:
		*this = (short)(_ans1.vshort + _ans2.vshort);
		break;
	case V_INT:
		*this = (int)(_ans1.vint + _ans2.vint);
		break;
	case V_FLOAT:
		*this = (float)(_ans1.vfloat + _ans2.vfloat);
		break;
	case V_DOUBLE:
		*this = (double)(_ans1.vdouble + _ans2.vdouble);
		break;
	default: break;
	}
	return *this;
}

SysVar& SysVar::operator*=(const SysVar& _var)
{
	if (type == V_STRING || type == V_USTRING || type == V_VOID)
		return *this;
	SysVar _ans1;
	SysVar _ans2;
	if (type > _var.type)
	{
		_ans1 = convert(_var, type);
		_ans2 = *this;
	}
	else
	{
		_ans1 = convert(*this, _var.type);
		_ans2 = _var;
	}
	switch (_ans1.type)
	{
	case V_CHAR:
		*this = (int)(_ans1.vchar * _ans2.vchar);
		break;
	case V_SHORT:
		*this = (short)(_ans1.vshort * _ans2.vshort);
		break;
	case V_INT:
		*this = (int)(_ans1.vint * _ans2.vint);
		break;
	case V_FLOAT:
		*this = (float)(_ans1.vfloat * _ans2.vfloat);
		break;
	case V_DOUBLE:
		*this = (double)(_ans1.vdouble * _ans2.vdouble);
		break;
	default: break;
	}
	return *this;
}

SysVar& SysVar::operator%=(const SysVar& _var)
{
	if (type == V_STRING || type == V_USTRING || type == V_VOID)
		return *this;
	SysVar _ans1;
	SysVar _ans2;
	_ans1 = convert(*this, V_INT);
	_ans2 = convert(_var, V_INT);
	*this = (int)(_ans1.vint % _ans2.vint);
	return *this;
}

SysVar& SysVar::operator-=(const SysVar& _var)
{
	if (type == V_STRING || type == V_USTRING || type == V_VOID)
		return *this;
	SysVar _ans1;
	SysVar _ans2;
	if (type > _var.type)
	{
		_ans1 = *this;
		_ans2 = convert(_var, type);
	}
	else
	{
		_ans1 = convert(*this, _var.type);
		_ans2 = _var;
	}
	switch (_ans1.type)
	{
	case V_CHAR:
		*this = (char)(_ans1.vchar - _ans2.vchar);
		break;
	case V_SHORT:
		*this = (short)(_ans1.vshort - _ans2.vshort);
		break;
	case V_INT:
		*this = (short)(_ans1.vint - _ans2.vint);
		break;
	case V_FLOAT:
		*this = (float)(_ans1.vfloat - _ans2.vfloat);
		break;
	case V_DOUBLE:
		*this = (double)(_ans1.vdouble - _ans2.vdouble);
		break;
	default: break;
	}
	return *this;
}

SysVar& SysVar::operator/=(const SysVar& _var)
{
	if (type == V_STRING || type == V_USTRING || type == V_VOID)
		return *this;
	SysVar _ans1;
	SysVar _ans2;
	if (type > _var.type)
	{
		_ans1 = *this;
		_ans2 = convert(_var, type);
	}
	else
	{
		_ans1 = convert(*this, _var.type);
		_ans2 = _var;
	}
	switch (_ans1.type)
	{
	case V_CHAR:
		*this = (float)(_ans1.vchar / _ans2.vchar);
		break;
	case V_SHORT:
		*this = (float)(_ans1.vshort / _ans2.vshort);
		break;
	case V_INT:
		*this = (float)(_ans1.vint / _ans2.vint);
		break;
	case V_FLOAT:
		*this = (float)(_ans1.vfloat / _ans2.vfloat);
		break;
	case V_DOUBLE:
		*this = (double)(_ans1.vdouble / _ans2.vdouble);
		break;
	default: break;
	}
	return *this;
}

SysVar SysVar::operator+(const SysVar& _var)
{
	SysVar _ans = *this;
	_ans += _var;
	return _ans;
}

SysVar SysVar::operator*(const SysVar& _var)
{
	SysVar _ans = *this;
	_ans *= _var;
	return _ans;
}

SysVar SysVar::operator%(const SysVar& _var)
{
	SysVar _ans = *this;
	_ans %= _var;
	return _ans;
}

SysVar SysVar::operator-(const SysVar& _var)
{
	SysVar _ans = *this;
	_ans -= _var;
	return _ans;
}

SysVar SysVar::operator/(const SysVar& _var)
{
	SysVar _ans = *this;
	_ans /= _var;
	return _ans;
}

bool SysVar::operator<(const SysVar& _var)
{
	if (type == V_STRING)
	{
		if (_var.type != V_STRING)
			throw TypeMismatchException("Comparing string to a non-string variable");
		return strcmp(vstring, _var.vstring) < 0;
	}
	if (type == V_VOID || _var.type == V_VOID || _var.type == V_EMPTY || type == V_EMPTY)
		throw TypeMismatchException("Comparing void* or empty variants not allowed");
	SysVar _ans1;
	_ans1 = convert(_var, type);
	switch (type)
	{
	case V_CHAR:
		return vchar < _ans1.vchar;
	case V_SHORT:
		return vshort < _ans1.vshort;
	case V_INT:
		return vint < _ans1.vint;
	case V_FLOAT:
		return vfloat < _ans1.vfloat;
	case V_DOUBLE:
		return vdouble < _ans1.vdouble;
	default: break;
	}
	return false;
}

bool SysVar::operator>(const SysVar& _var)
{
	if (type == V_STRING)
	{
		if (_var.type != V_STRING)
			throw TypeMismatchException("Comparing string to a non-string variable");
		return strcmp(vstring, _var.vstring) < 0;
	}
	if (type == V_VOID || _var.type == V_VOID || _var.type == V_EMPTY || type == V_EMPTY)
		throw TypeMismatchException("Comparing void* or empty variants not allowed");
	SysVar _ans1;
	_ans1 = convert(_var, type);
	switch (type)
	{
	case V_CHAR:
		return vchar > _ans1.vchar;
	case V_SHORT:
		return vshort > _ans1.vshort;
	case V_INT:
		return vint > _ans1.vint;
	case V_FLOAT:
		return vfloat > _ans1.vfloat;
	case V_DOUBLE:
		return vdouble > _ans1.vdouble;
	default:break;
	}
	return false;
}

bool SysVar::operator<=(const SysVar& _var)
{
	if (type == V_STRING)
	{
		if (_var.type != V_STRING)
			throw TypeMismatchException("Comparing string to a non-string variable");
		return strcmp(vstring, _var.vstring) < 0;
	}
	if (type == V_VOID || _var.type == V_VOID || _var.type == V_EMPTY || type == V_EMPTY)
		throw TypeMismatchException("Comparing void* or empty variants not allowed");
	SysVar _ans1;
	_ans1 = convert(_var, type);
	switch (type)
	{
	case V_CHAR:
		return vchar <= _ans1.vchar;
	case V_SHORT:
		return vshort <= _ans1.vshort;
	case V_INT:
		return vint <= _ans1.vint;
	case V_FLOAT:
		return vfloat <= _ans1.vfloat;
	case V_DOUBLE:
		return vdouble <= _ans1.vdouble;
	default: break;
	}
	return false;
}

bool SysVar::operator>=(const SysVar& _var)
{
	if (type == V_STRING)
	{
		if (_var.type != V_STRING)
			throw TypeMismatchException("Comparing string to a non-string variable");
		return strcmp(vstring, _var.vstring) < 0;
	}
	if (type == V_VOID || _var.type == V_VOID || _var.type == V_EMPTY || type == V_EMPTY)
		throw TypeMismatchException("Comparing void* or empty variants not allowed");
	SysVar _ans1;
	_ans1 = convert(_var, type);
	switch (type)
	{
	case V_CHAR:
		return vchar >= _ans1.vchar;
	case V_SHORT:
		return vshort >= _ans1.vshort;
	case V_INT:
		return vint >= _ans1.vint;
	case V_FLOAT:
		return vfloat >= _ans1.vfloat;
	case V_DOUBLE:
		return vdouble >= _ans1.vdouble;
	default: break;
	}
	return false;
}

string SysVar::get_string() const
{
	char _temp[1000];
	unsigned int i = 0;
	char _temp1[100];
	_temp[0] = '\0';
	switch (type)
	{
	case V_CHAR:
		sprintf(_temp, "%d", (int)vchar);
		return _temp;
	case V_SHORT:
		sprintf(_temp, "%d", vshort);
		return _temp;
	case V_INT:
		sprintf(_temp, "%d", vint);
		return _temp;
	case V_FLOAT:
		sprintf(_temp, "%f", vfloat);
		return _temp;
	case V_DOUBLE:
		sprintf(_temp, "%f", vdouble);
		return _temp;
	case V_STRING:
		return vstring;
	case V_USTRING:
	{

		for (i = 0; i < (size > 24 ? 24 : size); i++)
		{
			sprintf(_temp1, "0x%X,", (int)vustring[i]);
			strcat(_temp, _temp1);
		}
		return _temp;
	}
	case V_VOID:
		sprintf(_temp, "0x%x", *((unsigned int*)(vvoid)));
		return _temp;
	default:
		return "";
	}
}

bool SysVar::operator!=(const SysVar& _var)
{
	return !(operator==(_var));
}

string SysVar::get_type_string(SysVarType _type)
{
	const char* _types[] = { "empty", "unsigned char", "short", "int", "float", "double", "string",
		"unsigned char*" };
	return _types[_type];
}

SysVar SysVar::to_sysvar(SysVarType _type, const char* _str)
{
	SysVar _var;
	unsigned char* _ustr;
	char* _tmp;
	int _index = 0;
	_var.type = _type;
	switch (_type)
	{
	case SysVarType::V_CHAR:
		_var.vchar = _str[0];
		return _var;
	case SysVarType::V_SHORT:
		sscanf(_str, "%hu", &_var.vshort);
		return _var;
	case SysVarType::V_INT:
		sscanf(_str, "%d", &_var.vint);
		return _var;
	case SysVarType::V_FLOAT:
		sscanf(_str, "%f", &_var.vfloat);
		return _var;
	case SysVarType::V_DOUBLE:
		_var.vdouble = strtod(_str, &_tmp);
		return _var;
	case SysVarType::V_STRING:
		_var = _str;
		return _var;
	case SysVarType::V_USTRING:
	{
		int length = (int)strlen(_str);

		_ustr = new unsigned char[length];
		char* _c = (char*)_str;
		char* _prev = _c;
		while ((_c = strchr(_c, ',')) && _index < length)
		{
			*_c = '\0';
			int _i = 0;
			sscanf(_prev, "%x", &_i);
			_ustr[_index++] = (unsigned char)_i;
			_prev = ++_c;
		}
		if (_index < length && *_prev != '\0')
		{
			int _i = 0;
			sscanf(_prev, "%x", &_i);
			_ustr[_index++] = (unsigned char)_i;
		}
	}
	_var.set_UCHAR(_ustr, _index);
	delete[] _ustr;
	return _var;
	default:
		_var.type = SysVarType::V_EMPTY;
		return _var;
	}
}
