/*
 * \brief Provides UserAgent information for PAOS elements.
 *
 * \copyright Copyright (c) 2014 Governikus GmbH & Co. KG
 */

#pragma once

#include <QString>

namespace governikus
{

class UserAgent
{
	private:
		QString mMajor;
		QString mMinor;
		QString mSubminor;

	public:
		UserAgent();

		QString getName() const;
		const QString& getVersionMajor() const;
		const QString& getVersionMinor() const;
		const QString& getVersionSubminor() const;
};

}
