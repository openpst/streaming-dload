/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_gpt_task.h
* @class StreamingDloadReadGptTask
* @package openpst/streaming-dload
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "task/task.h"
#include "qualcomm/streaming_dload_serial.h"
#include "util/gpt_parser.h"
#include "widget/progress_group_widget.h"

using OpenPST::QC::StreamingDloadSerial;
using OpenPST::QC::StreamingDloadSerialError;
using OpenPST::Serial::SerialError;

namespace OpenPST {
    namespace GUI {
        class StreamingDloadReadGptTask : public Task
        {
            Q_OBJECT

            private:
                QString filePath;
                ProgressGroupWidget* progressContainer;
                StreamingDloadSerial&  port;
            public:
                StreamingDloadReadGptTask(QString filePath, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port);
                ~StreamingDloadReadGptTask();
                void run();
            signals:
                void received(QString filePath);            
        };
    }
}
