#include "CommandLineParser.h"
#include "MetaTypeRegister.h"
#include "SignalHandler.h"
#include "controller/AppController.h"
#include "core/DeviceInfo.h"
#include "global/BuildHelper.h"
#include "global/LogHandler.h"

#include <QLoggingCategory>
#include <QSslSocket>
#include <QSysInfo>
#include <QThread>
#include <QtPlugin>

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_WINRT)
#include <QGuiApplication>
#define QAPP QGuiApplication
#else
#include <QApplication>
#define QAPP QApplication
#endif


// Includes for version API
#include <openssl/crypto.h>

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS) && !defined(Q_OS_WINRT)
Q_IMPORT_PLUGIN(PcscReaderManagerPlugIn)
Q_IMPORT_PLUGIN(WebserviceActivationHandler)
#endif

Q_IMPORT_PLUGIN(InternalActivationHandler)

#if defined(Q_OS_ANDROID)
Q_IMPORT_PLUGIN(NfcReaderManagerPlugIn)
Q_IMPORT_PLUGIN(IntentActivationHandler)
#endif

#if defined(Q_OS_IOS)
Q_IMPORT_PLUGIN(CustomSchemeActivationHandler)
#endif

#if defined(Q_OS_ANDROID) || defined(Q_OS_LINUX) || defined(Q_OS_IOS)
Q_IMPORT_PLUGIN(BluetoothReaderManagerPlugIn)
#endif


#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS) && !defined(Q_OS_WINRT)
Q_IMPORT_PLUGIN(UIPlugInCli)
Q_IMPORT_PLUGIN(UIPlugInWidgets)
#endif

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || !defined(QT_NO_DEBUG)
Q_IMPORT_PLUGIN(UIPlugInJsonApi)
Q_IMPORT_PLUGIN(UIPlugInAidl)
Q_IMPORT_PLUGIN(UIPlugInQml)
#endif

#ifndef QT_NO_DEBUG
Q_IMPORT_PLUGIN(UIPlugInWebSocket)
#endif


using namespace governikus;

Q_DECLARE_LOGGING_CATEGORY(init)


static inline void printInfo()
{
	qCDebug(init) << "Logging to" << LogHandler::getInstance();

	qCInfo(init) << "##################################################";
	qCInfo(init) << "### ApplicationName:" << QCoreApplication::applicationName();
	qCInfo(init) << "### ApplicationVersion:" << QCoreApplication::applicationVersion();
	qCInfo(init) << "### OrganizationName:" << QCoreApplication::organizationName();
	qCInfo(init) << "### OrganizationDomain:" << QCoreApplication::organizationDomain();
	qCInfo(init) << "### Build:" << BuildHelper::getDateTime();
	qCInfo(init) << "### System:" << QSysInfo::prettyProductName();
	qCInfo(init) << "### Kernel:" << QSysInfo::kernelVersion();
	qCInfo(init) << "### Architecture:" << QSysInfo::currentCpuArchitecture();
#ifdef Q_OS_ANDROID
	qCInfo(init) << "### Device:" << DeviceInfo::getPrettyInfo();
	qCInfo(init) << "### VersionCode:" << BuildHelper::getVersionCode();
#endif
	qCInfo(init) << "### Qt Version:" << qVersion();
	qCInfo(init) << "### OpenSSL Version:" << QSslSocket::sslLibraryVersionString();
	qCInfo(init) << "##################################################";

	if (QSslSocket::sslLibraryVersionString() != QLatin1String(SSLeay_version(0)))
	{
		qCWarning(init) << "Linked OpenSSL Version differs:" << SSLeay_version(0);
	}

	const auto libPathes = QCoreApplication::libraryPaths();
	for (const auto& path : libPathes)
	{
		qCDebug(init) << "Library path:" << path;
	}
}


Q_DECL_EXPORT int main(int argc, char** argv)
{
	QCoreApplication::setOrganizationName(QStringLiteral(VENDOR));
#ifdef VENDOR_DOMAIN
	QCoreApplication::setOrganizationDomain(QStringLiteral(VENDOR_DOMAIN));
#endif
	QCoreApplication::setApplicationName(QStringLiteral("AusweisApp2"));
	QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));
	QGuiApplication::setQuitOnLastWindowClosed(false);

	QAPP app(argc, argv);
	QThread::currentThread()->setObjectName(QStringLiteral("MainThread"));

	LogHandler::getInstance().init();
	SignalHandler::getInstance().init();
	CommandLineParser::getInstance().parse();
	printInfo();

	MetaTypes::registerMetaTypes();

	AppController controller;
	if (!controller.start())
	{
		qCCritical(init) << "Cannot start application controller, exit application";
		return EXIT_FAILURE;
	}

	SignalHandler::getInstance().setController(controller);
	return SignalHandler::getInstance().shouldQuit() ? EXIT_SUCCESS : app.exec();
}
