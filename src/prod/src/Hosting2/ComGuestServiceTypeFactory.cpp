// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;
using namespace Hosting2;

StringLiteral const TraceType("GuestServiceTypeFactory");

ComGuestServiceTypeFactory::ComGuestServiceTypeFactory(
    vector<EndpointDescription> const & endpointDescriptions)
    : endpointDescriptions_(endpointDescriptions)
{
    traceId_ = wformatString("{0}", static_cast<void*>(this));

    WriteNoise(TraceType, traceId_, "ctor");
}

ComGuestServiceTypeFactory::~ComGuestServiceTypeFactory()
{
    WriteNoise(TraceType, traceId_, "dtor");
}

HRESULT ComGuestServiceTypeFactory::CreateInstance( 
    /* [in] */ LPCWSTR serviceType,
    /* [in] */ FABRIC_URI serviceName,
    /* [in] */ ULONG initializationDataLength,
    /* [size_is][in] */ const byte *initializationData,
    /* [in] */ FABRIC_PARTITION_ID partitionId,
    /* [in] */ FABRIC_INSTANCE_ID instanceId,
    /* [retval][out] */ IFabricStatelessServiceInstance **serviceInstance)
{
    UNREFERENCED_PARAMETER(initializationDataLength);
    UNREFERENCED_PARAMETER(initializationData);

    wstring serviceTypeStr(serviceType);
    wstring serviceNameStr(serviceName);
    Guid partitionIdGuid(partitionId);

    WriteNoise(
        TraceType, 
        traceId_, 
        "CreateInstance: ServiceType={0}, ServiceName={1}, PartitionId={2}, InstanceId={3}",
        serviceTypeStr,
        serviceNameStr,
        partitionIdGuid,
        instanceId);

    auto comServiceInstance = 
        make_com<ComGuestServiceInstance, IFabricStatelessServiceInstance>(
            serviceNameStr,
            serviceTypeStr,
            partitionIdGuid,
            instanceId,
            endpointDescriptions_);

    *serviceInstance = comServiceInstance.DetachNoRelease();
    
    return S_OK;
}
