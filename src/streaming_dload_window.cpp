/**
* LICENSE PLACEHOLDER
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

#define log(m) ui->logWidget->log(m); 

StreamingDloadWindow::StreamingDloadWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StreamingDloadWindow),
	port("", 115200, 100),
	taskRunner(&taskShouldCancel)
{
	ui->setupUi(this);
	 
	ui->securityModeValue->addItem("0x01 - Trusted", kStreamingDloadSecurityModeTrusted);
	ui->securityModeValue->addItem("0x00 - Untrusted", kStreamingDloadSecurityModeUntrusted);
	ui->securityModeValue->setCurrentIndex(0);
	
	ui->openModeValue->addItem("0x01 - Bootloader Download", kStreamingDloadOpenModeBootloader);
	ui->openModeValue->addItem("0x02 - Bootable Image Download", kStreamingDloadOpenModeBootableImage);
	ui->openModeValue->addItem("0x03 - CEFS Image Download", kStreamingDloadOpenModeCefs);
	ui->openModeValue->addItem("0x04 - Factory", kStreamingDloadOpenModeFactory);
	ui->openModeValue->setCurrentIndex(0);

	ui->eccSetValue->addItem("0x00 - Disable", 0x00);
	ui->eccSetValue->addItem("0x01 - Enable", 0x01);
	ui->eccSetValue->setCurrentIndex(0);

	ui->openMultiValue->addItem("0x01 - PBL", kStreamingDloadOpenModeMultiPbl);
	ui->openMultiValue->addItem("0x02 - QC SBL Header & Config", kStreamingDloadOpenModeMultiQcsblhHdCfg);
	ui->openMultiValue->addItem("0x03 - QC SBL", kStreamingDloadOpenModeMultiQcsbl);
	ui->openMultiValue->addItem("0x04 - OEM SBL", kStreamingDloadOpenModeMultiOemsbl);
	ui->openMultiValue->addItem("0x05 - AMSS", kStreamingDloadOpenModeMultiAmss);
	ui->openMultiValue->addItem("0x06 - APPS", kStreamingDloadOpenModeMultiApps);
	ui->openMultiValue->addItem("0x07 - OBL - MSM6250", kStreamingDloadOpenModeMultiObl);
	ui->openMultiValue->addItem("0x08 - FOTA UI", kStreamingDloadOpenModeMultiFotaui);
	ui->openMultiValue->addItem("0x09 - CEFS", kStreamingDloadOpenModeMultiCefs);
	ui->openMultiValue->addItem("0x0A - AMSS applications boot loader", kStreamingDloadOpenModeMultiApps_CEFS);
	ui->openMultiValue->addItem("0x0B - Apps CEFS", kStreamingDloadOpenModeMultiFlashBin);
	ui->openMultiValue->addItem("0x0C - Flash.bin for Windows Mobile", kStreamingDloadOpenModeMultiDsp1);
	ui->openMultiValue->addItem("0x0D - DSP1 runtime image", kStreamingDloadOpenModeMultiDsp1);
	ui->openMultiValue->addItem("0x0E - User defined partition", kStreamingDloadOpenModeMultiCustom);
	ui->openMultiValue->addItem("0x0F - DBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiDbl);
	ui->openMultiValue->addItem("0x10 - OSBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiOsbl);
	ui->openMultiValue->addItem("0x11 - FSBL - Secure Boot 2.0", kStreamingDloadOpenModeMultiFsbl);
	ui->openMultiValue->addItem("0x12 - DSP2 executable ", kStreamingDloadOpenModeMultiDsp2);
	ui->openMultiValue->addItem("0x13 - Apps EFS2 raw image ", kStreamingDloadOpenModeMultiRaw);
	ui->openMultiValue->addItem("0x14 - ROFS1 - Symbian", kStreamingDloadOpenModeMultiRofs1);
	ui->openMultiValue->addItem("0x15 - ROFS2 - Symbian", kStreamingDloadOpenModeMultiRofs2);
	ui->openMultiValue->addItem("0x16 - ROFS3 - Symbian", kStreamingDloadOpenModeMultiRofs3);
	ui->openMultiValue->addItem("0x21 - EMMC USER partition ", kStreamingDloadOpenModeMultiEmmcUser);
	ui->openMultiValue->addItem("0x22 - EMMC BOOT0 partition ", kStreamingDloadOpenModeMultiEmmcBoot0);
	ui->openMultiValue->addItem("0x23 - EMMC BOOT1 partition ", kStreamingDloadOpenModeMultiEmmcBoot1);
	ui->openMultiValue->addItem("0x24 - EMMC RPMB", kStreamingDloadOpenModeMultiRpmb);
	ui->openMultiValue->addItem("0x25 - EMMC GPP1", kStreamingDloadOpenModeMultiGpp1);
	ui->openMultiValue->addItem("0x26 - EMMC GPP2", kStreamingDloadOpenModeMultiGpp2);
	ui->openMultiValue->addItem("0x27 - EMMC GPP3", kStreamingDloadOpenModeMultiGpp3);
	ui->openMultiValue->addItem("0x28 - EMMC GPP4", kStreamingDloadOpenModeMultiGpp4);
	ui->openMultiValue->setCurrentIndex(0);

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

	QObject::connect(ui->progressGroupBox->cancelButton, SIGNAL(clicked()), this, SLOT(cancelCurrentTask()));
	QObject::connect(ui->progressGroupBox->cancelAllButton, SIGNAL(clicked()), this, SLOT(cancelAllTasks()));

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
		log("Port is currently open");
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
		log("A device is already open");
		return;
	} else if (selected.compare("0") == 0 || !selected.length()) {
		log("Select to a device first");
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
		log("Port Closed");
	}
}

/**
* @brief StreamingDloadWindow::sendHello
*/
void StreamingDloadWindow::sendHello()
{

	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	if (!ui->helloMagicValue->text().length()) {
		log("Magic is required\n");
		return;
	} else if (!ui->helloVersionValue->text().length()) {
		log("Version is required\n");
		return;
	} else if (!ui->helloCompatibleVersionValue->text().length()) {
		log("Compatible version is required\n");
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
	log(tmp.sprintf("Number of Sectors: %d", port.state.hello.numberOfSectors));
	
	
	// dump all sector sizes
	for (int i = 0; i < port.state.hello.numberOfSectors; i++) {
		log(tmp.sprintf("Sector %d: %lu", i+1, port.state.hello.sectorSizes[i]));
	}

	log(tmp.sprintf("Feature Bits: %02X", port.state.hello.featureBits));

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_UNCOMPRESSED_DOWNLOAD) {
		log("Device requires an uncompressed download");
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOTABLE_IMAGE) {
		log("Device features NAND Bootable Image");
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOT_LOADER) {
		log("Device features NAND Bootloader");
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_MULTI_IMAGE) {
		log("Supports multi-image");
	}

	if (port.state.hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_SECTOR_ADDRESSES) {
		log("Device features sector addresses");
	}
}

/**
* @brief StreamingDloadWindow::setSecurityMode
*/
void StreamingDloadWindow::setSecurityMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
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
		log("Port Not Open");
		return;
	}

	if (!ui->unlockCodeValue->text().length()) {
		ui->unlockCodeValue->setText("0000");
	}

	try {
		if (port.sendUnlock(ui->unlockCodeValue->text().toStdString())) {
			log("Unlocked");
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
		log("Port Not Open");
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
		log("Port Not Open");
		return;
	}

	try {
		port.sendReset();
		log("Device Resetting");
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
		log("Port Not Open");
		return;
	}

	try {
		port.sendPowerOff();
		log("Device Powering Down");
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
		log("Port Not Open");
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
		log("Port Not Open");
		return;
	}

	try {
		if (port.closeMode()) {
			log("Mode Closed");
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
		log("Port Not Open");
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
		log("ECC Enabled");
	} else if (state == 0x00) {
		log("ECC Disabled");
	} else {
		log("Unknown ECC State");
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
		log("Port Not Open");
		return;
	}

	uint8_t state = ui->eccSetValue->currentData().toUInt();

	try {

		bool status = port.setEcc(state);

		if (status && state == 0x00) {
			log("ECC Disabled");
		} else if (status && state == 0x01) {
			log("ECC Enabled");
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
		log("Port Not Open");
		return;
	}

	uint32_t address = std::stoul(ui->readAddressValue->text().toStdString().c_str(), nullptr, 16);
	size_t	 size = std::stoi(ui->readSizeValue->text().toStdString().c_str(), nullptr, 10);

	if (size <= 0) {
		log("Enter a valid size to read");
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Read Data"), "", tr("Binary Files (*.bin)"));

	if (!fileName.length()) {
		log("Read operation cancelled");
		return;
	}

	if (taskRunner.isRunning()) {
		log("Read task put in queue");
	}

	addTask(new StreamingDloadReadTask(address, size, fileName.toStdString(), ui->progressGroupBox, port));
}

/**
* @brief streamWrite - Stream write file starting at spcified address
*/
void StreamingDloadWindow::streamWrite()
{
	if (!port.isOpen()) {
		log("Port Not Open");
		return;
	}

	uint32_t address = std::stoul(ui->writeAddressValue->text().toStdString().c_str(), nullptr, 16);
	bool unframed = ui->unframedWriteCheckbox->isChecked();

	QString tmp;
	QString filePath = ui->writeFileValue->text();

	if (!filePath.length()) {
		log("No file specified");
		return;
	}

	if (taskRunner.isRunning()) {
		log("Write task put in queue");
	}
	
	addTask(new StreamingDloadStreamWriteTask(address, filePath.toStdString(), unframed, ui->progressGroupBox, port));
}

/**
* @brief StreamingDloadWindow::openMultiMode
*/
void StreamingDloadWindow::openMultiMode()
{
	if (!port.isOpen()) {
		log("Port Not Open");
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
		log("Port Not Open");
		return;
	}

	QString fileName = ui->writePartitionTableFileValue->text();

	if (!fileName.length()) {
		log("Enter Partition File Path");
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
	QMessageBox::StandardButton confirmation = QMessageBox::question(this, "DANGEROUS OPERATION", "If this operation fails, it can make the device inoperable and only able to be restored by JTAG. Continue?");

	if (confirmation == QMessageBox::Yes) {

	}

}


/**
* @brief StreamingDloadWindow::browseForWriteFile
*/
void StreamingDloadWindow::browseForWriteFile()
{

	QString fileName = QFileDialog::getOpenFileName(this, "Browse For File", "", "Image Files (*.bin, *.mbn)");

	if (fileName.length()) {
		ui->writeFileValue->setText(fileName);
	}
}

/**
* @brief StreamingDloadWindow::disableControls

void StreamingDloadWindow::disableControls()
{
	ui->tabSet->setEnabled(false);
	ui->deviceContainer->setEnabled(false);
	ui->cancelOperationButton->setEnabled(true);
}
*/
/**
* @brief StreamingDloadWindow::enableControls

void StreamingDloadWindow::enableControls()
{
	ui->tabSet->setEnabled(true);
	ui->deviceContainer->setEnabled(true);
	ui->progressBarTextLabel2->setText("");
	ui->progressBarTextLabel->setText("");
	ui->progressBar->setValue(0);
	ui->cancelOperationButton->setEnabled(false);
}
*/


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