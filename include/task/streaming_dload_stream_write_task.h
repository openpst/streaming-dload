/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_stream_write_task.h
* @class StreamingDloadStreamWriteWorker
* @package OpenPST
* @brief Handles background processing of writing data to flash using streaming write 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include <QThread>
#include "qualcomm/streaming_dload_serial.h"
#include "qualcomm/streaming_dload.h"
#include "task/task.h"

using OpenPST::QC::StreamingDloadSerial;

namespace OpenPST {
	namespace GUI {
		class StreamingDloadStreamWriteTask : public Task
		{
		    public:
                StreamingDloadStreamWriteTask(uint32_t address, std::string writeFilePath, bool unframed, StreamingDloadSerial& port);
                ~StreamingDloadStreamWriteTask();
                void run();

		    protected:
			    StreamingDloadSerial&  port;
                uint32_t address;
                std::string writeFilePath;
                bool unframed;
		};
	}
}
