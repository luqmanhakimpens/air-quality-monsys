/*
 * spiffs_io.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */


#include <serial_log.h>
#include <spiffs_io/spiffs_io.h>

String spiffs_read(fs::FS &fs, const char * path)
{
	String ret_str;

	File file = fs.open(path);
	if(!file || file.isDirectory())
	{
		LOG_PRINTLN("- failed to open file for reading");
		return "";
	}

	while(file.available())
	{
		ret_str += (char) file.read();
	}
	file.close();
	return ret_str;
}

size_t spiffs_write(fs::FS &fs, const char * path, const char * message)
{
	File file = fs.open(path, FILE_WRITE);
	if(!file)
	{
		LOG_PRINTLN("- failed to open file for writing");
		return -1;
	}
	return file.print(message);
}

bool spiffs_delete(fs::FS &fs, const char * path)
{
	LOG_PRINTF("Deleting file: %s\r\n", path);
	return fs.remove(path);
}
