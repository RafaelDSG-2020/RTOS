/*
 * Trace Recorder for Tracealyzer v4.6.0
 * Copyright 2021 Percepio AB
 * www.percepio.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * An example of a Tracealyzer extension for tracing API calls, in this case
 * for tracing selected functions in Amazon FreeRTOS/aws_secure_sockets.
 * See trcExtensions.h for information on how to use this.
 *
 * To create your own extension, first make sure to read the documentation
 * in trcExtensions.h. Then, to create an extension header file like this
 * one, you need to provide:
 *
 *  - Extension Definitions - name and event codes of the extensions.
 *
 *  - Trace Wrappers - calls the original function and traces the event.
 *
 *  - Function Redefinitions - changes the function calls to the trace wrappers.
 *
 * See the below comments for details about these definitions. Note that you
 * also need a matching .xml file for Tracealyzer to understand the data.
 * See trcExtensions.h for further information.
 */

#ifndef _AWS_SECURE_SOCKETS_TZEXT_H
#define _AWS_SECURE_SOCKETS_TZEXT_H

/***** Extension Definitions *****/

/******************************************************************************
 * <EXTENSIONPREFIX>_NAME
 * The name of the extension as a string constant. This name is used by the
 * Tracealyzer host application to find the right XML file for interpreting
 * the events. Assuming the extension name is "aws_secure_sockets", Tracealyzer
 * will look for an XML file named "aws_secure_sockets-<VERSION>.xml", first in
 * the folder of the current trace file, next in the Tracealyzer 4/cfg folder.
 * For the VERSION part, see the TRC_EXT_<ExtName>_VERSION macros below.
 *
 * Note: The extension name displayed in Tracealyzer is defined in the XML file
 * in the EventGroup element (e.g. <EventGroup name="SOCKETS">)
 *
 *****************************************************************************/
#define TRC_EXT_SOCKETS_NAME "aws_secure_sockets"

/******************************************************************************
 * <EXTENSIONPREFIX>_VERSION_MAJOR
 * <EXTENSIONPREFIX>_VERSION_MINOR
 * <EXTENSIONPREFIX>_VERSION_PATCH
 *
 * The version code of the extension (MAJOR.MINOR.PATCH)
 *
 * If you increment the version code when modifying an extension, you can still
 * show old traces recorded using an earlier version of the extension.
 *
 * Assuming the extension name is "aws_secure_sockets", and the below version
 * codes are 1 (MAJOR), 2 (MINOR), 3 (PATCH), Tracealyzer will assume the
 * corresponding XML file is named "aws_secure_sockets-v1.2.3.xml". So if then
 * view a trace recorded with extension version 1.2.2, those traces will look
 * for "aws_secure_sockets-v1.2.2.xml" instead.
 *
 * Note that major and minor are stored as 8 bit values, while patch is stored
 * using 16 bits. They are treated as unsigned integers, so the maximum values
 * are 256, 256 and 65535.
 *****************************************************************************/
#define TRC_EXT_SOCKETS_VERSION_MAJOR 1

#define TRC_EXT_SOCKETS_VERSION_MINOR 0

#define TRC_EXT_SOCKETS_VERSION_PATCH 0


/******************************************************************************
 * <EXTENSIONPREFIX>_<EVENTCODE>
 * The event codes used in the trace wrapper functions. Important that these
 * are relative to <PREFIX>_FIRST.
 *****************************************************************************/
#define EVENTCODE_SOCKETS_Connect (TRC_EXT_BASECODE + 0)

#define EVENTCODE_SOCKETS_Send (TRC_EXT_BASECODE + 1)

#define EVENTCODE_SOCKETS_Recv (TRC_EXT_BASECODE + 2)

/******************************************************************************
 * <EXTENSIONPREFIX>_COUNT
 * The number of event codes used by this extension. Should be at least 1.
 * Tracealyzer allows for events codes up to 4095.
 *****************************************************************************/
#define TRC_EXT_SOCKETS_COUNT 2


/***** Trace Wrappers *****/

#include <aws_secure_sockets.h> /* Including the original header file, so that custom data types are understood. */

static inline int32_t SOCKETS_Connect__trace( Socket_t xSocket, SocketsSockaddr_t * pxAddress, Socklen_t xAddressLength )
{
	int32_t ret = SOCKETS_Connect(xSocket, pxAddress, xAddressLength);

	// Note: The host-side xml file assumes that ret == 0 means OK, otherwise timeout/error.
	prvTraceStoreEvent3(EVENTCODE_SOCKETS_Connect, (uint32_t)xSocket, (uint32_t)pxAddress->ulAddress, (uint32_t)ret);

	return ret;
}

static inline int32_t SOCKETS_Send__trace( Socket_t xSocket, const void * pvBuffer, size_t xDataLength, uint32_t ulFlags )
{
	int32_t ret = SOCKETS_Send(xSocket, pvBuffer, xDataLength, ulFlags);

	// Note: The host-side xml file assumes that ret == 0 means OK, otherwise timeout/error.
	prvTraceStoreEvent2(EVENTCODE_SOCKETS_Send, (uint32_t)xSocket, (uint32_t)ret);

	return ret;
}


static inline int32_t SOCKETS_Recv__trace( Socket_t xSocket, void * pvBuffer, size_t xBufferLength, uint32_t ulFlags )
{
	int32_t ret = SOCKETS_Recv(xSocket, pvBuffer, xBufferLength, ulFlags);

	// Note: The host-side xml file assumes that ret == 0 means OK, otherwise timeout/error.
	prvTraceStoreEvent2(EVENTCODE_SOCKETS_Recv, (uint32_t)xSocket, (uint32_t)ret);

	return ret;
}

/***** Function Redefinitions *****/

#define SOCKETS_Connect SOCKETS_Connect__trace

#define SOCKETS_Send SOCKETS_Send__trace

#define SOCKETS_Recv SOCKETS_Recv__trace

#endif /* _AWS_SECURE_SOCKETS_TZEXT_H */
