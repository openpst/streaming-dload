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
* @file streaming_dload_window.cpp
* @class StreamingDloadWindow
* @package OpenPST
* @brief Streaming DLOAD GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "streaming_dload_window.h"

using namespace OpenPST::GUI;
using OpenPST::GptParser;

#define log(m) ui->logWidget->log(m); 

StreamingDloadWindow::StreamingDloadWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StreamingDloadWindow),
	port("", 115200, 100),
	taskRunner(&taskShouldCancel)
{
	ui->setupUi(this);
	 
	ui->securityModeValue->addItem("Trusted", kStreamingDloadSecurityModeTrusted);
	ui->securityModeValue->addItem("Untrusted", kStreamingDloadSecurityModeUntrusted);
	ui->securityModeValue->setCurrentIndex(0);
	
	ui->openModeValue->addItem("Bootloader Download", kStreamingDloadOpenModeBootloader);
	ui->openModeValue->addItem("Bootable Image Download", kStreamingDloadOpenModeBootableImage);
	ui->openModeValue->addItem("CEFS Image Download", kStreamingDloadOpenModeCefs);
	ui->openModeValue->addItem("Factory", kStreamingDloadOpenModeFactory);
	ui->openModeValue->setCurrentIndex(0);

	ui->eccSetValue->addItem("Disable", 0x00);
	ui->eccSetValue->addItem("Enable", 0x01);
	ui->eccSetValue->setCurrentIndex(0);

	ui->openMultiValue->addItem("PBL", kStreamingDloadOpenModeMultiPbl);
	ui->openMultiValue->addItem("QC SBL Header & Config", kStreamingDloadOpenModeMultiQcsblhHdCfg);
	ui->openMultiValue->addItem("QC SBL", kStreamingDloadOpenModeMultiQcsbl);
	ui->openMultiValue->addItem("OEM SBL", kStreamingDloadOpenModeMultiOemsbl);
	ui->openMultiValue->addItem("AMSS", kStreamingDloadOpenModeMultiAmss);
	ui->openMultiValue->addItem("APPS", kStreamingDloadOpenModeMultiApps);
	ui->openMultiValue->addItem("OBL - MSM6250", kStreamingDloadOpenModeMultiObl);
	ui->openMultiValue->addItem("FOTA UI", kStreamingDloadOpenModeMultiFotaui);
	ui->openMultiValue->addItem("CEFS", kStreamingDloadOpenModeMultiCefs);
	ui->openMultiValue->addItem("AMSS applications boot loader", kStreamingDloadOpenModeMultiApps_CEFS);
	ui->openMultiValue->addItem("Apps CEFS", kStreamingDloadOpenModeMultiFlashBin);
	ui->openMultiValue->addItem("Flash.bin for Windows Mobile", kStreamingDloadOpenModeMultiDsp1);
	ui->openMultiValue->addItem("DSP1 runtime image", kStreamingDloadOpenModeMultiDsp1);
	ui->openMultiValue->addItem("User defined partition", kStreamingDloadOpenModeMultiCustom);
	ui->openMultiValue->addItem("DBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiDbl);
	ui->openMultiValue->addItem("OSBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiOsbl);
	ui->openMultiValue->addItem("FSBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiFsbl);
	ui->openMultiValue->addItem("DSP2 executable ", kStreamingDloadOpenModeMultiDsp2);
	ui->openMultiValue->addItem("Apps EFS2 raw image ", kStreamingDloadOpenModeMultiRaw);
	ui->openMultiValue->addItem("ROFS1 - Symbian", kStreamingDloadOpenModeMultiRofs1);
	ui->openMultiValue->addItem("ROFS2 - Symbian", kStreamingDloadOpenModeMultiRofs2);
	ui->openMultiValue->addItem("ROFS3 - Symbian", kStreamingDloadOpenModeMultiRofs3);
	ui->openMultiValue->addItem("EMMC USER partition ", kStreamingDloadOpenModeMultiEmmcUser);
	ui->openMultiValue->addItem("EMMC BOOT0 partition ", kStreamingDloadOpenModeMultiEmmcBoot0);
	ui->openMultiValue->addItem("EMMC BOOT1 partition ", kStreamingDloadOpenModeMultiEmmcBoot1);
	ui->openMultiValue->addItem("EMMC RPMB", kStreamingDloadOpenModeMultiRpmb);
	ui->openMultiValue->addItem("EMMC GPP1", kStreamingDloadOpenModeMultiGpp1);
	ui->openMultiValue->addItem("EMMC GPP2", kStreamingDloadOpenModeMultiGpp2);
	ui->openMultiValue->addItem("EMMC GPP3", kStreamingDloadOpenModeMultiGpp3);
	ui->openMultiValue->addItem("EMMC GPP4", kStreamingDloadOpenModeMultiGpp4);
	ui->openMultiValue->setCurrentIndex(0);

	ui->autoWriteFormatComboBox->addItem("Qualcomm rawprogram0.xml", kAutoWriteFormatRawProgram);
	ui->autoWriteFormatComboBox->addItem("LG partition.txt", kAutoWriteFormatLgPartitionTxt);
	//ui->autoWriteFormatComboBox->addItem("From GPT Header Meta", kAutoWriteFormatGptHeaderMeta);

	ui->readWriteSettingsFlashSizeValue->addItem("", 0);
	ui->readWriteSettingsFlashSizeValue->addItem("1GB", 1);
	ui->readWriteSettingsFlashSizeValue->addItem("2GB", 2);
	ui->readWriteSettingsFlashSizeValue->addItem("4GB", 4);
	ui->readWriteSettingsFlashSizeValue->addItem("8GB", 8);
	ui->readWriteSettingsFlashSizeValue->addItem("16GB", 16);
	ui->readWriteSettingsFlashSizeValue->addItem("32GB", 32);
	ui->readWriteSettingsFlashSizeValue->addItem("64GB", 64);
	ui->readWriteSettingsFlashSizeValue->addItem("128GB", 128);
	ui->readWriteSettingsFlashSizeValue->addItem("256GB", 256);
	ui->readWriteSettingsFlashSizeValue->addItem("Custom Max Sectors", 0);
	ui->readWriteSettingsFlashSizeValue->setCurrentIndex(0);

	ui->readWriteSettingsSectorSizeValue->addItem("512 bytes", 512);
	ui->readWriteSettingsSectorSizeValue->setCurrentIndex(0);

	QObject::connect(ui->portRefreshButton, SIGNAL(clicked()), this, SLOT(updatePortList()));
	QObject::connect(ui->portDisconnectButton, SIGNAL(clicked()), this, SLOT(disconnectPort()));
	QObject::connect(ui->portConnectButton, SIGNAL(clicked()), this, SLOT(connectToPort()));	
	QObject::connect(ui->helloButton, SIGNAL(clicked()), this, SLOT(sendHello()));	
	QObject::connect(ui->unlockButton, SIGNAL(clicked()), this, SLOT(sendUnlock()));
	QObject::connect(ui->securityModeButton, SIGNAL(clicked()), this, SLOT(setSecurityMode()));
	QObject::connect(ui->nopButton, SIGNAL(clicked()), this, SLOT(sendNop()));
	QObject::connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(sendReset()));
	QObject::connect(ui->powerDownButton, SIGNAL(clicked()), this, SLOT(sendPowerDown()));
	QObject::connect(ui->eccReadButton, SIGNAL(clicked()), this, SLOT(readEccState()));
	QObject::connect(ui->eccSetButton, SIGNAL(clicked()), this, SLOT(setEccState()));
	QObject::connect(ui->openModeButton, SIGNAL(clicked()), this, SLOT(openMode()));
	QObject::connect(ui->openMultiButton, SIGNAL(clicked()), this, SLOT(openMultiMode()));
	QObject::connect(ui->closeModeButton, SIGNAL(clicked()), this, SLOT(closeMode()));
	QObject::connect(ui->openMultiCloseButton, SIGNAL(clicked()), this, SLOT(closeMode()));
	QObject::connect(ui->readButton, SIGNAL(clicked()), this, SLOT(read()));
	QObject::connect(ui->writePartitionTableButton, SIGNAL(clicked()), this, SLOT(writePartitionTable()));
	QObject::connect(ui->writePartitionTableFileBrowseButton, SIGNAL(clicked()), this, SLOT(browseForParitionTable()));
	QObject::connect(ui->writeFileBrowseButton, SIGNAL(clicked()), this, SLOT(browseForWriteFile()));
	QObject::connect(ui->streamWriteButton, SIGNAL(clicked()), this, SLOT(streamWrite()));
	QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
	QObject::connect(ui->readGptFromFileButton, SIGNAL(clicked()), this, SLOT(browseForGptFileAndParse()));
	QObject::connect(ui->readGptFromDeviceButton, SIGNAL(clicked()), this, SLOT(readGptFromDevice()));
	QObject::connect(ui->progressGroupBox->cancelButton, SIGNAL(clicked()), this, SLOT(cancelCurrentTask()));
	QObject::connect(ui->progressGroupBox->cancelAllButton, SIGNAL(clicked()), this, SLOT(cancelAllTasks()));
	QObject::connect(ui->autoWriteCheckButton, SIGNAL(clicked()), this, SLOT(checkAutoWriteFormat()));
	QObject::connect(ui->autoWriteButton, SIGNAL(clicked()), this, SLOT(runAutoWrite()));
	QObject::connect(ui->autoWriteFileBrowseButton, SIGNAL(clicked()), this, SLOT(browseForAutoWriteMeta()));
	QObject::connect(ui->eraseFlashButton, SIGNAL(clicked()), this, SLOT(eraseFlash()));


	//QObject::connect(ui->readWriteSettingsFlashTypeValue, SIGNAL(textChanged(const QString&)), this, SLOT(onFlashTypeChange()));
	QObject::connect(ui->readWriteSettingsFlashSizeValue, SIGNAL(textChanged(const QString&)), this, SLOT(onFlashSizeChange()));
	QObject::connect(ui->readWriteSettingsSectorSizeValue, SIGNAL(textChanged(const QString&)), this, SLOT(onSectorSizeChange()));
	//QObject::connect(ui->readWriteSettingsMaxSectorsValue, SIGNAL(textChanged(const QString&)), this, SLOT(onMaxSectorsChange()));
	QObject::connect(ui->readWriteSettingsReadMaxSectorsFromFlashButton, SIGNAL(clicked()), this, SLOT(flashSettingsReadGptFromDevice()));
	QObject::connect(ui->readWriteSettingsReadMaxSectorsFromFileButton, SIGNAL(clicked()), this, SLOT(flashSettingsBrowseForGptFileAndParse()));



	updatePortList();
}

StreamingDloadWindow::~StreamingDloadWindow()
{
	if (port.isOpen()) {
		port.close();
	}

	delete ui;
}

void StreamingDloadWindow::updatePortList()
{
	if (port.isOpen()) {
		log(tr("Port is currently open"));
		return;
	}

	std::vector<serial::PortInfo> devices = serial::list_ports();

	ui->portList->clear();
	ui->portList->addItem("- Select a Port -");

	QString tmp;

	log(tmp.sprintf("Found %d devices", devices.size()));

	for (auto device : serial::list_ports())  {

		log(tmp.sprintf("%s %s %s",
			device.port.c_str(),
			device.hardware_id.c_str(),
			device.description.c_str()
		));

		ui->portList->addItem(tmp, device.port.c_str());
	}
}

/**
* @brief StreamingDloadWindow::connectToPort
*/
void StreamingDloadWindow::connectToPort()
{
	serial::PortInfo device;
	QString selected = ui->portList->currentData().toString();
	QString tmp;

	if (port.isOpen()) {
		log(tr("A device is already open"));
		return;
	} else if (selected.compare("0") == 0 || !selected.length()) {
		log(tr("Select to a device first"));
		return;
	}

	for (auto device : serial::list_ports())  {
		if (selected.compare(device.port.c_str(), Qt::CaseInsensitive) == 0) {
			try {
				port.setPort(device.port);

				if (!port.isOpen()){
					port.open();
				}
				
				log(tmp.sprintf("Connected to %s", device.port.c_str()));

				ui->portDisconnectButton->setEnabled(true);

				if (ui->autoHelloCheckbox->isChecked()) {
					sendHello();
				}			

			} catch (SerialError e) {
				log(tmp.sprintf("Error connecting to device on %s", device.port.c_str()));
				log(e.getCode() == 13 ? "Permission Denied" : e.what());
				return;
			}
			break;
		}
	}
}

/**
* @brief StreamingDloadWindow::disconnectPort
*/
void StreamingDloadWindow::disconnectPort()
{
	if (port.isOpen()) {
		port.close();
		log(tr("Port Closed"));
	}
}

/**
* @brief StreamingDloadWindow::sendHello
*/
void StreamingDloadWindow::sendHello()
{

	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	if (!ui->helloMagicValue->text().length()) {
		log(tr("Magic is required"));
		return;
	} else if (!ui->helloVersionValue->text().length()) {
		log(tr("Version is required"));
		return;
	} else if (!ui->helloCompatibleVersionValue->text().length()) {
		log(tr("Compatible version is required"));
		return;
	}
	
	QString tmp;
	
	std::string magic = ui->helloMagicValue->text().toStdString().c_str();
	uint8_t version = std::stoul(ui->helloVersionValue->text().toStdString().c_str(), nullptr, 16);
	uint8_t compatibleVersion = std::stoul(ui->helloCompatibleVersionValue->text().toStdString().c_str(), nullptr, 16);

	try {
		port.sendHello(magic, version, compatibleVersion, STREAMING_DLOAD_FEATURE_ALL);
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return disconnectPort();
	} catch(SerialError& e) {
		log(e.what());
		return disconnectPort();
	}

	log(tmp.sprintf("Hello Response: %02X", port.state.hello.command));
	log(tmp.sprintf("Magic: %s", port.state.hello.magic));
	log(tmp.sprintf("Version: %02X", port.state.hello.version));
	log(tmp.sprintf("Compatible Version: %02X", port.state.hello.compatibleVersion));
	log(tmp.sprintf("Max Prefered Block Size: %d", port.state.hello.maxPreferredBlockSize));
	log(tmp.sprintf("Base Flash Address: 0x%08X", port.state.hello.baseFlashAddress));
	log(tmp.sprintf("Flash ID Length: %d", port.state.hello.flashIdLength));	
	log(tmp.sprintf("Flash Identifier: %s", port.state.hello.flashIdenfier));
	log(tmp.sprintf("Window Size: %d", port.state.hello.windowSize));

	log(tmp.sprintf("Feature Bits: %02X", port.state.hello.featureBits));

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_UNCOMPRESSED_DOWNLOAD) {
		log(tr("Device requires an uncompressed download"));
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOTABLE_IMAGE) {
		log(tr("Device features NAND Bootable Image"));
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOT_LOADER) {
		log(tr("Device features NAND Bootloader"));
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_MULTI_IMAGE) {
		log(tr("Supports multi-image"));
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_SECTOR_ADDRESSES) {
		log(tr("Device features sector addresses"));
	}
}

/**
* @brief StreamingDloadWindow::setSecurityMode
*/
void StreamingDloadWindow::setSecurityMode()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}
	
	QString tmp;
	bool result;
	uint8_t mode = ui->securityModeValue->currentData().toUInt();
	
	try {
		result = port.setSecurityMode(mode);
	} catch (StreamingDloadSerialError& e) {
		log(tmp.sprintf("Error setting security mode to %s - %s", (mode == 0x00 ? "Untrusted" : "Trusted"), e.what() ));
		return;
	} catch (SerialError& e) {
		log(tmp.sprintf("Error setting security mode to %s - %s", (mode == 0x00 ? "Untrusted" : "Trusted"), e.what() ));
		return;
	}

	if (result) {
		log(tmp.sprintf("Security mode set to %s", mode == 0x00 ? "Untrusted" : "Trusted"));
	} else {
		log(tmp.sprintf("Error setting security mode to %s", mode == 0x00 ? "Untrusted" : "Trusted"));
	}	
}

/**
* @brief StreamingDloadWindow::sendUnlock
*/
void StreamingDloadWindow::sendUnlock()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	if (!ui->unlockCodeValue->text().length()) {
		ui->unlockCodeValue->setText("0000");
	}

	try {
		if (port.sendUnlock(ui->unlockCodeValue->text().toStdString())) {
			log(tr("Unlocked"));
		}
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}


/**
* @brief sendNop
*/
void StreamingDloadWindow::sendNop()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	try {
		port.sendNop();
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}

/**
* @brief sendReset
*/
void StreamingDloadWindow::sendReset()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	try {
		port.sendReset();
		log(tr("Device Resetting"));
		port.close();
		
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}

/**
* @brief sendPowerDown
*/
void StreamingDloadWindow::sendPowerDown()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	try {
		port.sendPowerOff();
		log(tr("Device Powering Down"));
		port.close();		
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}

/**
* @brief closeMode
*/
void StreamingDloadWindow::openMode()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	QString tmp;
	uint8_t mode = ui->openModeValue->currentData().toUInt();

	try {
		if (port.openMode(mode)) {
			log(tmp.sprintf("Opened Mode %s", port.getNamedOpenMode(mode)));
		}
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}


/**
* @brief closeMode
*/
void StreamingDloadWindow::closeMode()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	try {
		if (port.closeMode()) {
			log(tr("Mode Closed"));
		}
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}

/**
* @brief readEccState
*/
void StreamingDloadWindow::readEccState()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	uint8_t state = 0x00;

	try {
		state = port.readEcc();
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}

	if (state == 0x01) {
		log(tr("ECC Enabled"));
	} else if (state == 0x00) {
		log(tr("ECC Disabled"));
	} else {
		log(tr("Unknown ECC State"));
	}

	// set the ecc set choice box value to the matching
	// state
	int choiceIdx = ui->eccSetValue->findData(state);
	if (choiceIdx) {
		ui->eccSetValue->setCurrentIndex(1);
	}
}

/**
* @brief setEccState
*/
void StreamingDloadWindow::setEccState()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	uint8_t state = ui->eccSetValue->currentData().toUInt();

	try {

		bool status = port.setEcc(state);

		if (status && state == 0x00) {
			log(tr("ECC Disabled"));
		} else if (status && state == 0x01) {
			log(tr("ECC Enabled"));
		}

	} catch (StreamingDloadSerialError& e) {
		log(e.what());
		return;
	} catch (SerialError& e) {
		log(e.what());
		return;
	}
}


/**
* @brief StreamingDloadWindow::read - Read address and size
*/
void StreamingDloadWindow::read()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	uint32_t address = std::stoul(ui->readAddressValue->text().toStdString().c_str(), nullptr, 16);
	size_t	 size = std::stoi(ui->readSizeValue->text().toStdString().c_str(), nullptr, 10);

	if (size <= 0) {
		log(tr("Enter a valid size to read"));
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Read Data"), "", tr("Binary Files (*.bin)"));

	if (!fileName.length()) {
		log(tr("Read operation cancelled"));
		return;
	}

	if (taskRunner.isRunning()) {
		log(tr("Read task put in queue"));
	}

	addTask(new StreamingDloadReadTask(address, size, fileName.toStdString(), ui->progressGroupBox, port));
}

/**
* @brief streamWrite - Stream write file starting at spcified address
*/
void StreamingDloadWindow::streamWrite()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	uint32_t address = std::stoul(ui->writeAddressValue->text().toStdString().c_str(), nullptr, 16);
	bool unframed = ui->unframedWriteCheckbox->isChecked();

	QString tmp;
	QString filePath = ui->writeFileValue->text();

	if (!filePath.length()) {
		log(tr("No file specified"));
		return;
	}

	if (taskRunner.isRunning()) {
		log(tr("Write task put in queue"));
	}
	
	addTask(new StreamingDloadStreamWriteTask(address, filePath.toStdString(), unframed, ui->progressGroupBox, port));
}

/**
* @brief StreamingDloadWindow::openMultiMode
*/
void StreamingDloadWindow::openMultiMode()
{
	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	uint8_t imageType = ui->openMultiValue->currentData().toUInt();
	QString tmp;
	
	try {
		if (port.openMultiImage(imageType)) {
			log(tmp.sprintf("Opening multi image mode for %s", port.getNamedMultiImage(imageType).c_str()));
		} else {
			log(tmp.sprintf("Error opening multi image mode for %s", port.getNamedMultiImage(imageType).c_str()));
		}
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
	} catch(SerialError& e) {
		log(e.what());
	}
}

/**
* @brief StreamingDloadWindow::browseForParitionTable
*/
void StreamingDloadWindow::browseForParitionTable()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Partition Image To Send", "", "Partition Image Files (*.bin)");

	if (fileName.length()) {
		ui->writePartitionTableFileValue->setText(fileName);
	}
}

/**
* @brief StreamingDloadWindow::writePartitionTable
*/
void StreamingDloadWindow::writePartitionTable()
{

	if (!port.isOpen()) {
		log(tr("Port Not Open"));
		return;
	}

	QString fileName = ui->writePartitionTableFileValue->text();

	if (!fileName.length()) {
		log(tr("Enter Partition File Path"));
		return;
	}

	bool overwrite = ui->writeParitionTableOverrideExistingCheckbox->isChecked();
	uint8_t status = NULL;
	QString tmp;

	try {
		status = port.writePartitionTable(fileName.toStdString(), overwrite);
		log(tmp.sprintf("Partition Table Response: %02X", status));
	} catch (StreamingDloadSerialError& e) {
		log(e.what());
	} catch(SerialError& e) {
		log(e.what());
	}	
}


void StreamingDloadWindow::eraseFlash()
{
	QMessageBox::StandardButton confirmation = QMessageBox::question(this, tr("DANGEROUS OPERATION"), 
		tr("If this operation fails, it can make the device inoperable and only able to be restored by JTAG. Continue?"));

	if (confirmation == QMessageBox::Yes) {
		try {
			port.eraseFlash();
		} catch (StreamingDloadSerialError& e) {
			log(e.what());
		} catch(SerialError& e) {
			log(e.what());
		}	
	}
}


/**
* @brief StreamingDloadWindow::browseForWriteFile
*/
void StreamingDloadWindow::browseForWriteFile()
{

	QString fileName = QFileDialog::getOpenFileName(this, tr("Browse For File"), "", tr("Image Files (*.bin, *.mbn)"));

	if (fileName.length()) {
		ui->writeFileValue->setText(fileName);
	}
}

/**
* @brief StreamingDloadWindow::browseForGptFileAndParse
*/
void StreamingDloadWindow::browseForGptFileAndParse()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Browse For File"), "", "*.*");
	
	if (fileName.length()) {
		parseGpt(fileName);
	}
}

void StreamingDloadWindow::parseGpt(QString filePath)
{
	GptParser parser;
	GptInfo gpt = {};
	QString tmp;

	int flags = kGptParserFlagAll;

	if (!filePath.length()) {
		return;
	}

	try {
		gpt = parser.parse(filePath.toStdString(), flags);
	} catch (std::out_of_range& e) {
		log(e.what());
		return;
	} catch(std::invalid_argument& e) {
		log(e.what());
		return;
	} catch (...) {
		log(tr("Unexpected error encountered"));
		return;
	}
	
	ui->gptMbrInfoTable->setItem(kGptMbrRowMbrSignature, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.mbr.mbrSignature)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowBootIndicator, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].bootIndicator)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowStartHead, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].startHead)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowStartSector, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].startSector)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowStartTrack, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].startTrack)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowOsType, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].osType)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowEndHead, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].endHead)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowEndSector, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].endSector)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowEndTrack, 1, new QTableWidgetItem(tmp.sprintf("0x%02X", gpt.mbr.partition[0].endTrack)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowStartingLba, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.mbr.partition[0].startingLba)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowLbaSize, 1, new QTableWidgetItem(tmp.sprintf("%lu (0x%08X", gpt.mbr.partition[0].lbaSize, gpt.mbr.partition[0].lbaSize)));
	ui->gptMbrInfoTable->setItem(kGptMbrRowSignature, 1, new QTableWidgetItem(tmp.sprintf("0x%04X", gpt.mbr.signature)));

	ui->gptHeaderInfoTable->setItem(kGptHeaderRowSignature, 1, new QTableWidgetItem(tmp.sprintf("%s", &gpt.header.signature[0])));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowRevision, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.revision)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowCRC, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.headerCrc32)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowPrimaryLBA, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.myLba)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowAlternativeLBA, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.alternateLba)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowLastUsableLBA, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.lastUsableLba)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowUuid, 1, new QTableWidgetItem(tmp.sprintf("%s", parser.getUUID(&gpt.header.guid).c_str())));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowEntryCount, 1, new QTableWidgetItem(tmp.sprintf("%d", gpt.header.numPartitionEntries)));	
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowEntrySize, 1, new QTableWidgetItem(tmp.sprintf("%lu", gpt.header.sizeofPartitionEntry)));
	ui->gptHeaderInfoTable->setItem(kGptHeaderRowEntriesCRC, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.partitionEntryArrayCrc32)));

	if (gpt.entries.size()) {
		ui->gptEntriesInfoTable->setRowCount(gpt.entries.size());
		int i = 0;
		for (auto &entry : gpt.entries) {
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnName, new QTableWidgetItem(tmp.sprintf("%s", parser.getEntryName(&entry).c_str())));
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnStartLBA, new QTableWidgetItem(tmp.sprintf("0x%08X", entry.startingLba)));
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnEndLBA, new QTableWidgetItem(tmp.sprintf("0x%08X", entry.endingLba)));
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnLbaSize, new QTableWidgetItem(tmp.sprintf("%d", (entry.endingLba - entry.startingLba))));
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnTypeUUID, new QTableWidgetItem(tmp.sprintf("%s", parser.getUUID(&entry.typeUuid).c_str())));
			ui->gptEntriesInfoTable->setItem(i, kGptEntiesColumnPartitionUUID, new QTableWidgetItem(tmp.sprintf("%s", parser.getUUID(&entry.partitionUuid).c_str())));
			i++;
		}

	} else {
		log(tr("No Entries To Parse"));
	}
}

void StreamingDloadWindow::readGptFromDevice()
{
	QString savePath = QFileDialog::getSaveFileName(this, tr("Save GPT data to file"), "", tr("Any Files (*.*)"));

	if (!savePath.length()) {
		log(tr("Aborted read GPT from device"));
		return;
	}

	StreamingDloadReadGptTask* task = new StreamingDloadReadGptTask(savePath, ui->progressGroupBox, port);

	connect(task, &StreamingDloadReadGptTask::received, this, &StreamingDloadWindow::parseReadGptFromDevice);
	
	addTask(task);
}

void StreamingDloadWindow::parseReadGptFromDevice(QString filePath) {
	parseGpt(filePath);
}

void StreamingDloadWindow::addTask(Task* task)
{
	connect(task, &Task::started,   this, &StreamingDloadWindow::onTaskStarted);
	connect(task, &Task::complete,  this, &StreamingDloadWindow::onTaskComplete);
	connect(task, &Task::aborted,   this, &StreamingDloadWindow::onTaskAborted);
	connect(task, &Task::error,		this, &StreamingDloadWindow::onTaskError);
	connect(task, &Task::log,		this, &StreamingDloadWindow::onTaskLog);

	ui->progressGroupBox->setTaskCount(++taskCount);

	taskRunner.queue(task);

}

void StreamingDloadWindow::cancelCurrentTask()
{
	QMessageBox::StandardButton answer = QMessageBox::question(
		this,
		tr("Confirmation"),
		tr("Are you sure you would like to cancel the current task?")
	);

	if (answer == QMessageBox::No) {
		return;
	}

	taskShouldCancel = true;
	taskRunner.waitForDone();
}

void StreamingDloadWindow::cancelAllTasks()
{
	QMessageBox::StandardButton answer = QMessageBox::question(
		this,
		tr("Confirmation"),
		tr("Are you sure you would like to cancel all tasks?")
	);

	if (answer == QMessageBox::No) {
		return;
	}

	taskRunner.clearQueue();
	cancelCurrentTask();
	taskCount = 0;
	ui->progressGroupBox->disableCancel();
	ui->progressGroupBox->disableCancelAll();
	ui->progressGroupBox->setTaskCount(taskCount);
}

void StreamingDloadWindow::onTaskStarted()
{
	ui->progressGroupBox->enableCancel();

	if (taskCount > 1) {
		ui->progressGroupBox->enableCancelAll();
	}
}

void StreamingDloadWindow::onTaskComplete()
{
	QString tmp;

	if (taskCount > 0) {
		taskCount--;
	}

	ui->progressGroupBox->setTaskCount(taskCount);

	if (!taskCount) {
		ui->progressGroupBox->disableCancel();
		ui->progressGroupBox->disableCancelAll();
	}	
}

void StreamingDloadWindow::onTaskAborted()
{
	QString tmp;

	if (taskCount > 0) {
		taskCount--;
	}

	ui->progressGroupBox->setTaskCount(taskCount);

	if (!taskCount) {
		ui->progressGroupBox->disableCancel();
		ui->progressGroupBox->disableCancelAll();
	}
}

void StreamingDloadWindow::onTaskError(QString msg)
{
	QString tmp; 
	
	if (taskCount > 0) {
		taskCount--;
	}
	
	ui->progressGroupBox->setTaskCount(taskCount);

	if (!taskCount) {
		ui->progressGroupBox->disableCancel();
		ui->progressGroupBox->disableCancelAll();
	}

	log(msg);
}

void StreamingDloadWindow::onTaskLog(QString msg)
{
	log(msg);
}

void StreamingDloadWindow::showAboutDialog()
{
	aboutDialog.exec();
}

void StreamingDloadWindow::closeEvent(QCloseEvent *event)
{
	
	if (taskRunner.isRunning()) {
		QMessageBox::StandardButton answer = QMessageBox::question(
			this,
			tr("Active Tasks Running"),
			tr("There are active tasks running. Are you sure?")
		);

		if (answer == QMessageBox::No) {
			event->ignore();
			return;
		}
	}

	event->accept();
}

void StreamingDloadWindow::browseForAutoWriteMeta()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select file"), "", tr("*.*"));

	if (fileName.length()) {
		ui->autoWriteFileValue->setText(fileName);
	}
}

std::vector<ResolvedRawProgramXmlEntry> StreamingDloadWindow::parseRawProgramXml(const QString& filePath)
{
	QString tmp;
	RawProgramXmlReader reader;
	std::vector<RawProgramXmlEntry> entries;
	std::vector<ResolvedRawProgramXmlEntry> ret;

	int numberOfSectors = 128;
	if (port.state.negotiated && port.state.hello.numberOfSectors){
		log(tmp.sprintf("Hello negotiation says there are %d sectors.", port.state.hello.numberOfSectors));
		numberOfSectors = port.state.hello.numberOfSectors;
	} else {
		log(tmp.sprintf("Hello has not been established. Parsing with assumed %d sectors.", numberOfSectors));
	}

	entries = reader.parse(filePath.toStdString(), numberOfSectors);
	
	if (!entries.size()) {
		log(tr("No entries found referenced in xml document"));
		return ret;
	}

	log(tmp.sprintf("Found %lu entries referenced in xml document", entries.size()));

	QFileInfo xmlFileInfo(filePath);
	QDir xmlFileDir 	= xmlFileInfo.dir();
	QDir applicationDir = QDir::current();

	log(tr("Searching the following directories for relative images:"));
	log(tr("\t- Specified absolute path to image"));
	log("\t- " + xmlFileDir.path());
	log("\t- " + applicationDir.path());

	for (auto &entry : entries) {
		ResolvedRawProgramXmlEntry e;

		e.entry 	 = entry;
		e.sourceFile = filePath;
		
		QFileInfo absoluteInfo(entry.fileName.c_str());
	
		if (absoluteInfo.exists()) {
			e.path = absoluteInfo.path();
		} else if (xmlFileDir.exists(entry.fileName.c_str())) {
			e.path = xmlFileDir.filePath(entry.fileName.c_str());
		} else if(applicationDir.exists(entry.fileName.c_str())) {
			e.path = applicationDir.filePath(entry.fileName.c_str());
		} else {
			e.path = "";
		}

		ret.push_back(e);
	}

	return ret;

	

}

std::vector<ResolvedPartitionTxtEntry> StreamingDloadWindow::parsePartitionTxt(const QString& filePath)
{
	
	QString tmp;
	PartitionTxtReader reader;
	std::vector<PartitionTxtEntry> entries;
	std::vector<ResolvedPartitionTxtEntry> ret;

	entries = reader.parse(filePath.toStdString());
	
	if (!entries.size()) {
		log(tr("No entries found referenced in xml document"));
		return ret;
	}

	log(tmp.sprintf("Found %lu entries referenced in file", entries.size()));

	QFileInfo fileInfo(filePath);
	QDir fileDir 	= fileInfo.dir();
	QDir applicationDir = QDir::current();

	log(tr("Searching the following directories for relative images:"));
	log(tr("\t- Specified absolute path to image"));
	log("\t- " + fileDir.path());
	log("\t- " + applicationDir.path());

	for (auto &entry : entries) {
		ResolvedPartitionTxtEntry e;

		e.entry 	 = entry;
		e.sourceFile = filePath;
		
		QString entryFile = entry.file;

		QFileInfo absoluteInfo(entryFile);
	
		if (absoluteInfo.exists()) {
			e.path = absoluteInfo.path();
		} else if (fileDir.exists(entryFile)) {
			e.path = fileDir.filePath(entryFile);
		} else if(applicationDir.exists(entryFile)) {
			e.path = applicationDir.filePath(entryFile);
		} else {
			e.path = "";
		}

		ret.push_back(e);
	}

	return ret;
}

void StreamingDloadWindow::checkAutoWriteFormat()
{
	
	QString tmp;

	int format = ui->autoWriteFormatComboBox->currentData().toInt();
	
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}

	if (format == kAutoWriteFormatRawProgram) {
		checkRawprogramXmlAutoWriteFile();
	} else if (format == kAutoWriteFormatLgPartitionTxt) {
		checkPartitionTxtAutoWriteFile();
	} else {
		log(tr("Unsupported Format"));
	}

}

void StreamingDloadWindow::runAutoWrite()
{
	QString tmp;

	int format = ui->autoWriteFormatComboBox->currentData().toInt();
	
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}

	if (format == kAutoWriteFormatRawProgram) {
		runAutoWriteRawProgramXml();
	} else if (format == kAutoWriteFormatLgPartitionTxt) {
		runAutoWritePartitionTxt();
	} else {
		log(tr("Unsupported Format"));
	}
}

void StreamingDloadWindow::checkRawprogramXmlAutoWriteFile()
{
	
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}

	QString tmp;
	std::vector<ResolvedRawProgramXmlEntry> entries;

	try {
		entries = parseRawProgramXml(ui->autoWriteFileValue->text());
	} catch(RawProgramXmlReaderError& e) {
		log(e.what());
		return;
	} catch(...) {
		log(tr("Unexpected Error"));
		return;
	}

	if (!entries.size()) {
		log(tr("No entries found in file"));
	}

	TableDialog dialog;

	dialog.setTitle("Raw Program XML Overview for " + ui->autoWriteFileValue->text());
	dialog.setWindowTitle(tr("Raw Program XML Overview"));

	QTableWidget* table = dialog.getTableWidget();

	table->setColumnCount(15);
	table->setHorizontalHeaderItem(0,  new QTableWidgetItem(tr("Label")));
	table->setHorizontalHeaderItem(1,  new QTableWidgetItem(tr("File Name")));
	table->setHorizontalHeaderItem(2,  new QTableWidgetItem(tr("Resolved Path")));
	table->setHorizontalHeaderItem(3,  new QTableWidgetItem(tr("Sector Size")));
	table->setHorizontalHeaderItem(4,  new QTableWidgetItem(tr("File Sector Offset")));
	table->setHorizontalHeaderItem(5,  new QTableWidgetItem(tr("# Partition Sectors")));
	table->setHorizontalHeaderItem(6,  new QTableWidgetItem(tr("Part of Single Image")));
	table->setHorizontalHeaderItem(7,  new QTableWidgetItem(tr("Physical Partition Number")));
	table->setHorizontalHeaderItem(8,  new QTableWidgetItem(tr("Read Back Verify")));
	table->setHorizontalHeaderItem(9,  new QTableWidgetItem(tr("Sparse")));
	table->setHorizontalHeaderItem(10, new QTableWidgetItem(tr("Start Byte")));
	table->setHorizontalHeaderItem(11, new QTableWidgetItem(tr("Start Byte HEX")));
	table->setHorizontalHeaderItem(12, new QTableWidgetItem(tr("Start Sector")));
	table->setHorizontalHeaderItem(13, new QTableWidgetItem(tr("Size")));
	table->setHorizontalHeaderItem(14, new QTableWidgetItem(tr("Size in KB")));

	table->setRowCount(entries.size());

	int row = 0;
	for (auto &entry : entries) {
		table->setItem(row, 0, new QTableWidgetItem(tmp.sprintf("%s", entry.entry.label.c_str())));
		table->setItem(row, 1, new QTableWidgetItem(tmp.sprintf("%s", entry.entry.fileName.c_str())));
		if (entry.path.length()) {
			table->setItem(row, 2, new QTableWidgetItem(entry.path));
		} else {
			table->setItem(row, 2, new QTableWidgetItem(tr("Not Found")));
		}
		
		table->setItem(row, 3, new QTableWidgetItem(tmp.sprintf("%lu", entry.entry.sectorSize)));
		table->setItem(row, 4, new QTableWidgetItem(tmp.sprintf("%lu", entry.entry.fileSectorOffset)));
		table->setItem(row, 5, new QTableWidgetItem(tmp.sprintf("%d", entry.entry.numPartitionSectors)));
		table->setItem(row, 6, new QTableWidgetItem(entry.entry.partOfSingleImage ? tr("Yes") : tr("No")));
		table->setItem(row, 7, new QTableWidgetItem(tmp.sprintf("%d", entry.entry.physicalPartitionNumber)));
		table->setItem(row, 8, new QTableWidgetItem(entry.entry.readBackVerify ? tr("Yes") : tr("No")));
		table->setItem(row, 9, new QTableWidgetItem(entry.entry.sparse ? tr("Yes") : tr("No")));
		table->setItem(row, 10, new QTableWidgetItem(tmp.sprintf("%d", entry.entry.startByte)));
		table->setItem(row, 11, new QTableWidgetItem(tmp.sprintf("0x%08X", entry.entry.startByte)));
		table->setItem(row, 12, new QTableWidgetItem(tmp.sprintf("%d", entry.entry.startSector)));
		table->setItem(row, 13, new QTableWidgetItem(tmp.sprintf("%d", entry.entry.size)));
		table->setItem(row, 14, new QTableWidgetItem(tmp.sprintf("%f", entry.entry.sizeInKb)));

		row++;
	}

	dialog.exec();
}

void StreamingDloadWindow::checkPartitionTxtAutoWriteFile()
{
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}

	QString tmp;
	std::vector<ResolvedPartitionTxtEntry> entries;

	try {
		entries = parsePartitionTxt(ui->autoWriteFileValue->text());
	} catch(PartitionTxtReaderError& e) {
		log(e.what());
		return;
	} catch(...) {
		log(tr("Unexpected Error"));
		return;
	}

	if (!entries.size()) {
		log(tr("No entries found in file"));
	}

	TableDialog dialog;

	dialog.setTitle("LG partition.txt Overview for " + ui->autoWriteFileValue->text());
	dialog.setWindowTitle(tr("LG partition.txt Overview"));

	QTableWidget* table = dialog.getTableWidget();

	table->setColumnCount(5);
	table->setHorizontalHeaderItem(0,  new QTableWidgetItem(tr("LBA")));
	table->setHorizontalHeaderItem(1,  new QTableWidgetItem(tr("Sectors in LBA")));
	table->setHorizontalHeaderItem(2,  new QTableWidgetItem(tr("Resolved Path")));
	table->setHorizontalHeaderItem(3,  new QTableWidgetItem(tr("Name")));
	table->setHorizontalHeaderItem(4,  new QTableWidgetItem(tr("File")));

	table->setRowCount(entries.size());


	int row = 0;
	for (auto &entry : entries) {
		table->setItem(row, 0, new QTableWidgetItem(tmp.sprintf("%08X", entry.entry.lba)));
		table->setItem(row, 1, new QTableWidgetItem(tmp.sprintf("%lu", entry.entry.sectors)));
		table->setItem(row, 2, new QTableWidgetItem(tmp.sprintf("%s", entry.path.toStdString().c_str())));
		table->setItem(row, 3, new QTableWidgetItem(tmp.sprintf("%s", &entry.entry.name)));
		table->setItem(row, 4, new QTableWidgetItem(tmp.sprintf("%s", &entry.entry.file)));
		row++;
	}

	dialog.exec();
}


void StreamingDloadWindow::runAutoWriteRawProgramXml()
{
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}
}

void StreamingDloadWindow::runAutoWritePartitionTxt()
{
	if (!ui->autoWriteFileValue->text().length()) {
		log(tr("Enter or browse for a valid file"));
		return;
	}

	QString tmp;
	std::vector<ResolvedPartitionTxtEntry> entries;

	try {
		entries = parsePartitionTxt(ui->autoWriteFileValue->text());
	} catch(PartitionTxtReaderError& e) {
		log(e.what());
		return;
	} catch(...) {
		log(tr("Unexpected Error"));
		return;
	}

	if (!entries.size()) {
		log(tr("No entries found in file"));
	}

	for (auto &entry : entries) {
		std::string name = entry.entry.name;
		if (name.compare("system") == 0 || 
			name.compare("cache") == 0 || 
			name.compare("userdata") == 0 ||
			name.compare("misc") == 0 ||
			name.compare("persist") == 0 ||
			name.compare("modem") == 0 
		) {
			continue;
		}

		if (taskRunner.isRunning()) {
			log(tmp.sprintf("Queued Writing %s to LBA 0x%08X", entry.path.toStdString().c_str(), entry.entry.lba));
		} else {
			log(tmp.sprintf("Writing %s to LBA 0x%08X", entry.path.toStdString().c_str(), entry.entry.lba));
		}
		

		addTask(new StreamingDloadStreamWriteTask(entry.entry.lba, entry.path.toStdString(), false, ui->progressGroupBox, port));
	}
}

void StreamingDloadWindow::onFlashSizeChange()
{
	int flashSizeSelection  = ui->readWriteSettingsFlashSizeValue->currentData().toInt();
	int blockSizeValue 	    = ui->readWriteSettingsSectorSizeValue->currentData().toInt();
	int lastLba 			= 0;

	// IDEMA Standard LBA 1-03 specification
	// http://www.idema.org/wp-content/plugins/download-monitor/download.php?id=1223
	if (flashSizeSelection) {
		if (blockSizeValue == 4096) {
			lastLba = (12212046 + (244188 * (flashSizeSelection - 50)));
		} else {
			lastLba = (97696368 + (1953504 * (flashSizeSelection - 50)));
		}
	}

	ui->readWriteSettingsMaxSectorsValue->setText(std::to_string(lastLba).c_str());
}

void StreamingDloadWindow::onSectorSizeChange()
{

}

void StreamingDloadWindow::onMaxSectorsChange()
{

}

void StreamingDloadWindow::flashSettingsReadGptFromDevice()
{

}

void StreamingDloadWindow::flashSettingsBrowseForGptFileAndParse()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Browse For File"), "", "*.*");
	
	if (fileName.length()) {
		parseGpt(fileName);
		if (ui->gptHeaderInfoTable->rowCount() > 0) {
			QTableWidgetItem* lastUsableLbaRow = ui->gptHeaderInfoTable->item(kGptHeaderRowLastUsableLBA, 1);
			if (lastUsableLbaRow) {
				log(lastUsableLbaRow->text());
				uint32_t lastUsableLba = std::strtoul(lastUsableLbaRow->text().toStdString().c_str(), nullptr, 16);
				ui->readWriteSettingsFlashSizeValue->setCurrentIndex(10);
				ui->readWriteSettingsMaxSectorsValue->setText(std::to_string(lastUsableLba).c_str());
			}
		}
	}
	//ui->gptHeaderInfoTable->setItem(kGptHeaderRowLastUsableLBA, 1, new QTableWidgetItem(tmp.sprintf("0x%08X", gpt.header.lastUsableLba)));

}
