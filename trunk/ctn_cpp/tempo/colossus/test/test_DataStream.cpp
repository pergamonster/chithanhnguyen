#define BOOST_TEST_MODULE colossus_test_DataStream

#include <iostream>
#include <fstream>
#include <ctime>

#include "../DataStream.h"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_DataStream)

const unsigned int N = 10;
const char* samlefilename = "sample.txt";

BOOST_AUTO_TEST_CASE(test_constructor_random_generator)
{
	SampleDataStream datastream(N);
		
	std::ifstream infile(samlefilename);
	if (!infile.good())
	{
		std::ofstream sampleFile;
		sampleFile.open(samlefilename);
		
		time_t t = time(0); struct tm * now = localtime(&t) ;
		sampleFile<<"# Data sample generated " << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'	<< now->tm_mday	<< std::endl;
		
		sampleFile << datastream;
		sampleFile.close();
	}
	else
	{
		std::cout << " Sample File is not rewrite" << std::endl;
	}
	
	const std::vector<int>& offlinedata = datastream.get_OfflineData();
	BOOST_CHECK(offlinedata.size() == N);
	//std::cout << datastream << std::endl;	
}


BOOST_AUTO_TEST_CASE(test_constructor_file_read)
{
	// small bug when a line have non number (at the end of file where a new line charactere
	
	SampleDataStream datastream(samlefilename);

	const std::vector<int>& offlinedata = datastream.get_OfflineData();
	BOOST_CHECK(offlinedata.size() == N);
}


BOOST_AUTO_TEST_CASE(test_data_processing)
{
	SampleDataStream datastream(N);
	
	unsigned int nbProcess = 0;

	std::vector<int> container;

	while(!datastream.isEnd())
	{
		datastream.update();
		const std::vector<int>& buffer = datastream.get_DataBuffer();
		container.push_back(buffer[0]);
	}

	const std::vector<int>& offlinedata = datastream.get_OfflineData();

	BOOST_CHECK( offlinedata.size() == container.size() );
	for (size_t i = 0; i < N; ++i)
	{
		BOOST_CHECK(offlinedata[i] == container[i]);
	}

	//std::cout << datastream << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()
