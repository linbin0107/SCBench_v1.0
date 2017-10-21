#ifndef DATA_H_
#define DATA_H_

#include <systemc.h>
#include <tlm.h>
#include "defines.h"

struct data
{
	//seems as if this has to be data, errors if not
    typedef data tlm_payload_type;
	//unused in our case but still necessary
  	typedef int tlm_phase_type;

	int address;
    unsigned int amba_id;
 	unsigned int *burst_size;
	unsigned int command;
	unsigned int data_length;
	unsigned int *data_ptr; //containing the reference of the submitted value
	int response_status; //the same type as tlm::TLM_OK_RESPONSE in real model
	int streaming_width;
	int *trans_type; 

	//dummy methods for being able to compile
    bool has_mm() { return false; }
  	void set_mm(bool mm) { return; }
  	void set_auto_extension(void *x) { return; }
	//these seem to be used for a kind of pooling for payloads
	//we don't use it here
  	void acquire() { return; }
  	void release() { return; }
  	int get_ref_count() { return 1; }


#ifdef DEBUG
	bool equals (data rhs) {
	  	cout << "data_ptr: this=" << data_ptr << ", other=" << rhs.data_ptr << endl;
		cout << "response_status: this=" << response_status << ", other=" << rhs.response_status << endl;
		cout << "command: this=" << command << ", other=" << rhs.command << endl;
		cout << "address: this=" << address << ", other=" << rhs.address << endl;
		cout << "data_length: this=" << data_length << ", other=" << rhs.data_length << endl;
		cout << "streaming_width: this=" << streaming_width << ", other=" << rhs.streaming_width << endl;
		cout << "trans_type: this=" << trans_type << ", other=" << rhs.trans_type << endl;
		cout << "burst_size: this=" << burst_size << ", other=" << rhs.burst_size << endl;
		cout << "amba_id: this=" << amba_id << ", other=" << rhs.amba_id << endl;

		return	data_ptr == rhs.data_ptr && 
			response_status == rhs.response_status && 
			command == rhs.command && 
			address == rhs.address && 
			data_length == rhs.data_length && 
			streaming_width == rhs.streaming_width && 
			trans_type == rhs.trans_type && 
			burst_size == rhs.burst_size && 
			amba_id == rhs.amba_id;
	}
#endif
};

struct tlmdata
{
	//seems as if this has to be data, errors if not
    typedef tlmdata tlm_payload_type;
	//unused in our case but still necessary
  	typedef int tlm_phase_type;

	//containing the submitted value
	int address;
	unsigned int command;
    unsigned int *data_ptr; 
	unsigned int data_length;
	int response_status; //the same type as tlm::TLM_OK_RESPONSE in real model
  	int streaming_width;

	//dummy methods for being able to compile
    bool has_mm() { return false; }
  	void set_mm(bool mm) { return; }
  	void set_auto_extension(void* x) { return; }
  	void acquire() { return; }
  	void release() { return; }
  	int get_ref_count() { return 1; }

};


#endif
