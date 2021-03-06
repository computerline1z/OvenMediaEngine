//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include "relay_datastructure.h"

#include <base/ovsocket/socket.h>
#include <base/application/application.h>
#include <physical_port/physical_port_manager.h>
#include <base/media_route/media_route_application_interface.h>
#include <base/media_route/media_route_application_observer.h>

class MediaRouteStream;

class RelayServer : public PhysicalPortObserver, public MediaRouteApplicationObserver
{
public:
	explicit RelayServer(MediaRouteApplicationInterface *media_route_application, const info::Application &application_info);
	~RelayServer();

	void Send(info::stream_id_t stream_id, const RelayPacket &base_packet, const ov::Data *data);
	void Send(info::stream_id_t stream_id, const RelayPacket &base_packet, const void *data, uint16_t data_size);
	void Send(ov::Socket *socket, info::stream_id_t stream_id, const RelayPacket &base_packet, const ov::Data *data);
	void Send(const std::shared_ptr<MediaRouteStream> &media_stream, MediaPacket *packet);

protected:
	struct ClientInfo
	{
		info::stream_id_t stream_id = 0;
	};

	//--------------------------------------------------------------------
	// Implementation of MediaRouteApplicationObserver
	//--------------------------------------------------------------------
	bool OnCreateStream(std::shared_ptr<StreamInfo> info) override;
	bool OnDeleteStream(std::shared_ptr<StreamInfo> info) override;
	bool OnSendVideoFrame(std::shared_ptr<StreamInfo> stream, std::shared_ptr<MediaTrack> track, std::unique_ptr<EncodedFrame> encoded_frame, std::unique_ptr<CodecSpecificInfo> codec_info, std::unique_ptr<FragmentationHeader> fragmentation) override;
	bool OnSendAudioFrame(std::shared_ptr<StreamInfo> stream, std::shared_ptr<MediaTrack> track, std::unique_ptr<EncodedFrame> encoded_frame, std::unique_ptr<CodecSpecificInfo> codec_info, std::unique_ptr<FragmentationHeader> fragmentation) override;
	ObserverType GetObserverType() override
	{
		return ObserverType::Relay;
	}
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
	// Implementation of PhysicalPortObserver
	//--------------------------------------------------------------------
	void OnConnected(ov::Socket *remote) override;
	void OnDataReceived(ov::Socket *remote, const ov::SocketAddress &address, const std::shared_ptr<const ov::Data> &data) override;
	void OnDisconnected(ov::Socket *remote, PhysicalPortDisconnectReason reason, const std::shared_ptr<const ov::Error> &error) override;
	//--------------------------------------------------------------------

	MediaRouteApplicationInterface *_media_route_application;

	const info::Application &_application_info;
	std::shared_ptr<PhysicalPort> _server_port;

	// All client list
	std::map<ov::Socket *, ClientInfo> _client_list;
	// Client list of specific stream
	std::map<info::stream_id_t, std::vector<ov::Socket *>> _client_map;

	uint32_t _transaction_id = 0;
};