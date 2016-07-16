#pragma once

enum class HttpMethod
{
	HTTP_GET = 0,   
	HTTP_POST,      
	HTTP_PUT,       
	HTTP_DELETE,    
	HTTP_HEAD       
};

enum class HttpErrorCode
{
	HTTP_ERROR_OK = 0,                          //No error occurred.

	HTTP_ERROR_RECEIVING_FAILED,                //Error occurred during receiving data.

	HTTP_ERROR_TIMEOUT,                         //Transaction timeout.

	HTTP_ERROR_BUFFER_TOO_SMALL,                //The receive buffer is too smaller or cannot allocate enough memory to store received data.

	HTTP_ERROR_SERVER,                          //The server returned error code (not equal 200).

	HTTP_ERROR_CONNECTION_FAILED,               //!< Connection failed.

	HTTP_ERROR_UNKNOWN                          //!< Unknown error.
};

enum class FileState
{
	IS_FREE = 0,
	DOWNLOAD_REQUIRED,
	DOWNLOADING,
	FAILED_DOWNLOAD,
	IN_MEMORY,
	IN_HDD,
	INVALID
};