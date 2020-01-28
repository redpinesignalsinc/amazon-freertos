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

#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

/*
 * @brief PEM-encoded client certificate.
 *
 * @todo If you are running one of the Amazon FreeRTOS demo projects, set this
 * to the certificate that will be used for TLS client authentication.
 *
 * @note Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAP9/Hy6LPEoPhcNYDfVdtM8NtDSBMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA4MDEwOTA3\n"\
"NDVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDcO/ay9ktSw5d/dYsg\n"\
"S9UxyuzjJW0DayLNW5r8D2hxGW4Icp3BLG9ugSt2uVy5Hp0MjdrLVPongfNUPQmU\n"\
"fv1g24v3gDOokByj1nMAX4Y3/N+uetwkp1EgoRbw3hDEa5nJ3DZRarJbnx0Guuex\n"\
"nbZJGInXcM+uIM1AMAARHCk8+k7A5Vz++tTgdH87/SafAdMh54rAZWWrauY/ebp5\n"\
"HvAED7Qvmd0ZvYHP7WUvoksQVwivhv9fms7hzTknXfGM3gyiu0cg1oqxaj8jM0dK\n"\
"m9LSDK7oWcp8gFlRHh+n61bTdng3lHSroZVjnxmTAOiSNm03jA7AxRjq1JQVtRk3\n"\
"T4U5AgMBAAGjYDBeMB8GA1UdIwQYMBaAFJzrtBX/MoGXFhFV8OgnwEyKx2DOMB0G\n"\
"A1UdDgQWBBSvDqTupHhoGpdU2YxGgW1bEHEFqjAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAlo8HytcdtrSCJU011jYEXThi\n"\
"UXwMYvIGyzSE5PGwKPUZMJZRlKIK6aBk9igkqi0t2mHaeZh2gi+gGW5AH/k1FxpF\n"\
"G7o/S3G4Qpk/708H17px9C8U/qbrrXOXXV1nNJ5i7gQSPlmmnVy3t3Py8PVF9gaJ\n"\
"njVCCwMKTmvqdJrWl7/wutTkRyF5eW/t5iDoBp8sQe4IUE0QI/6D08Zl9DBE3HD0\n"\
"ax9C7s3dMl4ybj7BIic1WOpbYqQIB/0Zb61+ZIoSHdg4QFMfGB3DGP7ulEKhGdw/\n"\
"EJUt0KslXHdOA26pW2zRNwHGZGeWRUMMTxxqWN/p2N5tW0tCwkfevooBClwHfg==\n"\
"-----END CERTIFICATE-----"

/*
 * @brief PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 *
 * @todo If you are using AWS IoT Just in Time Registration (JITR), set this to
 * the issuer (Certificate Authority) certificate of the client certificate above.
 *
 * @note This setting is required by JITR because the issuer is used by the AWS
 * IoT gateway for routing the device's initial request. (The device client
 * certificate must always be sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set below to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM    ""

/*
 * @brief PEM-encoded client private key.
 *
 * @todo If you are running one of the Amazon FreeRTOS demo projects, set this
 * to the private key that will be used for TLS client authentication.
 *
 * @note Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEpAIBAAKCAQEA3Dv2svZLUsOXf3WLIEvVMcrs4yVtA2sizVua/A9ocRluCHKd\n"\
"wSxvboErdrlcuR6dDI3ay1T6J4HzVD0JlH79YNuL94AzqJAco9ZzAF+GN/zfrnrc\n"\
"JKdRIKEW8N4QxGuZydw2UWqyW58dBrrnsZ22SRiJ13DPriDNQDAAERwpPPpOwOVc\n"\
"/vrU4HR/O/0mnwHTIeeKwGVlq2rmP3m6eR7wBA+0L5ndGb2Bz+1lL6JLEFcIr4b/\n"\
"X5rO4c05J13xjN4MortHINaKsWo/IzNHSpvS0gyu6FnKfIBZUR4fp+tW03Z4N5R0\n"\
"q6GVY58ZkwDokjZtN4wOwMUY6tSUFbUZN0+FOQIDAQABAoIBAQCe+R3kIwtOK+fQ\n"\
"1r1dy7+SGMQUVvCw9gYCLKzcZBblBRE7QdkEFxo8rzr7kWQQMLv20vLrsWR9Xj2S\n"\
"QhkHOv+VigUn3D/t3lD+Jw+OZmXRRlUEcqU0RB64qe+QYQvDbVRutOn0rUsf/eQK\n"\
"9qHGwWxSqUbe4AzZyc2/FqcFsslFvKZ3PSJXZK1vt41U2bFye5JTi0KicvvcHxF2\n"\
"NP4ydQknfLgGH12XM9MsQMoQvKczrx18xIucBKs9hSeuuVCYCSum53EVtXXETPGK\n"\
"534Z2+p1x2TWkcVSX9dpV29GtNHaD5IcfDsV08+9it/vn4CS0o8TdHcg6wxaMGRI\n"\
"hb4xDiqBAoGBAPLxgsqiCJ6/Gryr68PYXcPaOWF0NyUXn0HOizxn133jV3N27Sw+\n"\
"sV4Fx5BTb/cbMoN4MoTFExHK40wqB0C4r72K2Zc36C/+YLROxsxSOubipj/HOI5s\n"\
"+e5z+ekVNfN24u/cRzFHfmzCXLYEnCehAZeGpq+Ga/pXVF9hlYlK1ARxAoGBAOgS\n"\
"A1v9w4s+uV6UE+JjWInSaSinunpPx3pPuXPZJQ17jULrVwjMtdBrlSuq/0Kk/eXZ\n"\
"ZzCQ/mTztypGaj78a/rLCzLSuZLgha050W/Beq8Rlcq39c7nv/xYNbG7VPVR+RPp\n"\
"muuHxboM2iJul+xjmp0x/Sb2f446xTzBnoyR2tFJAoGAdZHzXeT0jv6lDdGVk33O\n"\
"Qs4FHuiDF2xLIim2sFEAtGelFrfjJ5pfc8UQQwReFxQOahltOVu2UtIBFgbw4519\n"\
"Txk3HDtuHmvYmOnWw2K4JqxhEFR1RlfuzDtMIkSli2FpifLscSjMQg39mAi7GWKN\n"\
"xqrfV+qRruE8b7xmBb80w8ECgYEA5c3Yl8KNXZjKNRbLLfYUmb3LNmAJb88KiFrk\n"\
"GQzg/R3ov2iOMf+M1wuBltFonhUF+xKydUxPLaaNLYa29RI+1wUzXKuA6in9FZFY\n"\
"jKL6Gtx+CVTrS20AFyE+YnZP5fG7hXVhXHSBXwAVDd2YAZSMabczqN6VZ0FZSRWu\n"\
"LCLOnUkCgYBbOOkuKDvkMAND18XZgv049eopK5lOsbI86pXcrnptuz6qnVVUqcSF\n"\
"Iw6uU7N1lZdtnVTyENkGLu1jL15j5ZoxDgSw8nhToNdGKGREss6dQZrkwMlWyVAu\n"\
"tDNUkMRe+WgnvR2uwGAIwMKdW5KO84eNFu56Vt3Gxoskk7bfvdGD5g==\n"\
"-----END RSA PRIVATE KEY-----"

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
