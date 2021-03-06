/*!
 * \brief Unit tests for \ref PACEInfo
 *
 * \copyright Copyright (c) 2015 Governikus GmbH & Co. KG
 */

#include <QtCore>
#include <QtTest>

#include "asn1/KnownOIDs.h"
#include "asn1/PACEInfo.h"


using namespace governikus;

class test_PACEInfo
	: public QObject
{
	Q_OBJECT

	private Q_SLOTS:
		void parseCrap()
		{
			QVERIFY(PACEInfo::decode("3003020101") == nullptr);
		}


		void parseChipAuthenticationInfo()
		{
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202030202"
												   "            02 01 02"
												   "            02 01 08");

			QVERIFY(PACEInfo::decode(bytes) == nullptr);
		}


		void parsePACEInfoWithWrongContent()
		{
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040202"
												   "            04 01 02"
												   "            04 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo == nullptr);
		}


		void missingOid()
		{
			QByteArray bytes = QByteArray::fromHex("30 06"
												   "            02 01 02"
												   "            02 01 08");

			QVERIFY(PACEInfo::decode(bytes) == nullptr);
		}


		void missingVersion()
		{
			QByteArray bytes = QByteArray::fromHex("30 0C"
												   "            06 0A 04007F00070202040202");

			QVERIFY(PACEInfo::decode(bytes) == nullptr);
		}


		void withoutOptionalParameter()
		{
			QByteArray bytes = QByteArray::fromHex("30 0F"
												   "            06 0A 04007F00070202040202"
												   "            02 01 02");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QCOMPARE(paceInfo->getParameterIdAsInt(), -1);
		}


		void getParameterId()
		{
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040202"
												   "            02 01 02"
												   "            02 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QCOMPARE(paceInfo->getParameterId(), QByteArray::fromHex("08"));
			QCOMPARE(paceInfo->getParameterIdAsInt(), 8);
		}


		void getVersion()
		{
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040202"
												   "            02 01 02"
												   "            02 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QCOMPARE(paceInfo->getVersion(), QByteArray::fromHex("02"));
		}


		void getEnumString()
		{
			QCOMPARE(getEnumName(KeyAgreementType::DH), QStringLiteral("DH"));
			QCOMPARE(getEnumName(MappingType::IM), QStringLiteral("IM"));
		}


		void getKeyAgreementType()
		{
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040101 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040102 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040103 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040104 020102"))->getKeyAgreementType(), KeyAgreementType::DH);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040201 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040202 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040203 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040204 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040301 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040302 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040303 020102"))->getKeyAgreementType(), KeyAgreementType::DH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040304 020102"))->getKeyAgreementType(), KeyAgreementType::DH);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040401 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040402 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040403 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040404 020102"))->getKeyAgreementType(), KeyAgreementType::ECDH);

		}


		void getMappingType()
		{
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040101 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040102 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040103 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040104 020102"))->getMappingType(), MappingType::GM);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040201 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040202 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040203 020102"))->getMappingType(), MappingType::GM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040204 020102"))->getMappingType(), MappingType::GM);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040301 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040302 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040303 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040304 020102"))->getMappingType(), MappingType::IM);

			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040401 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040402 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040403 020102"))->getMappingType(), MappingType::IM);
			QCOMPARE(PACEInfo::decode(QByteArray::fromHex("300F 060A04007F00070202040404 020102"))->getMappingType(), MappingType::IM);

		}


		void isStandardizedECDH_GM()
		{
			// version = 2, parameterId = 8
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040202"
												   "            02 01 02"
												   "            02 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(paceInfo->isStandardizedDomainParameters());
		}


		void isNotStandardizedECDH_GM()
		{
			// version = 2, parameterId = 7
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040202"
												   "            02 01 02"
												   "            02 01 07");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(!paceInfo->isStandardizedDomainParameters());
		}


		void isStandardizedECDH_IM()
		{
			// version = 2, parameterId = 8
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040402"
												   "            02 01 02"
												   "            02 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(paceInfo->isStandardizedDomainParameters());
		}


		void isNotStandardizedECDH_IM()
		{
			// version = 2, parameterId = 10
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040402"
												   "            02 01 02"
												   "            02 01 0A");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(!paceInfo->isStandardizedDomainParameters());
		}


		void isStandardizedDH_GM()
		{
			// version = 2, parameterId = 0
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040102"
												   "            02 01 02"
												   "            02 01 00");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(paceInfo->isStandardizedDomainParameters());
		}


		void isNotStandardizedDH_GM()
		{
			// version = 2, parameterId = 7
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040102"
												   "            02 01 02"
												   "            02 01 07");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(!paceInfo->isStandardizedDomainParameters());
		}


		void isStandardizedDH_IM()
		{
			// version = 2, parameterId = 0
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040302"
												   "            02 01 02"
												   "            02 01 00");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(paceInfo->isStandardizedDomainParameters());
		}


		void isNotStandardizedDH_IM()
		{
			// version = 2, parameterId = 8
			QByteArray bytes = QByteArray::fromHex("30 12"
												   "            06 0A 04007F00070202040302"
												   "            02 01 02"
												   "            02 01 08");

			auto paceInfo = PACEInfo::decode(bytes);

			QVERIFY(paceInfo != nullptr);
			QVERIFY(!paceInfo->isStandardizedDomainParameters());
		}


};

QTEST_GUILESS_MAIN(test_PACEInfo)
#include "test_PACEInfo.moc"
