/*!
 * \brief Worker implementation of ReaderManger thread
 *
 * \copyright Copyright (c) 2015-2022 Governikus GmbH & Co. KG, Germany
 */

#pragma once

#include "CardConnectionWorker.h"
#include "ReaderInfo.h"
#include "ReaderManagerPlugIn.h"
#include "ReaderManagerPlugInInfo.h"

#include <QObject>

namespace governikus
{
class ReaderManagerWorker
	: public QObject
{
	Q_OBJECT

	private:
		QVector<ReaderManagerPlugIn*> mPlugIns;

		void registerPlugIns();
		[[nodiscard]] bool isPlugIn(const QJsonObject& pJson) const;
		void registerPlugIn(ReaderManagerPlugIn* pPlugIn);
		[[nodiscard]] Reader* getReader(const QString& pReaderName) const;

	public:
		ReaderManagerWorker();
		~ReaderManagerWorker() override;

		Q_INVOKABLE void shutdown();

		Q_INVOKABLE void reset(ReaderManagerPlugInType pType);
		Q_INVOKABLE void startScan(ReaderManagerPlugInType pType, bool pAutoConnect);
		Q_INVOKABLE void stopScan(ReaderManagerPlugInType pType, const QString& pError);
		Q_INVOKABLE [[nodiscard]] bool isScanRunning() const;
		Q_INVOKABLE [[nodiscard]] bool isScanRunning(ReaderManagerPlugInType pType) const;

		Q_INVOKABLE [[nodiscard]] QVector<ReaderInfo> getReaderInfos() const;
		Q_INVOKABLE void updateReaderInfo(const QString& pReaderName);
		Q_INVOKABLE void createCardConnectionWorker(const QString& pReaderName);
		Q_INVOKABLE void updateRetryCounters();

	Q_SIGNALS:
		void firePluginAdded(const ReaderManagerPlugInInfo& pInfo);
		void fireStatusChanged(const ReaderManagerPlugInInfo& pInfo);
		void fireReaderAdded(const ReaderInfo& pInfo);
		void fireReaderRemoved(const ReaderInfo& pInfo);
		void fireReaderPropertiesUpdated(const ReaderInfo& pInfo);
		void fireCardInserted(const ReaderInfo& pInfo);
		void fireCardRemoved(const ReaderInfo& pInfo);
		void fireCardRetryCounterChanged(const ReaderInfo& pInfo);
		void fireCardConnectionWorkerCreated(const QSharedPointer<CardConnectionWorker>& pCardConnectionWorker);
		void fireInitialized();

	public Q_SLOTS:
		void onThreadStarted();
};

} // namespace governikus
