/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_stream_write_task.h
* @class StreamingDloadStreamWriteTask
* @package openpst/streaming-dload
* @brief Handles background processing of writing data to flash using streaming write 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "task/task.h"
#include "qualcomm/streaming_dload_serial.h"
#include "widget/progress_group_widget.h"

using OpenPST::QC::StreamingDloadSerial;
using OpenPST::QC::StreamingDloadSerialError;
using OpenPST::Serial::SerialError;

namespace OpenPST {
	namespace GUI {
		class StreamingDloadStreamWriteTask : public Task
		{
		    protected:
                uint32_t address;
                std::string filePath;
                bool unframed;
                ProgressGroupWidget* progressContainer;
			    StreamingDloadSerial&  port;
            public:
                StreamingDloadStreamWriteTask(uint32_t address, std::string filePath, bool unframed, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port);
                ~StreamingDloadStreamWriteTask();
                void run();


		};
	}
}
