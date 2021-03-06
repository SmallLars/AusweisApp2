FUNCTION(ADD_TEST_EXECUTABLE testname)
	ADD_EXECUTABLE(${testname}
		$<TARGET_OBJECTS:QRC_FIXTURE_OBJ>
		${ARGN})

	TARGET_INCLUDE_DIRECTORIES(${testname} SYSTEM PUBLIC ${OPENSSL_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(${testname} Qt5::Network Qt5::Xml Qt5::Test ${OPENSSL_LIBRARIES})
	TARGET_LINK_LIBRARIES(${testname} AusweisAppTestHelper AusweisAppCore AusweisAppCard AusweisAppGlobal AusweisAppServices AusweisAppSettings AusweisAppNetwork)
	TARGET_LINK_LIBRARIES(${testname} AusweisAppActivationInternal AusweisAppJsonApi AusweisAppAidl AusweisAppQml)
	TARGET_LINK_LIBRARIES(${testname} AusweisAppCardRemote)

	IF(DESKTOP)
		TARGET_LINK_LIBRARIES(${testname} Qt5::Widgets AusweisAppWidget AusweisAppCardPcsc AusweisAppCardDrivers AusweisAppActivationWebservice)
	ENDIF()

	IF(LINUX OR ANDROID OR IOS)
		TARGET_LINK_LIBRARIES(${testname} AusweisAppCardBluetooth)
	ENDIF()

	IF(WIN32)
		TARGET_LINK_LIBRARIES(${testname} ${WIN_DEFAULT_LIBS})
	ELSEIF(MAC)
		TARGET_LINK_LIBRARIES(${testname} ${OSX_APPKIT})
	ELSEIF(ANDROID)
		TARGET_LINK_LIBRARIES(${testname} Qt5::AndroidExtras)
	ENDIF()
ENDFUNCTION()


FUNCTION(ADD_TEST_EXECUTABLE_SUBDIR)
	SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
	IF(APPLE OR WIN32 OR BSD)
		LIST(REMOVE_ITEM SUBDIRS bluetooth)
	ENDIF()
	IF(IOS OR ANDROID)
		LIST(REMOVE_ITEM SUBDIRS gui)
		LIST(REMOVE_ITEM SUBDIRS cli)
		LIST(REMOVE_ITEM SUBDIRS pcsc)
		LIST(REMOVE_ITEM SUBDIRS drivers)
		LIST(REMOVE_ITEM SUBDIRS activation_webservice)
	ENDIF()

	FOREACH(SUBDIR ${SUBDIRS})
		FILE(GLOB_RECURSE TEST_SUBFILES "${SUBDIR}/*.cpp")

		FOREACH(sourcefile ${TEST_SUBFILES})
			EXTRACT_TESTNAME(TESTNAME ${sourcefile})
			ADD_TEST_EXECUTABLE(${TESTNAME} ${sourcefile})
			GET_TEST_CMDLINE(TEST_CMDLINE ${TESTNAME})
			ADD_TEST(${TESTNAME} ${TESTNAME} ${TEST_CMDLINE})
			SET_TESTS_PROPERTIES(${TESTNAME} PROPERTIES LABELS "ausweisapp")
		ENDFOREACH()
	ENDFOREACH()
ENDFUNCTION()



CONFIGURE_DEFAULT_FILES(${CMAKE_CURRENT_BINARY_DIR})

# Build single test binaries for each cpp file
ADD_TEST_EXECUTABLE_SUBDIR()
