/*!
 * \brief Class for retrieving informations about installed antivirus software on windows.
 *
 * \copyright Copyright (c) 2018-2022 Governikus GmbH & Co. KG, Germany
 */

#pragma once

#include <QLoggingCategory>
#include <QProcess>
#include <QSharedPointer>

class test_DiagnosisAntivirusDetection;
class test_DiagnosisTreeModel;
class test_DiagnosisModel;

namespace governikus
{

class AntivirInfo
{
	private:
		QString mDisplayName;
		QString mLastUpdate;
		QString mExePath;

	public:
		AntivirInfo(const QString& pDisplayName, const QString& pLastUpdate, const QString& pExePath);

		[[nodiscard]] const QString& getDisplayName() const
		{
			return mDisplayName;
		}


		[[nodiscard]] const QString& getLastUpdate() const
		{
			return mLastUpdate;
		}


		[[nodiscard]] const QString& getExePath() const
		{
			return mExePath;
		}


};

class DiagnosisAntivirusDetection
	: public QObject
{
	Q_OBJECT

	private:
		friend class ::test_DiagnosisAntivirusDetection;
		friend class ::test_DiagnosisTreeModel;
		friend class ::test_DiagnosisModel;

#if defined(Q_OS_WIN)
		QSharedPointer<QProcess> mProcess;
#endif
		QVector<QSharedPointer<AntivirInfo>> mAntivirInfos;
		void parseAntivirInfos(const QString& pAntivirInfos);

	private Q_SLOTS:
#if defined(Q_OS_WIN)
		void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void onError(QProcess::ProcessError pError);
#endif

	Q_SIGNALS:
		void fireAntivirusInformationChanged();
		void fireDetectionFailed();

	public:
		DiagnosisAntivirusDetection();
		void startInformationProcess();
		[[nodiscard]] const QVector<QSharedPointer<AntivirInfo>>& getAntivirusInformations() const;
};

} // namespace governikus
