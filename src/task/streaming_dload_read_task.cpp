/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_read_task.cpp
* @class StreamingDloadReadWorker
* @package OpenPST
* @brief Handles background processing of open/open-multi mode reading
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "task/streaming_dload_read_task.h"

using namespace OpenPST::GUI;

StreamingDloadReadTask::StreamingDloadReadTask(uint32_t address, size_t amount, std::string outFilePath, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port) :
    address(address),
    amount(amount),
    outFilePath(outFilePath),
    progressContainer(progressContainer),
    port(port)
{

}

StreamingDloadReadTask::~StreamingDloadReadTask()
{

}

void StreamingDloadReadTask::run()
{
    QString message;
    size_t step  = amount;
    size_t total = 0;

    if (port.state.negotiated && port.state.hello.maxPreferredBlockSize && step > port.state.hello.maxPreferredBlockSize) {
        step = port.state.hello.maxPreferredBlockSize;
    } else if (step > STREAMING_DLOAD_MAX_DATA_SIZE) {
        step = STREAMING_DLOAD_MAX_DATA_SIZE;
    }

    QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, amount), Q_ARG(int, 0));
    QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, message.sprintf("Reading %lu bytes from 0x%08X", amount, address)));
    QMetaObject::invokeMethod(progressContainer, "setTextRight", Qt::QueuedConnection, Q_ARG(QString, message.sprintf("0/%d bytes", amount)));
    
    emit started();

    std::ofstream file(outFilePath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    
    if (!file.is_open()) {
        emit error(message.sprintf("Error opening %s for writing", outFilePath.c_str()));
        return;
    }

    emit log(message.sprintf("Writing data to %s", outFilePath.c_str()));

    while (total < amount) {
        if (cancelled()) {
            emit aborted();
            return;
        }

        if ((amount - total) < step) {
            step = amount - total;
        }

        try {
            total += port.readFlash(address + total, step, file);
        } catch(StreamingDloadSerialError& e) {
            file.close();
            emit error(message.sprintf("Error reading %lu bytes from address 0x%08X: %s", step, (address + total), e.what()));
            return;
        } catch(SerialError& e) {
            file.close();
            emit error(message.sprintf("Error reading %lu bytes from address 0x%08X: %s", step, (address + total), e.what()));
            return;
        } catch (...) {
            file.close();
            emit error("Unexpected error encountered");
            return;
        }


        QMetaObject::invokeMethod(progressContainer, "setProgress", Qt::QueuedConnection, Q_ARG(int, total));
        QMetaObject::invokeMethod(progressContainer, "setTextRight", Qt::QueuedConnection, Q_ARG(QString, message.sprintf("%d/%d bytes", total, amount)));
    }

    emit log(message.sprintf("Read %lu bytes from 0x%08X. Contents saved to %s", amount, address, outFilePath.c_str()));

    file.close();

    emit complete();

}