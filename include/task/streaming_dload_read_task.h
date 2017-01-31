/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_task.h
* @class StreamingDloadReadTask
* @package openpst/streaming-dload
* @brief Handles background processing of open/open-multi mode reading
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
        class StreamingDloadReadTask : public Task
        {
            private:
                uint32_t    address;
                size_t      amount;
                std::string outFilePath;
                ProgressGroupWidget* progressContainer;
                StreamingDloadSerial&  port;
            public:
                StreamingDloadReadTask(uint32_t address, size_t amount, std::string outFilePath, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port);
                ~StreamingDloadReadTask();
                void run();
        };
    }
}
