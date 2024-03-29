/************************************************
*     Mini Porject helloworld for cmake         *
*     @authors Chi Thanh NGUYEN                 *
*     contact chithanhnguyen.math@gmail.com     *
*************************************************/
/*!
 *	@file prog/say_hello.cpp
 *	@date Sept 2013
 */

#include <hello.hpp>
#include <modules/hello_english.hpp>
#include <modules/hello_french.hpp>
#include <modules/hello_vietnamese.hpp>

int main()
{
	hello_english he;
	hello_french hf;
	hello_vietnamese hv;

	he.say_hello();
	hf.say_hello();
	hv.say_hello();

	return 0;
}
