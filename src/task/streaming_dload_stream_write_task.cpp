/**
* LICENSE PLACEHOLDER
*
* @file streaming_dload_stream_write_task.cpp
* @class StreamingDloadStreamWriteWorker
* @package OpenPST
* @brief Handles background processing of writing data to flash using streaming write 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "task/streaming_dload_stream_write_task.h"

using namespace OpenPST::GUI;

StreamingDloadStreamWriteTask::StreamingDloadStreamWriteTask(uint32_t address, std::string filePath, bool unframed, ProgressGroupWidget* progressContainer, StreamingDloadSerial& port) :
    address(address),
    filePath(filePath),
    unframed(unframed),
    progressContainer(progressContainer),
    port(port)
{
    
}

StreamingDloadStreamWriteTask::~StreamingDloadStreamWriteTask()
{

}


void StreamingDloadStreamWriteTask::run()
{
    QString message;

    emit started();

    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        emit error(message.sprintf("Error opening %s for reading", filePath.c_str()));
        return;
    }

    file.seekg(0, file.end);

    size_t total = file.tellg();
    size_t step  = total;
    size_t written = 0;
    
    if (unframed) {
        step = STREAMING_DLOAD_MAX_DATA_SIZE * 20;
        log("Writing in unframed mode");
    } else {
        if (port.state.negotiated && port.state.hello.maxPreferredBlockSize && step > port.state.hello.maxPreferredBlockSize) {
            step = port.state.hello.maxPreferredBlockSize;
        } else if (step > STREAMING_DLOAD_MAX_DATA_SIZE) {
            step = STREAMING_DLOAD_MAX_DATA_SIZE;
        }
    }

    QMetaObject::invokeMethod(progressContainer, "setProgress",  Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, total), Q_ARG(int, 0));
    QMetaObject::invokeMethod(progressContainer, "setTextLeft",  Qt::QueuedConnection, Q_ARG(QString, message.sprintf("Writing %lu bytes at 0x%08X", total, address)));
    QMetaObject::invokeMethod(progressContainer, "setTextRight", Qt::QueuedConnection, Q_ARG(QString, message.sprintf("0/%d bytes", total)));

    char* fbuff = new char[step];

    if (!fbuff) {
        emit error(message.sprintf("Could not allocate %lu bytes for file read buffer", step));
        return;
    }

    file.seekg(0, file.beg);
    
    emit log(message.sprintf("Writing %lu bytes at %08X from %s", total, address, filePath.c_str()));

    while (written < total) {
        if (cancelled()) {
            log("Aborted Write");
            emit aborted();
            return;
        }

        if ((total - written) < step) {
            step = total - written;
        }

        if (!file.read(fbuff, step)){
            emit error(message.sprintf("Error reading file. Attempted to read %lu bytes but only read %lu bytes", step, file.gcount()));
            delete[] fbuff;
            file.close();
            return;
        }

        try {
            if (unframed) {
                written += port.writeFlashUnframed(address + written, reinterpret_cast<uint8_t*>(fbuff), step);
            } else {
                written += port.writeFlash(address + written, reinterpret_cast<uint8_t*>(fbuff), step);
            }
        } catch(StreamingDloadSerialError& e) {
            delete[] fbuff;
            file.close();
            emit error(message.sprintf("Error writing %lu bytes at address 0x%08X: %s", step, (address + total), e.what()));
            return;
        } catch(SerialError& e) {
            delete[] fbuff;
            file.close();
            emit error(message.sprintf("Error writing %lu bytes at address 0x%08X: %s", step, (address + total), e.what()));
            return;
        } catch (...) {
            delete[] fbuff;
            file.close();
            emit error("Unexpected error encountered");
            return;
        }

        QMetaObject::invokeMethod(progressContainer, "setProgress", Qt::QueuedConnection, Q_ARG(int, written));
        QMetaObject::invokeMethod(progressContainer, "setTextRight", Qt::QueuedConnection, Q_ARG(QString, message.sprintf("%d/%d bytes", written, total)));
        
    }

    emit log(message.sprintf("Wrote %lu bytes at 0x%08X from %s", written, address, filePath.c_str()));

    delete[] fbuff;

    file.close();

    emit complete();
}

