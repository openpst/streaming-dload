/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_task.h
* @class StreamingDloadReadWorker
* @package OpenPST
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include <QThread>
#include "task/task.h"
#include "qualcomm/streaming_dload_serial.h"
#include "qualcomm/streaming_dload.h"

using OpenPST::QC::StreamingDloadSerial;

namespace OpenPST {
    namespace GUI {
        class StreamingDloadReadTask : public Task
        {
            public:
                StreamingDloadReadTask(uint32_t address, size_t size, std::string outFilePath, StreamingDloadSerial& port);
                ~StreamingDloadReadTask();
                void run();
            protected:
                StreamingDloadSerial&  port;
                uint32_t address;
                size_t size;
                std::string outFilePath; 
        };
    }
}
