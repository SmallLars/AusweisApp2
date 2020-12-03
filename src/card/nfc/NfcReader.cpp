/*!
 * \copyright Copyright (c) 2015-2020 Governikus GmbH & Co. KG, Germany
 */

#include "NfcReader.h"

#include "CardConnectionWorker.h"

#include <QLoggingCategory>

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#endif

using namespace governikus;


Q_DECLARE_LOGGING_CATEGORY(card_nfc)


Reader::CardEvent NfcReader::updateCard()
{
	return CardEvent::NONE;
}


void NfcReader::adapterStateChanged(QNearFieldManager::AdapterState pState)
{
	Q_EMIT fireNfcAdapterStateChanged(pState == QNearFieldManager::AdapterState::Online);
}


void NfcReader::targetDetected(QNearFieldTarget* pTarget)
{
	if (pTarget == nullptr)
	{
		return;
	}
	qCDebug(card_nfc) << "targetDetected, type:" << pTarget->type();

	if (!(pTarget->accessMethods() & QNearFieldTarget::TagTypeSpecificAccess))
	{
		qCDebug(card_nfc) << "The target does not provide commands";
		return;
	}

	int length = pTarget->maxCommandLength();
	mReaderInfo.setMaxApduLength(length);
	if (!mReaderInfo.sufficientApduLength())
	{
		Q_EMIT fireReaderPropertiesUpdated(mReaderInfo);
		qCDebug(card_nfc) << "ExtendedLengthApduSupport missing. MaxTransceiveLength:" << length;
	}

	mCard.reset(new NfcCard(pTarget));
	connect(mCard.data(), &NfcCard::fireSetProgressMessage, this, &NfcReader::setProgressMessage);
	QSharedPointer<CardConnectionWorker> cardConnection = createCardConnectionWorker();
	CardInfoFactory::create(cardConnection, mReaderInfo);
	//: INFO IOS Feedback when a new ID card has been detected
	mNfManager.setUserInformation(tr("ID card detected. Please do not move the device!"));
	Q_EMIT fireCardInserted(mReaderInfo);
}


void NfcReader::targetLost(QNearFieldTarget* pTarget)
{
	qCDebug(card_nfc) << "targetLost";
	if (pTarget != nullptr && mCard && mCard->invalidateTarget(pTarget))
	{
		mCard.reset();
		mReaderInfo.setCardInfo(CardInfo(CardType::NONE));
		Q_EMIT fireCardRemoved(mReaderInfo);
	}
}


void NfcReader::setProgressMessage(const QString& pMessage)
{
	mNfManager.setUserInformation(pMessage);
}


NfcReader::NfcReader()
	: ConnectableReader(ReaderManagerPlugInType::NFC, QStringLiteral("NFC"))
	, mNfManager()
{
	mReaderInfo.setBasicReader(true);
	mReaderInfo.setConnected(true);

	connect(&mNfManager, &QNearFieldManager::adapterStateChanged, this, &NfcReader::adapterStateChanged);
	connect(&mNfManager, &QNearFieldManager::targetDetectionStopped, this, &NfcReader::fireReaderDisconnected);
	connect(&mNfManager, &QNearFieldManager::targetDetected, this, &NfcReader::targetDetected);
	connect(&mNfManager, &QNearFieldManager::targetLost, this, &NfcReader::targetLost);

#if defined(Q_OS_ANDROID)
	mNfManager.startTargetDetection(QNearFieldTarget::TagTypeSpecificAccess);
	auto activity = QtAndroid::androidActivity();
	// Check if not used as SDK
	if (activity.isValid())
	{
		activity.callMethod<void>("enableNfcReaderMode");
	}
#endif
}


NfcReader::~NfcReader()
{
#if defined(Q_OS_ANDROID)
	auto activity = QtAndroid::androidActivity();
	// Check if not used as SDK
	if (activity.isValid())
	{
		activity.callMethod<void>("disableNfcReaderMode");
	}
	mNfManager.stopTargetDetection();
#endif
}


bool NfcReader::isEnabled() const
{
	return mNfManager.isEnabled();

}


Card* NfcReader::getCard() const
{
	if (mCard && mCard->isValid())
	{
		return mCard.data();
	}

	return nullptr;
}


void NfcReader::connectReader()
{
#if defined(Q_OS_IOS)
	//: INFO IOS The ID card may be inserted, the authentication process may be started.
	mNfManager.setUserInformation(tr("Please place your ID card on the top of the device's back side."));
	mNfManager.startTargetDetection(QNearFieldTarget::TagTypeSpecificAccess);
#endif
}


void NfcReader::disconnectReader(const QString& pError)
{
#if defined(Q_OS_IOS)
	if (pError.isNull())
	{
		//: INFO IOS The current session was stopped without errors.
		mNfManager.setUserInformation(tr("Scanning process has been finished successfully."));
	}
	mNfManager.stopTargetDetection(pError);
#else
	Q_UNUSED(pError)
#endif
}
