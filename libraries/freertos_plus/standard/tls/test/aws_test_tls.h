/*
 * Amazon FreeRTOS TLS V1.1.4
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef _AWS_TLS_TEST_H_
#define _AWS_TLS_TEST_H_

/*
 * PEM-encoded client certificate.
 *
 * Certificate for P-256 elliptic curve key.
 */
static const char tlstestCLIENT_CERTIFICATE_PEM_EC[] =
		"-----BEGIN CERTIFICATE-----\n"
"MIIDDDCCAfSgAwIBAgIUTQxm5xRwiF2D6Yt8H+iJJ6qlymAwDQYJKoZIhvcNAQEL\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDgwMTA5MTQ0\n"
"MFoXDTQ5MTIzMTIzNTk1OVowgZsxCzAJBgNVBAYTAklOMREwDwYDVQQIDAhUZWxh\n"
"Z2FuYTESMBAGA1UEBwwJSHlkZXJhYmFkMSQwIgYDVQQKDBtSZWRwaW5lIFNpZ25h\n"
"bHMgSW5kIFBWVCBMdGQxDjAMBgNVBAMMBXZpbm9kMS8wLQYJKoZIhvcNAQkBFiB2\n"
"aW5vZC5hbGliYWRlQHJlZHBpbmVzaWduYWxzLmNvbTBZMBMGByqGSM49AgEGCCqG\n"
"SM49AwEHA0IABFeWjak1UT+73wZMBGLJ2l12jrMT2ZXtd/JuQnOEEGeymiHoCrT8\n"
"7tscx4ihfc6nXjcAO645PBlARO3l5FQsOJSjYDBeMB8GA1UdIwQYMBaAFODtqm/z\n"
"TRaO76iGglNr2xoUElpsMB0GA1UdDgQWBBTapNLilsx2UloQDEF0F68Pk+hSZjAM\n"
"BgNVHRMBAf8EAjAAMA4GA1UdDwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEA\n"
"ch3TSalU5iJWKPy1v1Q9gxI0Oc8xFJ5bgBZWxm0eSd4JEUteKdYS6LJvUo/07F4S\n"
"DWrj4yvB4a7kU7QdlylikJ7vBZDuXc8Olz1iMboSuB8F4u22WHV0uS+xf4YK3v08\n"
"6RWtQNJnRrN83Tetp8znypz5fu1O3iICbEEiK+JdYSiNEPrTwlnM0MqYA0hI0Rz0\n"
"dTiOsF8Igh2wcTUvuYibK0HCL1FojEWKH3k5jFmcE04aD09YVDC3GwsOQCKR1c2q\n"
"3f/FUdOYge6b+z+MK6rGAc2uIs9wfKDXBgfc0LB8bEIlQzTwTLgsdGbh2aYy1/Xc\n"
"8YFhzsKnF8kUBlR0N+6SZQ==\n"
"-----END CERTIFICATE-----";

/*
 * PEM-encoded client private key.
 *
 * This is a P-256 elliptic curve key.
 */
static const char tlstestCLIENT_PRIVATE_KEY_PEM_EC[] =
		"-----BEGIN EC PRIVATE KEY-----\n"
		"MHcCAQEEIPv9dzsY2PidoIdJYzeaL4WcptCuUWoaGo13HpQp9hh/oAoGCCqGSM49\n"
		"AwEHoUQDQgAEV5aNqTVRP7vfBkwEYsnaXXaOsxPZle138m5Cc4QQZ7KaIegKtPzu\n"
		"2xzHiKF9zqdeNwA7rjk8GUBE7eXkVCw4lA==\n"
		"-----END EC PRIVATE KEY-----";
/* One character of this certificate has been changed in the issuer
 * name from Amazon Web Services to Amazon Web Cervices. */
static const char tlstestCLIENT_CERTIFICATE_PEM_MALFORMED[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDWjCCAkKgAwIBAgIVAP9/Hy6LPEoPhcNYDfVdtM8NtDSBMA0GCSqGSIb3DQEB\n"
		"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgQ2VydmljZXMgTz1BbWF6b24uY29t\n"
		"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA4MDEwOTA3\n"
		"NDVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
		"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDcO/ay9ktSw5d/dYsg\n"
		"S9UxyuzjJW0DayLNW5r8D2hxGW4Icp3BLG9ugSt2uVy5Hp0MjdrLVPongfNUPQmU\n"
		"fv1g24v3gDOokByj1nMAX4Y3/N+uetwkp1EgoRbw3hDEa5nJ3DZRarJbnx0Guuex\n"
		"nbZJGInXcM+uIM1AMAARHCk8+k7A5Vz++tTgdH87/SafAdMh54rAZWWrauY/ebp5\n"
		"HvAED7Qvmd0ZvYHP7WUvoksQVwivhv9fms7hzTknXfGM3gyiu0cg1oqxaj8jM0dK\n"
		"m9LSDK7oWcp8gFlRHh+n61bTdng3lHSroZVjnxmTAOiSNm03jA7AxRjq1JQVtRk3\n"
		"T4U5AgMBAAGjYDBeMB8GA1UdIwQYMBaAFJzrtBX/MoGXFhFV8OgnwEyKx2DOMB0G\n"
		"A1UdDgQWBBSvDqvinodoGpdU2YxGgW1bEHEFqjAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
		"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAlo8HytcdtrSCJU011jYEXThi\n"
		"UXwMYvIGyzSE5PGwKPUZMJZRlKIK6aBk9igkqi0t2mHaeZh2gi+gGW5AH/k1FxpF\n"
		"G7o/S3G4Qpk/708H17px9C8U/qbrrXOXXV1nNJ5i7gQSPlmmnVy3t3Py8PVF9gaJ\n"
		"njVCCwMKTmvqdJrWl7/wutTkRyF5eW/t5iDoBp8sQe4IUE0QI/6D08Zl9DBE3HD0\n"
		"ax9C7s3dMl4ybj7BIic1WOpbYqQIB/0Zb61+ZIoSHdg4QFMfGB3DGP7ulEKhGdw/\n"
		"EJUt0KslXHdOA26pW2zRNwHGZGeWRUMMTxxqWN/p2N5tW0tCwkfevooBClwHfg==\n"
		"-----END CERTIFICATE-----";

/* Certificate which is not trusted by the broker. */
static const char tlstestCLIENT_UNTRUSTED_CERTIFICATE_PEM[] =
		"Certificate:\n"
		" Data:\n"
		" Version: 3 (0x2)\n"
		" Serial Number:\n"
		" b9:bc:90:ed:ad:aa:0a:8c\n"
		" Signature Algorithm: sha256WithRSAEncryption\n"
		" Issuer: C=US, ST=Montana, L=Bozeman, O=wolfSSL_2048, OU=Programming-2048, CN=www.wolfssl.com/emailAddress=info@wolfssl.com\n"
		" Validity\n"
		" Not Before: Aug 11 20:07:37 2016 GMT\n"
		" Not After : May 8 20:07:37 2019 GMT\n"
		" Subject: C=US, ST=Montana, L=Bozeman, O=wolfSSL_2048, OU=Programming-2048, CN=www.wolfssl.com/emailAddress=info@wolfssl.com\n"
		" Subject Public Key Info:\n"
		" Public Key Algorithm: rsaEncryption\n"
		" Public-Key: (2048 bit)\n"
		" Modulus:\n"
		" 00:c3:03:d1:2b:fe:39:a4:32:45:3b:53:c8:84:2b:\n"
		" 2a:7c:74:9a:bd:aa:2a:52:07:47:d6:a6:36:b2:07:\n"
		" 32:8e:d0:ba:69:7b:c6:c3:44:9e:d4:81:48:fd:2d:\n"
		" 68:a2:8b:67:bb:a1:75:c8:36:2c:4a:d2:1b:f7:8b:\n"
		" ba:cf:0d:f9:ef:ec:f1:81:1e:7b:9b:03:47:9a:bf:\n"
		" 65:cc:7f:65:24:69:a6:e8:14:89:5b:e4:34:f7:c5:\n"
		" b0:14:93:f5:67:7b:3a:7a:78:e1:01:56:56:91:a6:\n"
		" 13:42:8d:d2:3c:40:9c:4c:ef:d1:86:df:37:51:1b:\n"
		" 0c:a1:3b:f5:f1:a3:4a:35:e4:e1:ce:96:df:1b:7e:\n"
		" bf:4e:97:d0:10:e8:a8:08:30:81:af:20:0b:43:14:\n"
		" c5:74:67:b4:32:82:6f:8d:86:c2:88:40:99:36:83:\n"
		" ba:1e:40:72:22:17:d7:52:65:24:73:b0:ce:ef:19:\n"
		" cd:ae:ff:78:6c:7b:c0:12:03:d4:4e:72:0d:50:6d:\n"
		" 3b:a3:3b:a3:99:5e:9d:c8:d9:0c:85:b3:d9:8a:d9:\n"
		" 54:26:db:6d:fa:ac:bb:ff:25:4c:c4:d1:79:f4:71:\n"
		" d3:86:40:18:13:b0:63:b5:72:4e:30:c4:97:84:86:\n"
		" 2d:56:2f:d7:15:f7:7f:c0:ae:f5:fc:5b:e5:fb:a1:\n"
		" ba:d3\n"
		" Exponent: 65537 (0x10001)\n"
		" X509v3 extensions:\n"
		" X509v3 Subject Key Identifier: \n"
		" 33:D8:45:66:D7:68:87:18:7E:54:0D:70:27:91:C7:26:D7:85:65:C0\n"
		" X509v3 Authority Key Identifier: \n"
		" keyid:33:D8:45:66:D7:68:87:18:7E:54:0D:70:27:91:C7:26:D7:85:65:C0\n"
		" DirName:/C=US/ST=Montana/L=Bozeman/O=wolfSSL_2048/OU=Programming-2048/CN=www.wolfssl.com/emailAddress=info@wolfssl.com\n"
		" serial:B9:BC:90:ED:AD:AA:0A:8C\n"
		"\n"
		" X509v3 Basic Constraints: \n"
		" CA:TRUE\n"
		" Signature Algorithm: sha256WithRSAEncryption\n"
		" 33:85:08:b4:58:0e:a2:00:03:74:de:77:fb:d1:2b:76:9c:97:\n"
		" 90:20:21:a2:e8:2e:22:50:26:04:76:ba:5b:47:79:e5:52:f7:\n"
		" c4:0d:79:ff:62:3f:05:7c:c3:08:6c:e0:b7:81:d0:ce:c6:c9:\n"
		" 46:b9:8e:4b:5f:56:79:4b:13:b6:d1:6b:66:4b:ce:00:0d:e3:\n"
		" 76:5e:fb:cb:b5:5d:12:31:05:f1:bb:39:f6:86:90:ca:92:56:\n"
		" a4:a0:75:21:b6:1d:4c:96:c3:45:eb:5a:91:94:32:d3:59:b8:\n"
		" c9:73:1f:03:a9:81:63:e0:43:c0:1e:c8:65:be:3b:a7:53:c3:\n"
		" 44:ff:b3:fb:47:84:a8:b6:9d:00:d5:6b:ae:87:f8:bb:35:b2:\n"
		" 6c:66:0b:11:ee:6f:fe:12:ed:59:79:f1:3e:f2:d3:61:27:8b:\n"
		" 95:7e:99:75:8d:a4:9f:34:85:f1:25:4d:48:1e:9b:6b:70:f6:\n"
		" 66:cc:56:b1:a3:02:52:8a:7c:aa:af:07:da:97:c6:0c:a5:8f:\n"
		" ed:cb:f5:d8:04:5d:97:0a:5d:5a:2b:49:f5:bd:93:e5:23:9b:\n"
		" 99:b5:0c:ff:0c:7e:38:82:b2:6e:ab:8a:c9:a7:45:ab:d6:d7:\n"
		" 93:35:70:07:7e:c8:3d:a5:fe:33:8f:d9:85:c0:c7:5a:02:e4:\n"
		" 7c:d6:35:9e\n"
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEyjCCA7KgAwIBAgIJALm8kO2tqgqMMA0GCSqGSIb3DQEBCwUAMIGeMQswCQYD\n"
		"VQQGEwJVUzEQMA4GA1UECAwHTW9udGFuYTEQMA4GA1UEBwwHQm96ZW1hbjEVMBMG\n"
		"A1UECgwMd29sZlNTTF8yMDQ4MRkwFwYDVQQLDBBQcm9ncmFtbWluZy0yMDQ4MRgw\n"
		"FgYDVQQDDA93d3cud29sZnNzbC5jb20xHzAdBgkqhkiG9w0BCQEWEGluZm9Ad29s\n"
		"ZnNzbC5jb20wHhcNMTYwODExMjAwNzM3WhcNMTkwNTA4MjAwNzM3WjCBnjELMAkG\n"
		"A1UEBhMCVVMxEDAOBgNVBAgMB01vbnRhbmExEDAOBgNVBAcMB0JvemVtYW4xFTAT\n"
		"BgNVBAoMDHdvbGZTU0xfMjA0ODEZMBcGA1UECwwQUHJvZ3JhbW1pbmctMjA0ODEY\n"
		"MBYGA1UEAwwPd3d3LndvbGZzc2wuY29tMR8wHQYJKoZIhvcNAQkBFhBpbmZvQHdv\n"
		"bGZzc2wuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwwPRK/45\n"
		"pDJFO1PIhCsqfHSavaoqUgdH1qY2sgcyjtC6aXvGw0Se1IFI/S1oootnu6F1yDYs\n"
		"StIb94u6zw357+zxgR57mwNHmr9lzH9lJGmm6BSJW+Q098WwFJP1Z3s6enjhAVZW\n"
		"kaYTQo3SPECcTO/Rht83URsMoTv18aNKNeThzpbfG36/TpfQEOioCDCBryALQxTF\n"
		"dGe0MoJvjYbCiECZNoO6HkByIhfXUmUkc7DO7xnNrv94bHvAEgPUTnINUG07ozuj\n"
		"mV6dyNkMhbPZitlUJttt+qy7/yVMxNF59HHThkAYE7BjtXJOMMSXhIYtVi/XFfd/\n"
		"wK71/Fvl+6G60wIDAQABo4IBBzCCAQMwHQYDVR0OBBYEFDPYRWbXaIcYflQNcCeR\n"
		"xybXhWXAMIHTBgNVHSMEgcswgciAFDPYRWbXaIcYflQNcCeRxybXhWXAoYGkpIGh\n"
		"MIGeMQswCQYDVQQGEwJVUzEQMA4GA1UECAwHTW9udGFuYTEQMA4GA1UEBwwHQm96\n"
		"ZW1hbjEVMBMGA1UECgwMd29sZlNTTF8yMDQ4MRkwFwYDVQQLDBBQcm9ncmFtbWlu\n"
		"Zy0yMDQ4MRgwFgYDVQQDDA93d3cud29sZnNzbC5jb20xHzAdBgkqhkiG9w0BCQEW\n"
		"EGluZm9Ad29sZnNzbC5jb22CCQC5vJDtraoKjDAMBgNVHRMEBTADAQH/MA0GCSqG\n"
		"SIb3DQEBCwUAA4IBAQAzhQi0WA6iAAN03nf70St2nJeQICGi6C4iUCYEdrpbR3nl\n"
		"UvfEDXn/Yj8FfMMIbOC3gdDOxslGuY5LX1Z5SxO20WtmS84ADeN2XvvLtV0SMQXx\n"
		"uzn2hpDKklakoHUhth1MlsNF61qRlDLTWbjJcx8DqYFj4EPAHshlvjunU8NE/7P7\n"
		"R4Sotp0A1Wuuh/i7NbJsZgsR7m/+Eu1ZefE+8tNhJ4uVfpl1jaSfNIXxJU1IHptr\n"
		"cPZmzFaxowJSinyqrwfal8YMpY/ty/XYBF2XCl1aK0n1vZPlI5uZtQz/DH44grJu\n"
		"q4rJp0Wr1teTNXAHfsg9pf4zj9mFwMdaAuR81jWe\n"
		"-----END CERTIFICATE-----";

/* Private key corresponding to the untrusted certificate. */
static const char tlstestCLIENT_UNTRUSTED_PRIVATE_KEY_PEM[] =
		"-----BEGIN RSA PRIVATE KEY-----\n"
		"MIIEpAIBAAKCAQEAwwPRK/45pDJFO1PIhCsqfHSavaoqUgdH1qY2sgcyjtC6aXvG\n"
		"w0Se1IFI/S1oootnu6F1yDYsStIb94u6zw357+zxgR57mwNHmr9lzH9lJGmm6BSJ\n"
		"W+Q098WwFJP1Z3s6enjhAVZWkaYTQo3SPECcTO/Rht83URsMoTv18aNKNeThzpbf\n"
		"G36/TpfQEOioCDCBryALQxTFdGe0MoJvjYbCiECZNoO6HkByIhfXUmUkc7DO7xnN\n"
		"rv94bHvAEgPUTnINUG07ozujmV6dyNkMhbPZitlUJttt+qy7/yVMxNF59HHThkAY\n"
		"E7BjtXJOMMSXhIYtVi/XFfd/wK71/Fvl+6G60wIDAQABAoIBAQCi5thfEHFkCJ4u\n"
		"bdFtHoXSCrGMR84sUWqgEp5T3pFMHW3qWXvyd6rZxtmKq9jhFuRjJv+1bBNZuOOl\n"
		"yHIXLgyfb+VZP3ZvSbERwlouFikN3reO3EDVou7gHqH0vpfbhmOWFM2YCWAtMHac\n"
		"PM3miO5HknkLWgDiXl8RfH35CLcgBokqXf0AqyLh8LO8JKleJg4fAC3+IZpTW23T\n"
		"K6uUgmhDNtj2L8Yi/LVBXQ0zYOqkfX7oS1WRVtNcV48flBcvqt7pnqj0z4pMjqDk\n"
		"VnOyz0+GxWk88yQgi1yWDPprEjuaZ8HfxpaypdWSDZsJQmgkEEXUUOQXOUjQNYuU\n"
		"bRHej8pZAoGBAOokp/lpM+lx3FJ9iCEoL0neunIW6cxHeogNlFeEWBY6gbA/os+m\n"
		"bB6wBikAj+d3dqzbysfZXps/JpBSrvw4kAAUu7QPWJTnL2p+HE9BIdQxWR9OihqN\n"
		"p1dsItjl9H4yphDLZKVVA4emJwWMw9e2J7JNujDaR49U0z2LhI2UmFilAoGBANU4\n"
		"G8OPxZMMRwtvNZLFsI1GyJIYj/WACvfvof6AubUqusoYsF2lB9CTjdicBBzUYo6m\n"
		"JoEB/86KKmM0NUCqbYDeiSNqV02ebq2TTlaQC22dc4sMric93k7wqsVseGdslFKc\n"
		"N2dsLe+7r9+mkDzER8+Nlp6YqbSfxaZQ3LPw+3QXAoGAXoMJYr26fKK/QnT1fBzS\n"
		"ackEDYV+Pj0kEsMYe/Mp818OdmxZdeRBhGmdMvPNIquwNbpKsjzl2Vi2Yk9d3uWe\n"
		"CspTsiz3nrNrClt5ZexukU6SIPb8/Bbt03YM4ux/smkTa3gOWkZktF63JaBadTpL\n"
		"78c8Pvf9JrggxJkKmnO+wxkCgYEAukSTFKw0GTtfkWCs97TWgQU2UVM96GXcry7c\n"
		"YT7Jfbh/h/A7mwOCKTfOck4R1bHBDAegmZFKjX/sec/xObXphexi99p9vGRNIjwO\n"
		"8tZR9YfYmcARIF0PKf1b4q7ZHNkhVm38hNBf7RAVHBgh58Q9S9fQnmqVzyLJA3ue\n"
		"42AB/C8CgYAR0EvPG2e5nxB1R4ZlrjHCxjCsWQZQ2Q+1cAb38NPIYnyo2m72IT/T\n"
		"f1/qiqs/2Spe81HSwjA34y2jdQ0eTSE01VdwXIm/cuxKbmjVzRh0M06MOkWP5pZA\n"
		"62P5GYY6Ud2JS7Dz+Z9dKJU4vjWrylznk1M0oUVdEzllQkahn831vw==\n"
		"-----END RSA PRIVATE KEY-----";

/* Device certificate created using BYOC instructions. */
static const char tlstestCLIENT_BYOC_CERTIFICATE_PEM[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIB8zCCAZkCCQCnjIZU0WDbMjAKBggqhkjOPQQDAjB4MQswCQYDVQQGEwJVUzET\n"
"MBEGA1UECAwKV2FzaGluZ3RvbjEYMBYGA1UECgwPQW1hem9uLmNvbSBJbmMuMRww\n"
"GgYDVQQLDBNBbWF6b24gV2ViIFNlcnZpY2VzMRwwGgYDVQQDDBNBV1MgSW9UIENl\n"
"cnRpZmljYXRlMB4XDTE5MTIyMDEyMTYwOVoXDTIxMDUwMzEyMTYwOVowgYoxCzAJ\n"
"BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\n"
"MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xHDAaBgNVBAsME0FtYXpvbiBXZWIg\n"
"U2VydmljZXMxHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNhdGUwWTATBgcqhkjO\n"
"PQIBBggqhkjOPQMBBwNCAASi+I04yKM+v1ewPgHAJeLoQwX9x2fBiKti6JD0cQlv\n"
"MdPmZxdx6+rlFSJJolVtmpvfEwhwWQXFUUj0Q8elm5n5MAoGCCqGSM49BAMCA0gA\n"
"MEUCIQDbcVFT9qqxFx4QU1SJcf6zO4KvCT/d/Lte0cjVqg0UWgIgL/1OOnpkGwZW\n"
"cvqeEJ8Zl7DUuErZeE5kTsxeIDh/pa4=\n"
"-----END CERTIFICATE-----";

/* Device private key created using BYOC instructions. */
static const char tlstestCLIENT_BYOC_PRIVATE_KEY_PEM[] =
"-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEINzy3M/kXZP0cj2tRrAFy8NDNHvJQyzjUNCWbNYHnVmboAoGCCqGSM49\n"
"AwEHoUQDQgAEoviNOMijPr9XsD4BwCXi6EMF/cdnwYirYuiQ9HEJbzHT5mcXcevq\n"
"5RUiSaJVbZqb3xMIcFkFxVFI9EPHpZuZ+Q==\n"
"-----END EC PRIVATE KEY-----";

#endif /* ifndef _AWS_TLS_TEST_H_ */
