/*!
 * ResponseType.h
 *
 * \brief Represents a PAOS response type according to ISOCommon.xsd
 *
 * \copyright Copyright (c) 2014 Governikus GmbH & Co. KG
 */


#pragma once


#include "PaosMessage.h"
#include "Result.h"


namespace governikus
{

class ResponseType
	: public PaosMessage
{
	private:
		Result mResult;

	public:
		ResponseType(PaosType pType);
		virtual ~ResponseType();

		const Result& getResult() const;
		void setResult(const Result& result);
};

} /* namespace governikus */
