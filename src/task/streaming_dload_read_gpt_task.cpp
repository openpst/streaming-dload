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
* @file streaming_dload_read_gpt_task.cpp
* @class StreamingDloadReadTask
* @package openpst/streaming-dload
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "task/streaming_dload_read_gpt_task.h"

using namespace OpenPST::GUI;

StreamingDloadReadGptTask::StreamingDloadReadGptTask(QString filePath, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port) :
    filePath(filePath),
    progressContainer(progressContainer),
    port(port)
{

}

StreamingDloadReadGptTask::~StreamingDloadReadGptTask()
{

}

void StreamingDloadReadGptTask::run()
{
    QString message;
    std::vector<uint8_t> data;
    size_t toRead = GPT_MAX_SIZE;
    size_t read = 0;
    size_t step = toRead;
    int progressTotal = toRead;

    if (port.state.negotiated && port.state.hello.maxPreferredBlockSize && step > port.state.hello.maxPreferredBlockSize) {
        step = port.state.hello.maxPreferredBlockSize;
    } else if (step > STREAMING_DLOAD_MAX_DATA_SIZE) {
        step = STREAMING_DLOAD_MAX_DATA_SIZE;
    }

    emit started();

    if (!port.isOpen()) {
        emit error("Select a device and connect first");
        return;
    }

    if (!port.state.negotiated) {
        emit error("Send Hello First");
        return;
    }

    std::ofstream file(filePath.toStdString(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        emit error("Could not create file on local filesystem");
        return;
    }

    QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, progressTotal), Q_ARG(int, 0));
    QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, ""));
    QMetaObject::invokeMethod(progressContainer, "setTextRight", Qt::QueuedConnection, Q_ARG(QString, ""));

    try {

        QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 20));
        QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, "Setting security mode"));

        port.setSecurityMode(kStreamingDloadSecurityModeUntrusted);

        QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 30));
        QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, "Opening EMMC User Partition"));

        port.openMode(kStreamingDloadOpenModeMultiEmmcUser);
        
        QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 40));

        message.sprintf("Reading %lu bytes from start of EMMC to 0x%08X", toRead, toRead);

        QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, message));

        while(read < toRead) {
            if (cancelled()) {
                log("Aborted Read");
                emit aborted();
                return;
            }

            read += port.readFlash(0x00000000 + read, step, data);

            QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, read));
        }
        
        QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, "Done"));


        if (!file.write(reinterpret_cast<const char*>(&data[0]), data.size())) {
            emit error("Error writing to temporary file");
            return;
        }

        file.close();

        emit received(filePath);

    } catch(StreamingDloadSerialError& e) {
        emit error(e.what());
        return;
    } catch(SerialError& e) {
        emit error(e.what());
        return;
    } catch (...) {
        emit error("Unexpected error encountered");
        return;
    }

    emit complete();
}