#pragma once
#include "Packet.h"

namespace sp
{


	struct HouseKeeping3VCPI: public Packet
	{
			word		_data[83];
	};



	template<class T>
	inline T&	operator >> (T& reader, HouseKeeping3VCPI& in)
	{
		reader.read(reinterpret_cast<byte*>(&in._data[0]),sizeof(in._data));
		in.PacketID = in._data[0];
		return reader;
	}


}
