//=============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "publisher.h"
#include "tls.h"
#include "urls.h"
namespace cfg
{
	struct DashPublisher : public Publisher
	{
		PublisherType GetType() const override
		{
			return PublisherType::Dash;
		}

		int GetPort() const
		{
			return _port;
		}
		const Tls &GetTls() const
		{
			return _tls;
		}

		int GetSegmentCount() const
		{
			return _segment_count;
		}

		int GetSegmentDuration() const
		{
			return _segment_duration;
		}

		const std::vector<Url> &GetCrossDomains() const
		{
			return _cross_domain_list.GetUrls();
		}

		const std::vector<Url> &GetCorsUrls() const
		{
			return _cors_url_list.GetUrls();
		}

	protected:
		void MakeParseList() const override
		{
			Publisher::MakeParseList();

			RegisterValue<Optional>("Port", &_port);
			RegisterValue<Optional>("TLS", &_tls);
			RegisterValue<Optional>("SegmentCount", &_segment_count);
			RegisterValue<Optional>("SegmentDuration", &_segment_duration);
			RegisterValue<Optional>("CrossDoamin", &_cross_domain_list);
			RegisterValue<Optional>("Cors", &_cors_url_list); 				// http(s) 경로 까지 입력
		}

		int _port = 80;
		Tls _tls;
		int _segment_count;
		int _segment_duration;
		Urls _cross_domain_list;
		Urls _cors_url_list;
	};
}
