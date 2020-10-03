#include "PDU.hpp"

using namespace dpm;

PDU::PDU()
{
	clear();
}

PDU::PDU(char* _buf, int _buflen)
{
	clear();
	short fc;
	GetFunctionCode(_buf, _buflen, fc);
	_functionCode = (unsigned short)fc;
	short paramCount;
	GetNoOfParameter(_buf, _buflen, paramCount);
	for (short i = 1; i <= paramCount; i++)
	{
		SysVar res;
		clear();
		GetParameter(_buf, _buflen, i, res);
		addParameter(res);
	}
}

PDU::~PDU()
{
}

void PDU::addParameter(SysVar _param)
{
	_parameters.push_back(_param);
}

void PDU::setFunctionCode(short _fc)
{
	_functionCode = (unsigned char)_fc;
}

CMD_ENUM PDU::getFunctionCode()
{
	return (CMD_ENUM)_functionCode;
}

int PDU::getParametersCount()
{
	return (int)_parameters.size();
}

SysVar PDU::getPrameterAt(int _i)
{

	return _i >= 0 && _i < (int)_parameters.size() ? _parameters.at(_i) : SysVar();
}

void PDU::makePDU(char* _buf, int _buflen, int& _retlen)
{
	int _len = 0;
	unsigned short _size = 0;
	unsigned char* _ptr;
	// We are going to use first two bytes to fill with total bytes in packet
	// so real packet starts from +2 in buffer
	// We will fill the total bytes in packet at end of this function
	_len = _len + 2;
	//

	// Fuction Code
	_buf[_len++] = (unsigned char)_functionCode;
	// No of _parameters
	_buf[_len++] = (unsigned char)_parameters.size();
	for (_paramItr = _parameters.begin(); _paramItr != _parameters.end() && _len < _buflen;
		_paramItr++)
	{
		// Lenght of the data
		_ptr = NULL;
		_size = (unsigned short)_paramItr->size;
		_ptr = (unsigned char*)&_size;
		// MSB
		_buf[_len++] = *(_ptr + 1);
		// LSB
		_buf[_len++] = *(_ptr);


		switch (_paramItr->type)
		{
		case SysVarType::V_CHAR:
			// Type
			_buf[_len++] = SysVarType::V_CHAR;
			// Data
			_buf[_len++] = _paramItr->vchar;
			break;
		case SysVarType::V_SHORT:
			// Type
			_buf[_len++] = SysVarType::V_SHORT;
			_ptr = (unsigned char*)&_paramItr->vshort;
			// Data
			for (int i = _paramItr->size - 1; i >= 0; i--)
				_buf[_len++] = *(_ptr + i);
			break;
		case SysVarType::V_INT:
			// Type
			_buf[_len++] = SysVarType::V_INT;
			_ptr = (unsigned char*)&_paramItr->vint;
			// Data
			for (int i = _paramItr->size - 1; i >= 0; i--)
				_buf[_len++] = *(_ptr + i);
			break;
		case SysVarType::V_FLOAT:
			// Type
			_buf[_len++] = SysVarType::V_FLOAT;
			// Data
			_ptr = (unsigned char*)&_paramItr->vfloat;
			for (int i = _paramItr->size - 1; i >= 0; i--)
				_buf[_len++] = (unsigned char)*(_ptr + i);
			break;
		case SysVarType::V_DOUBLE:
			// Type
			_buf[_len++] = SysVarType::V_DOUBLE;
			// Data
			_ptr = (unsigned char*)&_paramItr->vdouble;
			for (int i = _paramItr->size - 1; i >= 0; i--)
				_buf[_len++] = (unsigned char)*(_ptr + i);

			break;
		case SysVarType::V_STRING:
			// Type
			_buf[_len++] = SysVarType::V_STRING;
			// Data
			memcpy(_buf + _len, _paramItr->vstring, _paramItr->size);
			_len += _paramItr->size;
			_buf[_len] = 0;
			break;
		case SysVarType::V_USTRING:
			// Type
			_buf[_len++] = SysVarType::V_USTRING;
			// Data
			memcpy(_buf + _len, _paramItr->vstring, _paramItr->size);
			_len += _paramItr->size;
			break;
		default:
			_retlen = 0;
			return;
		}
	}
	if (_buflen <= _len)
		printf("FAULT: Remote Interface communication Problem :Buffer Overflow: Must restart "
			"communication\n");

	_retlen = _len;

	// We are substracting as we are not suppose count 2 bytes in packet size
	_len -= 2;
	_buf[0] = (char)((_len & 0xffff) >> 8);
	_buf[1] = (char)(_len & 0xff);
	return;
}
void PDU::clear()
{
	_functionCode = 0;
	_parameters.clear();
}

// Receive PDU parser functions
void PDU::GetFunctionCode(char* _buf, int _buflen, short& _ret)
{
	_ret = (short)_buf[0];
}
void PDU::GetNoOfParameter(char* _buf, int _buflen, short& _ret)
{
	_ret = (short)_buf[1];
}
void PDU::GetParameter(char* _buf, int _buflen, short _parano, SysVar& _ret)
{
	char* _ptr = _buf + 2;
	char* _tptr = NULL;

	for (int i = 1; i < _parano; i++)
	{
		unsigned short _tmp = (unsigned short)(((*_ptr) << 8) | (*(_ptr + 1) & 0x00FF));
		_ptr += (_tmp)+3;
	}


	short _type, _len = 0;

	char* _lenptr = (char*)&_len;
	*_lenptr = *(_ptr + 1);
	*(_lenptr + 1) = *(_ptr);


	_ptr += 2;
	// Now _ptr is at pointing at Type field of PDU
	_type = (short)*_ptr;

	_ptr++;
	// now _ptr is pointing first byte of data field of PDU
	switch (_type)
	{
	case SysVarType::V_CHAR:
		_ret.type = SysVarType::V_CHAR;
		_ret.set_size(_len);
		_tptr = (char*)&_ret.vchar;
		*_tptr = *_ptr;
		break;
	case SysVarType::V_SHORT:
		_ret.type = SysVarType::V_SHORT;
		_ret.set_size(_len);
		_tptr = (char*)&_ret.vshort;
		for (int i = _len - 1; i >= 0; i--, _ptr++)
			*(_tptr + i) = *_ptr;
		break;
	case SysVarType::V_INT:
		_ret.type = SysVarType::V_INT;
		_ret.set_size(_len);
		_tptr = (char*)&_ret.vint;
		for (int i = _len - 1; i >= 0; i--, _ptr++)
			*(_tptr + i) = *_ptr;
		break;
	case SysVarType::V_FLOAT:
		_ret.type = SysVarType::V_FLOAT;
		_ret.set_size(_len);
		_tptr = (char*)&_ret.vfloat;
		for (int i = _len - 1; i >= 0; i--, _ptr++)
			*(_tptr + i) = *_ptr;
		break;
	case SysVarType::V_DOUBLE:
		_ret.type = SysVarType::V_DOUBLE;
		_ret.set_size(_len);
		_tptr = (char*)&_ret.vdouble;
		for (int i = _len - 1; i >= 0; i--, _ptr++)
			*(_tptr + i) = *_ptr;
		break;
	case SysVarType::V_STRING:
		_ret.type = SysVarType::V_STRING;
		_ret.set_size(_len);
		memcpy(_ret.vstring, _ptr, _len);
		_ret.vstring[_len] = 0;
		break;
	case SysVarType::V_USTRING:
		_ret.type = SysVarType::V_USTRING;
		_ret.set_size(_len);
		memcpy(_ret.vustring, _ptr, _len);
		break;
	default:
		_ret.type = SysVarType::V_EMPTY;
		break;
	}

}

void PDU::FillPDU(char* _buf, int _buflen, PDU& _pdu)
{
	_pdu.clear();
	short fc;
	GetFunctionCode(_buf, _buflen, fc);
	_pdu.setFunctionCode(fc);
	short paramCount;
	GetNoOfParameter(_buf, _buflen, paramCount);

	for (short i = 1; i <= paramCount; i++)
	{
		SysVar res;
		res.clear();
		GetParameter(_buf, _buflen, i, res);
		_pdu.addParameter(res);
	}
}