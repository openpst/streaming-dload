/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of streaming-dload.
* 
* streaming-dload is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* streaming-dload is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with streaming-dload. If not, see <http://www.gnu.org/licenses/>.
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
