/*
 * tool.h
 *
 *  Created on: Aug 7, 2019
 *      Author: luqma
 */

#ifndef SRC_TOOL_TOOL_H_
#define SRC_TOOL_TOOL_H_

#include "Arduino.h"
#include <rom/rtc.h>
#include <MD5Builder.h>


class tool
{
public:
	tool();
	String get_mac_address(bool delimited=true);
	String device_name(const String& prefix="", const String& sufix="");
	String device_id();
	String generate_random_string(unsigned int len);
	String parse_string(String data, char separator, int index);
	String cli_parser(String cli_str, const String& opt);
	String verbose_print_reset_reason(RESET_REASON reason);
	void md5_hash(const char *input, char *output);
	bool is_alphanumeric(const char* str);
};

#endif /* SRC_TOOL_TOOL_H_ */
