#include "PLC-master.h"

Master::Master()
{
	uint8_t i;
	this->num_devices = 0;
	for(i=0; i<10; i++)
		this->id_devices[i]  = 0;
}

void Master::InsertDevice(uint8_t id_device)
{
	if(this->num_devices==20)
		return;
	this->num_devices++;
	this->id_devices[this->num_devices-1] = id_device;	
}


int8_t Master::FindDevice(uint8_t id_device)
{
	uint8_t i;
	if(this->num_devices==0)
		return false;
	for(i=0; i<this->num_devices; i++)
		if(this->id_devices[i]==id_device)
			return i;
	return -1;
}

void Master::DeleteDevice(uint8_t id_device)
{
	uint8_t i;
	int8_t index_id = this->FindDevice(id_device);
	if(index_id==-1)
		return;
	for(i=index_id; i<this->num_devices-1; i++)
		this->id_devices[i] = this->id_devices[i+1];
	this->id_devices[this->num_devices-1] = 0;
	this->num_devices--;
}
/*-------------------------------------------------------------*/
Frame::Frame()
{
	this->Reset();
}

void Frame::Reset()
{
	this->check_start = false;
	this->check_stop = false;
	this->buff = "???";
	this->data_frame = "";
}

void Frame::Catch(uint8_t ch)
{
	this->buff[0] = this->buff[1];
	this->buff[1] = this->buff[2];
	this->buff[2] = ch;
	
	if(this->check_start==false)
	{
		if(this->buff=="MS:")
		{
			this->check_start = true;
			this->data_frame += "MS/";
		}
		if(this->buff=="SL:")
		{
			this->check_start = true;
			this->data_frame += "SL/";
		}
	}
	else
	{
		this->data_frame += ch;
		if(this->buff==":MS" || this->buff==":SL")
		{
			this->check_stop = true;
			this->data_frame.erase(this->data_frame.end()-3, this->data_frame.end());
		}
	}
}

bool Frame::Get_checkStart()
{
	return this->check_start;
}

bool Frame::Get_checkStop()
{
	return this->check_stop;
}

