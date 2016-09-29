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

#include "gui/task/streaming_dload_stream_write_task.h"

using namespace OpenPST::GUI;

StreamingDloadStreamWriteTask::StreamingDloadStreamWriteTask(uint32_t address, std::string writeFilePath, bool unframed, StreamingDloadSerial& port) :
    address(address),
    writeFilePath(writeFilePath),
    unframed(unframed),
    port(port)
{
    
}

StreamingDloadStreamWriteTask::~StreamingDloadStreamWriteTask()
{

}


void StreamingDloadStreamWriteTask::run()
{
    /*QString tmp;

    std::ifstream file(request.filePath.c_str(), std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        emit error(request, tmp.sprintf("Error opening %s for reading", request.filePath.c_str()));
        return;
    }

    file.seekg(0, file.end);

    size_t fileSize = file.tellg();

    file.seekg(0, file.beg);

    size_t writeSize = port.state.hello.maxPreferredBlockSize;

    request.outSize = 0;

    uint8_t fileBuffer[STREAMING_DLOAD_MAX_TX_SIZE];

    if (fileSize <= writeSize) {

        file.read((char*)fileBuffer, fileSize);

        if (port.streamWrite(request.address, fileBuffer, fileSize, request.unframed) != kStreamingDloadSuccess) {
            file.close();
            emit error(request, tmp.sprintf("Error writing %lu bytes starting at address 0x%04X", fileSize, request.address));
            return;
        }

        emit chunkComplete(request);
    } else {

        uint32_t address = request.address;

        do {

            file.read((char*)fileBuffer, writeSize);

            if (port.streamWrite((address + request.outSize), fileBuffer, writeSize, request.unframed) != kStreamingDloadSuccess) {
                file.close();
                emit error(request, tmp.sprintf("Error writing %lu bytes starting at address 0x%04X", writeSize, (address + request.outSize)));
                return;
            }
            
            request.outSize += writeSize;

            emit chunkComplete(request);

        } while (request.outSize < fileSize && !cancelled);

    }       

    file.close();

    emit complete(request);
    */
}

