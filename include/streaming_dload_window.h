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
* @file streaming_dload_window.h
* @class StreamingDloadWindow
* @package OpenPST
* @brief Streaming DLOAD GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include <iostream>
#include <fstream>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <vector>
#include "ui_streaming_dload_window.h"
#include "qualcomm/streaming_dload.h"
#include "qualcomm/streaming_dload_serial.h"
#include "qualcomm/raw_program_xml_reader.h"
#include "lg/partition_txt_reader.h"
#include "task/streaming_dload_read_task.h"
#include "task/streaming_dload_stream_write_task.h"
#include "task/streaming_dload_read_gpt_task.h"
#include "task/task_runner.h"
#include "util/gpt_parser.h"
#include "about_dialog.h"
#include "table_dialog.h"


namespace Ui {
	class StreamingDloadWindow;
}

using OpenPST::QC::StreamingDloadSerialError;
using OpenPST::QC::RawProgramXmlReader;
using OpenPST::QC::RawProgramXmlReaderError;
using OpenPST::QC::RawProgramXmlEntry;
using OpenPST::LG::PartitionTxtReader;
using OpenPST::LG::PartitionTxtReaderError;
using OpenPST::LG::PartitionTxtEntry;
using OpenPST::Serial::SerialError;

namespace OpenPST{
	namespace GUI {

		struct ResolvedRawProgramXmlEntry {
			RawProgramXmlEntry entry;
			QString sourceFile;
			QString path;
		};

		struct ResolvedPartitionTxtEntry {
			PartitionTxtEntry entry;
			QString sourceFile;
			QString path;
		};

		class StreamingDloadWindow : public QMainWindow
		{

            Q_OBJECT

			enum GptEntiesColumns{
				kGptEntiesColumnName = 0,
				kGptEntiesColumnStartLBA,
				kGptEntiesColumnEndLBA,
				kGptEntiesColumnLbaSize,
				kGptEntiesColumnTypeUUID,
				kGptEntiesColumnPartitionUUID
			};

			enum GptHeaderRows{
				kGptHeaderRowSignature = -1,
				kGptHeaderRowRevision ,
				kGptHeaderRowCRC,
				kGptHeaderRowPrimaryLBA,
				kGptHeaderRowAlternativeLBA,
				kGptHeaderRowLastUsableLBA,
				kGptHeaderRowUuid,
				kGptHeaderRowEntryCount,
				kGptHeaderRowEntrySize,
				kGptHeaderRowEntriesCRC
			};
			enum GptMbrRows{
				kGptMbrRowMbrSignature = -1,
				kGptMbrRowBootIndicator,
				kGptMbrRowStartHead,
				kGptMbrRowStartSector,
				kGptMbrRowStartTrack,
				kGptMbrRowOsType,
				kGptMbrRowEndHead,
				kGptMbrRowEndSector,
				kGptMbrRowEndTrack,
				kGptMbrRowStartingLba,
				kGptMbrRowLbaSize,
				kGptMbrRowSignature
			};

			enum AutoWriteFormats {
				kAutoWriteFormatRawProgram = 1,
				kAutoWriteFormatLgPartitionTxt = 2,
				kAutoWriteFormatGptHeaderMeta = 3
			};

            private:
                Ui::StreamingDloadWindow *ui;
                TaskRunner taskRunner;
                volatile bool taskShouldCancel = false;
				StreamingDloadSerial port;
                serial::PortInfo currentPort;
                int taskCount = 0;
                AboutDialog aboutDialog;                
			public:
				explicit StreamingDloadWindow(QWidget *parent = 0);
				~StreamingDloadWindow();


			public slots:

				/**
				 * @brief updatePortList
				 */
				void updatePortList();

				/**
				 * @brief connectToPort
				 */
				void connectToPort();

				/**
				 * @brief disconnectPort
				 */
				void disconnectPort();

				/**
				 * @brief sendHello
				 */
				void sendHello();

				/**
				* @brief sendUnlock
				*/
				void sendUnlock();

				/**
				* @brief setSecurityMode
				*/
				void setSecurityMode();

				/**
				* @brief sendNop
				*/
				void sendNop();

				/**
				* @brief sendReset
				*/
				void sendReset();

				/**
				* @brief sendPowerDown
				*/
				void sendPowerDown();

				/**
				* @brief openMode
				*/
				void openMode();

				/**
				* @brief closeMode
				*/
				void closeMode();

				/**
				* @brief openMultiMode
				*/
				void openMultiMode();

				/**
				* @brief readEccState
				*/
				void readEccState();

				/**
				* @brief setEccState
				*/
				void setEccState();

				/**
				* @brief read - Read address and size
				*/
				void read();

				/**
				* @brief streamWrite - Stream write file starting at spcified address
				*/
				void streamWrite();

				/**
				* @brief eraseFlash
				*/
				void eraseFlash();

				/**
				* @brief browseForPartitionTable
				*/
				void browseForParitionTable();

				/**
				* @brief writePartitionTable
				*/
				void writePartitionTable();

				/**
				* @brief browseForWriteFile
				*/
				void browseForWriteFile();
            
				/**
				* @brief browseForGptFileAndParse
				*/
				void browseForGptFileAndParse();
				
				/**
				* @brief readGptFromDeviceAndParse
				*/
				void readGptFromDevice();
				
				/**
				* @brief parseReadGptFromDevice
				*/
				void parseReadGptFromDevice(QString tmpFilePath);
				
				/**
				* @brief parseGpt
				*/
				void parseGpt(QString filePath);

				/**
				* @brief showAboutDialog
				*/
                void showAboutDialog();
                				
				/**
				* @brief cancelCurrentTask
				*/
                void cancelCurrentTask();
                				
				/**
				* @brief cancelAllTasks
				*/
				void cancelAllTasks();
                				
				/**
				* @brief onTaskStarted
				*/
				void onTaskStarted();
                				
				/**
				* @brief onTaskComplete
				*/
				void onTaskComplete();
                				
				/**
				* @brief onTaskAborted
				*/
				void onTaskAborted();
                				
				/**
				* @brief onTaskError
				*/
				void onTaskError(QString msg);
                				
				/**
				* @brief onTaskLog
				*/
				void onTaskLog(QString msg);

				/**
				* @brief browseForAutoWriteMeta	
				*/
				void browseForAutoWriteMeta();

				/**
				* @brief checkAutoWriteFormat	
				*/
				void checkAutoWriteFormat();

				/**
				* @brief runAutoWrite	
				*/
				void runAutoWrite();

            protected:
				/**
				* @brief onTaskLog
				*/

                void closeEvent(QCloseEvent *event);
				/**
				* @brief onTaskLog
				*/

                void addTask(Task* task);

                void checkRawprogramXmlAutoWriteFile();

                void checkPartitionTxtAutoWriteFile();

                void runAutoWriteRawProgramXml();

                void runAutoWritePartitionTxt();

				/**
				* @brief parseRawXml
				*/
				std::vector<ResolvedRawProgramXmlEntry> parseRawProgramXml(const QString& filePath);
				
				/**
				* @brief parseRawXml
				*/
				std::vector<ResolvedPartitionTxtEntry> parsePartitionTxt(const QString& filePath);
			};
	}
}