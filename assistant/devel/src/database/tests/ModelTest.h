/**
 *	\file ModelTest.h
 *	Model tests
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/03
 */


#pragma once
#include <cppunit/extensions/HelperMacros.h>

class ModelTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ModelTest);
	CPPUNIT_TEST(testGetModel);
	CPPUNIT_TEST_SUITE_END();

public:
	ModelTest();
	virtual ~ModelTest();

	void setUp();
	void tearDown();

	void testGetModel();
};
