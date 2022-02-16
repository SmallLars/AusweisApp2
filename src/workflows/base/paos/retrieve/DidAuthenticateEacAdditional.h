/*!
 * \brief Class to hold information of DIDAuthenticateEACAdditional.
 *
 * \copyright Copyright (c) 2014-2022 Governikus GmbH & Co. KG, Germany
 */

#pragma once

#include "paos/element/ConnectionHandle.h"
#include "paos/PaosMessage.h"

#include <QString>

namespace governikus
{

class DIDAuthenticateEACAdditional
	: public PaosMessage
{
	friend class DidAuthenticateEacAdditionalParser;

	private:
		ConnectionHandle mConnectionHandle;
		QString mDidName;
		QString mSignature;

		void setConnectionHandle(const ConnectionHandle& pConnectionHandle);
		void setDidName(const QString& didName);
		void setSignature(const QString& signature);

	public:
		DIDAuthenticateEACAdditional();
		~DIDAuthenticateEACAdditional() override;

		[[nodiscard]] const QString& getDidName() const;
		[[nodiscard]] const QString& getSignature() const;
};

} // namespace governikus