/*
 * Amazon FreeRTOS V201906.00 Major
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef AWS_TEST_TCP_H
#define AWS_TEST_TCP_H

/* Non-Encrypted Echo Server.
 * Update tcptestECHO_SERVER_ADDR# and
 * tcptestECHO_PORT with IP address
 * and port of unencrypted TCP echo server. */
#define tcptestECHO_SERVER_ADDR0         192
#define tcptestECHO_SERVER_ADDR1         168
#define tcptestECHO_SERVER_ADDR2         0
#define tcptestECHO_SERVER_ADDR3         2
#define tcptestECHO_PORT                 ( 5001 )

/* Encrypted Echo Server.
 * If tcptestSECURE_SERVER is set to 1, the following must be updated:
 * 1. aws_clientcredential.h to use a valid AWS endpoint.
 * 2. aws_clientcredential_keys.h with corresponding AWS keys.
 * 3. tcptestECHO_SERVER_TLS_ADDR0-3 with the IP address of an
 * echo server using TLS.
 * 4. tcptestECHO_PORT_TLS, with the port number of the echo server
 * using TLS.
 * 5. tcptestECHO_HOST_ROOT_CA with the trusted root certificate of the
 * echo server using TLS. */
#define tcptestSECURE_SERVER             1

#define tcptestECHO_SERVER_TLS_ADDR0     192
#define tcptestECHO_SERVER_TLS_ADDR1     168
#define tcptestECHO_SERVER_TLS_ADDR2     0
#define tcptestECHO_SERVER_TLS_ADDR3     2
#define tcptestECHO_PORT_TLS             ( 9000 )

/* Number of times to retry a connection if it fails. */
#define tcptestRETRY_CONNECTION_TIMES    6

/* The root certificate used for the encrypted echo server.
 * This certificate is self-signed, and not in the trusted catalog. */
static const char tcptestECHO_HOST_ROOT_CA[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEAzCCAuugAwIBAgIUcfuD+KkO3BQZxh+8BmDm4oh+4PcwDQYJKoZIhvcNAQEL\n"
		"BQAwgZAxCzAJBgNVBAYTAklOMQswCQYDVQQIDAJUUzESMBAGA1UEBwwJSHlkZXJh\n"
		"YmFkMRAwDgYDVQQKDAdSZWRwaW5lMQswCQYDVQQLDAJJVDEXMBUGA1UEAwwOMTky\n"
		"LjE2OC4zMS4xMjIxKDAmBgkqhkiG9w0BCQEWGXZpbm9kLmFsaWJhZGVAcmVkcGlu\n"
		"ZS5jb20wHhcNMTkwOTE0MDkwMzI3WhcNMjAwOTEzMDkwMzI3WjCBkDELMAkGA1UE\n"
		"BhMCSU4xCzAJBgNVBAgMAlRTMRIwEAYDVQQHDAlIeWRlcmFiYWQxEDAOBgNVBAoM\n"
		"B1JlZHBpbmUxCzAJBgNVBAsMAklUMRcwFQYDVQQDDA4xOTIuMTY4LjMxLjEyMjEo\n"
		"MCYGCSqGSIb3DQEJARYZdmlub2QuYWxpYmFkZUByZWRwaW5lLmNvbTCCASIwDQYJ\n"
		"KoZIhvcNAQEBBQADggEPADCCAQoCggEBANKDL9ynL4XrkeR3k4zJXkwIVfKDiyJc\n"
		"vc2g8VcBRB9IjgHuY/6HsmBOWUUiGJRjsUUqGQ8MKufBu1YcMu88hcOp7NxxCEfK\n"
		"UodJXxbec6NDGxT7Def6bhW61cZdIzcmUgeWueNxYXilQPUR+JfsPPHbjyoleWEb\n"
		"Lm4uM5ATPD9POlAAVexQn0KEVSslHjxMZdtpJOiKTqr7it9azieJzHChtslOnfuw\n"
		"rRgZAyA2hCc4mM3yHhc8OzNYBwnOJc4QkGdwu6njJtjWA2nxR2oghD7OBUomzPVl\n"
		"GirFou9YiXCa0LAQ2GYp6hNX/ceGJKXcUdrZtEO8oJBWTqX7KtvMGDMCAwEAAaNT\n"
		"MFEwHQYDVR0OBBYEFCGfccrmFEBocRx7ibmd10bBn6AYMB8GA1UdIwQYMBaAFCGf\n"
		"ccrmFEBocRx7ibmd10bBn6AYMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL\n"
		"BQADggEBABcfKafqBmLLTwKRCOpbN79Wf6wQCxc/wlHRk6Lh+atx9eS7FowggEaE\n"
		"PS/VjnpbLcvoJMkrl6HUwAQ3yVmgZd6n7VU65c+vpofZPyJnk8skY5Fd0xpSmACX\n"
		"H0ZidwvJ/kztVqfh/LYgm5EuAj8L9edPWiv+t4l8tpG3KcnG7w4aFuJ4EzZH4evL\n"
		"nx6xWqVzbyK8+mXKOyuRNuNdnAiGt/QezfwwG0xI4mxn8I99JY+H8rQp+4w+WMez\n"
		"RdQhBc+JCDVoeuSdL+AFnUuf9BhuTvSpnjhBrY8QrNytjk064xOGm5PcIqegfhCO\n"
		"kEJEiYUQpTCwUmeQxvtO7oRyAIS+TYs=\n"
		"-----END CERTIFICATE-----";
#endif /* ifndef AWS_TEST_TCP_H */
