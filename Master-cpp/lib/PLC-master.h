#include<iostream>
#include <cstdint>
#include <string>


class Master{
	public:
		Master();
		void InsertDevice(uint8_t id_device);
		int8_t FindDevice(uint8_t id_device);
		void DeleteDevice(uint8_t id_device);
	private:
		uint8_t num_devices;
		uint8_t id_devices[20];	
};

class Frame{
	public:
		Frame();
		void Reset();
		void Catch(uint8_t ch);
		bool Get_checkStart();
		bool Get_checkStop();
	private:
		bool check_start;
		bool check_stop;
		string buff;
		string data_frame;	
};


