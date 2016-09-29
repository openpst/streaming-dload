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
#ifndef _GUI_TASK_STREAMING_DLOAD_READ_TASK_H
#define _GUI_TASK_STREAMING_DLOAD_READ_TASK_H

#include <QThread>
#include "gui/task/task.h"
#include "qc/streaming_dload_serial.h"
#include "qc/streaming_dload.h"

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

#endif // _GUI_TASK_STREAMING_DLOAD_READ_TASK_H