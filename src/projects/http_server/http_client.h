//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "http_request.h"
#include "http_response.h"

class HttpClient
{
public:
	friend class HttpServer;
	friend class HttpsServer;

	HttpClient(ov::ClientSocket *socket, const std::shared_ptr<HttpRequestInterceptor> &interceptor);
	virtual ~HttpClient() = default;

	std::shared_ptr<HttpRequest> &GetRequest()
	{
		return _request;
	}

	std::shared_ptr<HttpResponse> &GetResponse()
	{
		return _response;
	}

	void Send(const std::shared_ptr<ov::Data> &data);

	void MarkAsAccepted()
	{
		_is_tls_accepted = true;
	}

	bool IsAccepted() const
	{
		return _is_tls_accepted;
	}

protected:
	//--------------------------------------------------------------------
	// APIs which is related to TLS
	//--------------------------------------------------------------------
	void SetTls(const std::shared_ptr<ov::Tls> &tls);
	std::shared_ptr<ov::Tls> GetTls();
	std::shared_ptr<const ov::Tls> GetTls() const;

	void SetTlsData(const std::shared_ptr<const ov::Data> &data);

	//--------------------------------------------------------------------
	// Called by TLS module
	//--------------------------------------------------------------------
	ssize_t TlsRead(ov::Tls *tls, void *buffer, size_t length);
	ssize_t TlsWrite(ov::Tls *tls, const void *data, size_t length);

protected:
	std::shared_ptr<HttpRequest> _request = nullptr;
	std::shared_ptr<HttpResponse> _response = nullptr;

	std::shared_ptr<const ov::Data> _tls_data = nullptr;
	bool _is_tls_accepted = false;
};
