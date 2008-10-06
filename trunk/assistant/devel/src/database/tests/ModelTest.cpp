/**
 *	\file ModelTest.cpp
 *
 *	\author Pavel Mosunov 
 *  \since 2005/10/03
 */

#include "StdAfx.h"
#include "ModelTest.h"
#include "Model.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ModelTest);

ModelTest::ModelTest()
{
}

ModelTest::~ModelTest()
{
}

void ModelTest::setUp()
{
}

void ModelTest::tearDown()
{
}

void ModelTest::testGetModel()
{
	Models::IModel* pModel = GetModel();
	CPPUNIT_ASSERT(pModel != NULL);
}
